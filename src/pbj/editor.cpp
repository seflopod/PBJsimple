///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/editor.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::Editor functions.

#ifdef PBJ_EDITOR
#include "pbj/editor.h"

#include "pbj/gfx/texture_font.h"
#include "pbj/scene/ui_root.h"
#include "pbj/scene/ui_button.h"
#include "pbj/input_controller.h"
#include "pbj/sw/sandwich_open.h"

#include "pbj/look_editor_mode.h"
#include "pbj/add_clobber_editor_mode.h"
#include "pbj/move_editor_mode.h"
#include "pbj/rotate_editor_mode.h"
#include "pbj/scale_editor_mode.h"
#include "pbj/decorate_editor_mode.h"
#include "pbj/dupe_editor_mode.h"
#include "pbj/mimic_editor_mode.h"

#include <iostream>
#include <thread>

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
Editor::Editor()
    : engine_(getEngine()),
      window_(*getEngine().getWindow()),
      menu_toggled_(false),
      menu_visible_counter_(0),
      zoom_(1.0),
      active_scale_(1, 2),
      active_material_("spawnpoint")
{
    mouse_down_mode_[0] = nullptr;
    mouse_down_mode_[1] = nullptr;
    mouse_down_mode_[2] = nullptr;

    initUI();

    setActiveMaterial("spawnpoint", vec2(1, 2));

    window_.registerContextResizeListener([=](I32 width, I32 height)
    {
        onContextResized_(width, height);
    });

    InputController::registerKeyAllListener([&](I32 keycode, I32 scancode, I32 action, I32 modifiers)
    {
        if (keycode == GLFW_KEY_SPACE ||
            keycode == GLFW_KEY_A ||
            keycode == GLFW_KEY_Q ||
            keycode == GLFW_KEY_W ||
            keycode == GLFW_KEY_E ||
            keycode == GLFW_KEY_R ||
            keycode == GLFW_KEY_F ||
            keycode == GLFW_KEY_D ||
            keycode == GLFW_KEY_G)
        {
            menu_visible_counter_ += action == GLFW_PRESS ? 1 : (action == GLFW_RELEASE ? -1 : 0); 
        }

        if ((ui_.getFocus() == nullptr || ui_.getFocus() == &ui_.panel) &&
            action == GLFW_PRESS &&
            ((modifiers & (GLFW_MOD_ALT | GLFW_MOD_SHIFT | GLFW_MOD_CONTROL)) == 0))
        {
            switch (keycode)
            {
                case GLFW_KEY_Q: setMode(Id("EditorMode.look"));        break;
                case GLFW_KEY_A: setMode(Id("EditorMode.add_clobber")); break;
                case GLFW_KEY_W: setMode(Id("EditorMode.move"));        break;
                case GLFW_KEY_E: setMode(Id("EditorMode.rotate"));      break;
                case GLFW_KEY_R: setMode(Id("EditorMode.scale"));       break;
                case GLFW_KEY_F: setMode(Id("EditorMode.decorate"));    break;
                case GLFW_KEY_D: setMode(Id("EditorMode.dupe"));        break;
                case GLFW_KEY_G: setMode(Id("EditorMode.mimic"));       break;
                default: break;
            }
        }

        if (keycode == GLFW_KEY_GRAVE_ACCENT && action == GLFW_RELEASE)
        {
            if (menu_toggled_ && (ui_.getFocus() == &ui_.panel || ui_.getFocus() == nullptr))
            {
                menu_toggled_ = false;
                menu_visible_counter_--;
            }
            else if (!menu_toggled_)
            {
                menu_toggled_ = true;
                menu_visible_counter_++;
            }
        }

        if (menu_visible_counter_ > 0)
            ui_.panel.setVisible(true);
        else
            ui_.panel.setVisible(false);
    });

    InputController::registerMouseButtonAnyListener([&](I32 button, I32 action, I32 modifiers)
    {
        if (current_mode_ && (button == GLFW_MOUSE_BUTTON_1 || button == GLFW_MOUSE_BUTTON_2 || button == GLFW_MOUSE_BUTTON_3))
        {
            EditorMode*& down_mode = mouse_down_mode_[button];

            if (action == GLFW_PRESS && ui_.getElementUnderMouse() == nullptr)
            {
                down_mode = current_mode_.get();
                vec2 pos = getCamera().getWorldPosition(mouse_position_, window_.getContextSize());
                current_mode_->onMouseDown(button, pos);
            }
            else if (action == GLFW_RELEASE && down_mode == current_mode_.get())
            {
                if (ui_.getElementUnderMouse() == nullptr)
                {
                    vec2 pos = getCamera().getWorldPosition(mouse_position_, window_.getContextSize());
                    current_mode_->onMouseUp(button, pos);
                }
                else
                {
                    vec2 pos = getCamera().getWorldPosition(mouse_position_, window_.getContextSize());
                    current_mode_->onMouseCancel(button, pos);
                }
            }
        }
    });

    InputController::registerMouseMotionListener([&](F64 x, F64 y)
    {
        if (current_mode_ && ui_.getElementUnderMouse() == nullptr)
        {
            mouse_position_ = ivec2(I32(x), I32(y));
            vec2 pos = getCamera().getWorldPosition(mouse_position_, window_.getContextSize());
            current_mode_->onMouseMove(pos);
        }
    });

    InputController::registerScrollListener([&](F64 delta_x, F64 delta_y)
    {
        if (current_mode_ && ui_.getElementUnderMouse() == nullptr)
        {
            vec2 pos = getCamera().getWorldPosition(mouse_position_, window_.getContextSize());
            current_mode_->onMouseWheel(I32(delta_y));
        }
    });
}

///////////////////////////////////////////////////////////////////////////////
Editor::~Editor()
{
}


///////////////////////////////////////////////////////////////////////////////
void Editor::initUI()
{
    sw::ResourceId menu_panel_style(Id(PBJ_ID_PBJBASE), Id("menu_panel"));
    sw::ResourceId std_btn_panel_style(Id(PBJ_ID_PBJBASE), Id("std_btn.normal.panel"));
    sw::ResourceId std_font(Id(PBJ_ID_PBJBASE), Id("std_font"));
    color4 label_color(0.5f, 0.6f, 0.65f, 1.0f);

    scene::UIElement* last_focusable = &ui_.panel;

    ui_.panel.setStyle(menu_panel_style);
    ui_.panel.setVisible(false);
    
    menu_ = new scene::UIPanel();
    ui_.panel.addElement(std::unique_ptr<scene::UIElement>(menu_));

    newButton_(Id("menu.look_btn"),     "Look",        vec2(0, 0 * 22), vec2(100, 20), [=]() { setMode(Id("EditorMode.look")); }, menu_, last_focusable);
    newButton_(Id("menu.add_clob_btn"), "Add/Clobber", vec2(0, 1 * 22), vec2(100, 20), [=]() { setMode(Id("EditorMode.add_clobber")); }, menu_, last_focusable);
    newButton_(Id("menu.move_btn"),     "Move",        vec2(0, 2 * 22), vec2(100, 20), [=]() { setMode(Id("EditorMode.move")); }, menu_, last_focusable);
    newButton_(Id("menu.rotate_btn"),   "Rotate",      vec2(0, 3 * 22), vec2(100, 20), [=]() { setMode(Id("EditorMode.rotate")); }, menu_, last_focusable);
    newButton_(Id("menu.scale_btn"),    "Scale",       vec2(0, 4 * 22), vec2(100, 20), [=]() { setMode(Id("EditorMode.scale")); }, menu_, last_focusable);
    newButton_(Id("menu.decorate_btn"), "Decorate",    vec2(0, 5 * 22), vec2(100, 20), [=]() { setMode(Id("EditorMode.decorate")); }, menu_, last_focusable);
    newButton_(Id("menu.dupe_btn"),     "Dupe",        vec2(0, 6 * 22), vec2(100, 20), [=]() { setMode(Id("EditorMode.dupe")); }, menu_, last_focusable)->setDisabled(true);
    newButton_(Id("menu.mimic_btn"),    "Mimic",       vec2(0, 7 * 22), vec2(100, 20), [=]() { setMode(Id("EditorMode.mimic")); }, menu_, last_focusable)->setDisabled(true);

    newButton_(Id("menu.save_btn"),     "Save",        vec2(0, 256),    vec2(100, 20), [=]() { saveMap(); }, menu_, last_focusable),
    newButton_(Id("menu.exit_btn"),     "Exit",        vec2(0, 278),    vec2(100, 20), [&]() { window_.requestClose(); }, menu_, last_focusable);
   
#pragma region look panel
    {
        scene::UIPanel* panel = new scene::UIPanel();
        menu_->addElement(std::unique_ptr<scene::UIElement>(panel));
        ui_elements_[Id("menu.look_panel")] = panel;
        panel->setPosition(vec2(110, 0));
        panel->setDimensions(vec2(490, 50));
        panel->setVisible(false);

        newLabel_(Id("menu.look_panel.hotkey"), "Hotkey: Q", vec2(0, 0), vec2(490, 10), label_color, scene::UILabel::AlignRight, panel);
        newLabel_(Id("menu.look_panel.directions0"), "Drag with the left mouse button to move the camera around.", vec2(0, 0), vec2(490, 10), label_color, scene::UILabel::AlignLeft, panel);
        newLabel_(Id("menu.look_panel.directions1"), "Right click to center the screen on the cursor's position.", vec2(0, 10), vec2(490, 10), label_color, scene::UILabel::AlignLeft, panel);
        newLabel_(Id("menu.look_panel.directions2"), "Scroll wheel zooms in and out.", vec2(0, 20), vec2(490, 10), label_color, scene::UILabel::AlignLeft, panel);

        newLabel_(Id("menu.look_panel.zoom_label"), "Zoom: ", vec2(125, 75), vec2(100, 30), label_color, scene::UILabel::AlignRight, panel)->setTextScale(vec2(2, 2));
        newLabel_(Id("menu.look_panel.zoom"), std::to_string(zoom_) + "x", vec2(225, 75), vec2(150, 30), label_color, scene::UILabel::AlignLeft, panel)->setTextScale(vec2(2, 2));

        newButton_(Id("menu.look_panel.reset_pos_btn"), "Reset Position", vec2(115, 150), vec2(120, 20), [=]() { getCamera().setTargetPosition(vec2()); }, panel, last_focusable);
        newButton_(Id("menu.look_panel.reset_zoom_btn"), "Reset Zoom",    vec2(265, 150), vec2(120, 20), [=]() { setZoom(1.0); }, panel, last_focusable);
    }
#pragma endregion

#pragma region add clobber panel
    {
        scene::UIPanel* panel = new scene::UIPanel();
        menu_->addElement(std::unique_ptr<scene::UIElement>(panel));
        ui_elements_[Id("menu.add_clob_panel")] = panel;
        panel->setPosition(vec2(110, 0));
        panel->setDimensions(vec2(490, 50));
        panel->setVisible(false);
        
        newLabel_(Id("menu.add_clob_panel.hotkey"), "Hotkey: A", vec2(0, 0), vec2(490, 10), label_color, scene::UILabel::AlignRight, panel);
        newLabel_(Id("menu.add_clob_panel.directions0"), "Left click to add a new entity to the scene.", vec2(0, 0), vec2(490, 10), label_color, scene::UILabel::AlignLeft, panel);
        newLabel_(Id("menu.add_clob_panel.directions1"), "Right click to remove the closest entity from the scene.", vec2(0, 10), vec2(490, 10), label_color, scene::UILabel::AlignLeft, panel);
    }
#pragma endregion
    
#pragma region material panel
    {
        scene::UIPanel* panel = new scene::UIPanel();
        menu_->addElement(std::unique_ptr<scene::UIElement>(panel));
        ui_elements_[Id("menu.material_panel")] = panel;
        panel->setPosition(vec2(110, 25));
        panel->setDimensions(vec2(490, 300));
        panel->setVisible(false);

        newLabel_(Id("menu.material_panel.label2x2"),   "2x2",   vec2(5,   5), vec2(50, 9), label_color, scene::UILabel::AlignCenter, panel);
        newLabel_(Id("menu.material_panel.label4x2"),   "4x2",   vec2(55,  5), vec2(50, 9), label_color, scene::UILabel::AlignCenter, panel);
        newLabel_(Id("menu.material_panel.label8x2"),   "8x2",   vec2(105, 5), vec2(50, 9), label_color, scene::UILabel::AlignCenter, panel);
        newLabel_(Id("menu.material_panel.label4x4"),   "4x4",   vec2(155, 5), vec2(50, 9), label_color, scene::UILabel::AlignCenter, panel);
        newLabel_(Id("menu.material_panel.label8x4"),   "8x4",   vec2(205, 5), vec2(50, 9), label_color, scene::UILabel::AlignCenter, panel);
        newLabel_(Id("menu.material_panel.label16x4"),  "16x4",  vec2(255, 5), vec2(50, 9), label_color, scene::UILabel::AlignCenter, panel);
        newLabel_(Id("menu.material_panel.label8x8"),   "8x8",   vec2(305, 5), vec2(50, 9), label_color, scene::UILabel::AlignCenter, panel);
        newLabel_(Id("menu.material_panel.label16x8"),  "16x8",  vec2(355, 5), vec2(50, 9), label_color, scene::UILabel::AlignCenter, panel);
        newLabel_(Id("menu.material_panel.label16x16"), "16x16", vec2(405, 5), vec2(50, 9), label_color, scene::UILabel::AlignCenter, panel);

        newButton_(Id("menu.material_panel.black2x2"),   "black", vec2(5,   15), vec2(50, 16), [=]() { setActiveMaterial( "black2x2",   vec2(2,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.black4x2"),   "black", vec2(55,  15), vec2(50, 16), [=]() { setActiveMaterial( "black4x2",   vec2(4,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.black8x2"),   "black", vec2(105, 15), vec2(50, 16), [=]() { setActiveMaterial( "black8x2",   vec2(8,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.black4x4"),   "black", vec2(155, 15), vec2(50, 16), [=]() { setActiveMaterial( "black4x4",   vec2(4,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.black8x4"),   "black", vec2(205, 15), vec2(50, 16), [=]() { setActiveMaterial( "black8x4",   vec2(8,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.black16x4"),  "black", vec2(255, 15), vec2(50, 16), [=]() { setActiveMaterial( "black16x4",  vec2(16,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.black8x8"),   "black", vec2(305, 15), vec2(50, 16), [=]() { setActiveMaterial( "black8x8",   vec2(8,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.black16x8"),  "black", vec2(355, 15), vec2(50, 16), [=]() { setActiveMaterial( "black16x8",  vec2(16,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.black16x16"), "black", vec2(405, 15), vec2(50, 16), [=]() { setActiveMaterial( "black16x16", vec2(16,16)); }, panel, last_focusable);
        
        newButton_(Id("menu.material_panel.white2x2"),   "white", vec2(5,   31), vec2(50, 16), [=]() { setActiveMaterial( "white2x2",   vec2(2,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.white4x2"),   "white", vec2(55,  31), vec2(50, 16), [=]() { setActiveMaterial( "white4x2",   vec2(4,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.white8x2"),   "white", vec2(105, 31), vec2(50, 16), [=]() { setActiveMaterial( "white8x2",   vec2(8,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.white4x4"),   "white", vec2(155, 31), vec2(50, 16), [=]() { setActiveMaterial( "white4x4",   vec2(4,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.white8x4"),   "white", vec2(205, 31), vec2(50, 16), [=]() { setActiveMaterial( "white8x4",   vec2(8,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.white16x4"),  "white", vec2(255, 31), vec2(50, 16), [=]() { setActiveMaterial( "white16x4",  vec2(16,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.white8x8"),   "white", vec2(305, 31), vec2(50, 16), [=]() { setActiveMaterial( "white8x8",   vec2(8,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.white16x8"),  "white", vec2(355, 31), vec2(50, 16), [=]() { setActiveMaterial( "white16x8",  vec2(16,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.white16x16"), "white", vec2(405, 31), vec2(50, 16), [=]() { setActiveMaterial( "white16x16", vec2(16,16)); }, panel, last_focusable);
        
        newButton_(Id("menu.material_panel.red2x2"),   "red", vec2(5,   47), vec2(50, 16), [=]() { setActiveMaterial( "red2x2",   vec2(2,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.red4x2"),   "red", vec2(55,  47), vec2(50, 16), [=]() { setActiveMaterial( "red4x2",   vec2(4,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.red8x2"),   "red", vec2(105, 47), vec2(50, 16), [=]() { setActiveMaterial( "red8x2",   vec2(8,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.red4x4"),   "red", vec2(155, 47), vec2(50, 16), [=]() { setActiveMaterial( "red4x4",   vec2(4,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.red8x4"),   "red", vec2(205, 47), vec2(50, 16), [=]() { setActiveMaterial( "red8x4",   vec2(8,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.red16x4"),  "red", vec2(255, 47), vec2(50, 16), [=]() { setActiveMaterial( "red16x4",  vec2(16,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.red8x8"),   "red", vec2(305, 47), vec2(50, 16), [=]() { setActiveMaterial( "red8x8",   vec2(8,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.red16x8"),  "red", vec2(355, 47), vec2(50, 16), [=]() { setActiveMaterial( "red16x8",  vec2(16,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.red16x16"), "red", vec2(405, 47), vec2(50, 16), [=]() { setActiveMaterial( "red16x16", vec2(16,16)); }, panel, last_focusable);
        
        newButton_(Id("menu.material_panel.orange2x2"),   "orange", vec2(5,   63), vec2(50, 16), [=]() { setActiveMaterial( "orange2x2",   vec2(2,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.orange4x2"),   "orange", vec2(55,  63), vec2(50, 16), [=]() { setActiveMaterial( "orange4x2",   vec2(4,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.orange8x2"),   "orange", vec2(105, 63), vec2(50, 16), [=]() { setActiveMaterial( "orange8x2",   vec2(8,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.orange4x4"),   "orange", vec2(155, 63), vec2(50, 16), [=]() { setActiveMaterial( "orange4x4",   vec2(4,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.orange8x4"),   "orange", vec2(205, 63), vec2(50, 16), [=]() { setActiveMaterial( "orange8x4",   vec2(8,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.orange16x4"),  "orange", vec2(255, 63), vec2(50, 16), [=]() { setActiveMaterial( "orange16x4",  vec2(16,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.orange8x8"),   "orange", vec2(305, 63), vec2(50, 16), [=]() { setActiveMaterial( "orange8x8",   vec2(8,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.orange16x8"),  "orange", vec2(355, 63), vec2(50, 16), [=]() { setActiveMaterial( "orange16x8",  vec2(16,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.orange16x16"), "orange", vec2(405, 63), vec2(50, 16), [=]() { setActiveMaterial( "orange16x16", vec2(16,16)); }, panel, last_focusable);
        
        newButton_(Id("menu.material_panel.lime2x2"),   "lime", vec2(5,   79), vec2(50, 16), [=]() { setActiveMaterial( "lime2x2",   vec2(2,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.lime4x2"),   "lime", vec2(55,  79), vec2(50, 16), [=]() { setActiveMaterial( "lime4x2",   vec2(4,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.lime8x2"),   "lime", vec2(105, 79), vec2(50, 16), [=]() { setActiveMaterial( "lime8x2",   vec2(8,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.lime4x4"),   "lime", vec2(155, 79), vec2(50, 16), [=]() { setActiveMaterial( "lime4x4",   vec2(4,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.lime8x4"),   "lime", vec2(205, 79), vec2(50, 16), [=]() { setActiveMaterial( "lime8x4",   vec2(8,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.lime16x4"),  "lime", vec2(255, 79), vec2(50, 16), [=]() { setActiveMaterial( "lime16x4",  vec2(16,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.lime8x8"),   "lime", vec2(305, 79), vec2(50, 16), [=]() { setActiveMaterial( "lime8x8",   vec2(8,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.lime16x8"),  "lime", vec2(355, 79), vec2(50, 16), [=]() { setActiveMaterial( "lime16x8",  vec2(16,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.lime16x16"), "lime", vec2(405, 79), vec2(50, 16), [=]() { setActiveMaterial( "lime16x16", vec2(16,16)); }, panel, last_focusable);
        
        newButton_(Id("menu.material_panel.green2x2"),   "green", vec2(5,   95), vec2(50, 16), [=]() { setActiveMaterial( "green2x2",   vec2(2,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.green4x2"),   "green", vec2(55,  95), vec2(50, 16), [=]() { setActiveMaterial( "green4x2",   vec2(4,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.green8x2"),   "green", vec2(105, 95), vec2(50, 16), [=]() { setActiveMaterial( "green8x2",   vec2(8,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.green4x4"),   "green", vec2(155, 95), vec2(50, 16), [=]() { setActiveMaterial( "green4x4",   vec2(4,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.green8x4"),   "green", vec2(205, 95), vec2(50, 16), [=]() { setActiveMaterial( "green8x4",   vec2(8,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.green16x4"),  "green", vec2(255, 95), vec2(50, 16), [=]() { setActiveMaterial( "green16x4",  vec2(16,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.green8x8"),   "green", vec2(305, 95), vec2(50, 16), [=]() { setActiveMaterial( "green8x8",   vec2(8,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.green16x8"),  "green", vec2(355, 95), vec2(50, 16), [=]() { setActiveMaterial( "green16x8",  vec2(16,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.green16x16"), "green", vec2(405, 95), vec2(50, 16), [=]() { setActiveMaterial( "green16x16", vec2(16,16)); }, panel, last_focusable);
        
        newButton_(Id("menu.material_panel.cyan2x2"),   "cyan", vec2(5,   111), vec2(50, 16), [=]() { setActiveMaterial( "cyan2x2",   vec2(2,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.cyan4x2"),   "cyan", vec2(55,  111), vec2(50, 16), [=]() { setActiveMaterial( "cyan4x2",   vec2(4,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.cyan8x2"),   "cyan", vec2(105, 111), vec2(50, 16), [=]() { setActiveMaterial( "cyan8x2",   vec2(8,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.cyan4x4"),   "cyan", vec2(155, 111), vec2(50, 16), [=]() { setActiveMaterial( "cyan4x4",   vec2(4,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.cyan8x4"),   "cyan", vec2(205, 111), vec2(50, 16), [=]() { setActiveMaterial( "cyan8x4",   vec2(8,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.cyan16x4"),  "cyan", vec2(255, 111), vec2(50, 16), [=]() { setActiveMaterial( "cyan16x4",  vec2(16,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.cyan8x8"),   "cyan", vec2(305, 111), vec2(50, 16), [=]() { setActiveMaterial( "cyan8x8",   vec2(8,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.cyan16x8"),  "cyan", vec2(355, 111), vec2(50, 16), [=]() { setActiveMaterial( "cyan16x8",  vec2(16,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.cyan16x16"), "cyan", vec2(405, 111), vec2(50, 16), [=]() { setActiveMaterial( "cyan16x16", vec2(16,16)); }, panel, last_focusable);
        
        newButton_(Id("menu.material_panel.blue2x2"),   "blue", vec2(5,   127), vec2(50, 16), [=]() { setActiveMaterial( "blue2x2",   vec2(2,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.blue4x2"),   "blue", vec2(55,  127), vec2(50, 16), [=]() { setActiveMaterial( "blue4x2",   vec2(4,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.blue8x2"),   "blue", vec2(105, 127), vec2(50, 16), [=]() { setActiveMaterial( "blue8x2",   vec2(8,2)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.blue4x4"),   "blue", vec2(155, 127), vec2(50, 16), [=]() { setActiveMaterial( "blue4x4",   vec2(4,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.blue8x4"),   "blue", vec2(205, 127), vec2(50, 16), [=]() { setActiveMaterial( "blue8x4",   vec2(8,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.blue16x4"),  "blue", vec2(255, 127), vec2(50, 16), [=]() { setActiveMaterial( "blue16x4",  vec2(16,4)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.blue8x8"),   "blue", vec2(305, 127), vec2(50, 16), [=]() { setActiveMaterial( "blue8x8",   vec2(8,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.blue16x8"),  "blue", vec2(355, 127), vec2(50, 16), [=]() { setActiveMaterial( "blue16x8",  vec2(16,8)); }, panel, last_focusable);
        newButton_(Id("menu.material_panel.blue16x16"), "blue", vec2(405, 127), vec2(50, 16), [=]() { setActiveMaterial( "blue16x16", vec2(16,16)); }, panel, last_focusable);
        
        newButton_(Id("menu.material_panel.indigo2x2"),   "indigo", vec2(5,   143), vec2(50, 16), [=]() { setActiveMaterial( "indigo2x2",   vec2(2,2)); }, panel, last_focusable);  
        newButton_(Id("menu.material_panel.indigo4x2"),   "indigo", vec2(55,  143), vec2(50, 16), [=]() { setActiveMaterial( "indigo4x2",   vec2(4,2)); }, panel, last_focusable);  
        newButton_(Id("menu.material_panel.indigo8x2"),   "indigo", vec2(105, 143), vec2(50, 16), [=]() { setActiveMaterial( "indigo8x2",   vec2(8,2)); }, panel, last_focusable);  
        newButton_(Id("menu.material_panel.indigo4x4"),   "indigo", vec2(155, 143), vec2(50, 16), [=]() { setActiveMaterial( "indigo4x4",   vec2(4,4)); }, panel, last_focusable);  
        newButton_(Id("menu.material_panel.indigo8x4"),   "indigo", vec2(205, 143), vec2(50, 16), [=]() { setActiveMaterial( "indigo8x4",   vec2(8,4)); }, panel, last_focusable);  
        newButton_(Id("menu.material_panel.indigo16x4"),  "indigo", vec2(255, 143), vec2(50, 16), [=]() { setActiveMaterial( "indigo16x4",  vec2(16,4)); }, panel, last_focusable); 
        newButton_(Id("menu.material_panel.indigo8x8"),   "indigo", vec2(305, 143), vec2(50, 16), [=]() { setActiveMaterial( "indigo8x8",   vec2(8,8)); }, panel, last_focusable);  
        newButton_(Id("menu.material_panel.indigo16x8"),  "indigo", vec2(355, 143), vec2(50, 16), [=]() { setActiveMaterial( "indigo16x8",  vec2(16,8)); }, panel, last_focusable); 
        newButton_(Id("menu.material_panel.indigo16x16"), "indigo", vec2(405, 143), vec2(50, 16), [=]() { setActiveMaterial( "indigo16x16", vec2(16,16)); }, panel, last_focusable);
        
        newButton_(Id("menu.material_panel.magenta2x2"),   "magenta", vec2(5,   159), vec2(50, 16), [=]() { setActiveMaterial( "magenta2x2",   vec2(2,2)); }, panel, last_focusable);  
        newButton_(Id("menu.material_panel.magenta4x2"),   "magenta", vec2(55,  159), vec2(50, 16), [=]() { setActiveMaterial( "magenta4x2",   vec2(4,2)); }, panel, last_focusable);  
        newButton_(Id("menu.material_panel.magenta8x2"),   "magenta", vec2(105, 159), vec2(50, 16), [=]() { setActiveMaterial( "magenta8x2",   vec2(8,2)); }, panel, last_focusable);  
        newButton_(Id("menu.material_panel.magenta4x4"),   "magenta", vec2(155, 159), vec2(50, 16), [=]() { setActiveMaterial( "magenta4x4",   vec2(4,4)); }, panel, last_focusable);  
        newButton_(Id("menu.material_panel.magenta8x4"),   "magenta", vec2(205, 159), vec2(50, 16), [=]() { setActiveMaterial( "magenta8x4",   vec2(8,4)); }, panel, last_focusable);  
        newButton_(Id("menu.material_panel.magenta16x4"),  "magenta", vec2(255, 159), vec2(50, 16), [=]() { setActiveMaterial( "magenta16x4",  vec2(16,4)); }, panel, last_focusable); 
        newButton_(Id("menu.material_panel.magenta8x8"),   "magenta", vec2(305, 159), vec2(50, 16), [=]() { setActiveMaterial( "magenta8x8",   vec2(8,8)); }, panel, last_focusable);  
        newButton_(Id("menu.material_panel.magenta16x8"),  "magenta", vec2(355, 159), vec2(50, 16), [=]() { setActiveMaterial( "magenta16x8",  vec2(16,8)); }, panel, last_focusable); 
        newButton_(Id("menu.material_panel.magenta16x16"), "magenta", vec2(405, 159), vec2(50, 16), [=]() { setActiveMaterial( "magenta16x16", vec2(16,16)); }, panel, last_focusable);

        newButton_(Id("menu.material_panel.spawnpoint"), "Spawn Point", vec2(355, 177), vec2(100, 16), [=]() { setActiveMaterial( "spawnpoint", vec2(1,2)); }, panel, last_focusable);
    }
#pragma endregion

#pragma region move panel
    {
        scene::UIPanel* panel = new scene::UIPanel();
        menu_->addElement(std::unique_ptr<scene::UIElement>(panel));
        ui_elements_[Id("menu.move_panel")] = panel;
        panel->setPosition(vec2(110, 0));
        panel->setDimensions(vec2(490, 50));
        panel->setVisible(false);
        
        newLabel_(Id("menu.move_panel.hotkey"), "Hotkey: W", vec2(0, 0), vec2(490, 10), label_color, scene::UILabel::AlignRight, panel);
        newLabel_(Id("menu.move_panel.directions0"), "Drag to move the closest entity.", vec2(0, 0), vec2(490, 10), label_color, scene::UILabel::AlignLeft, panel);
    }
#pragma endregion

#pragma region scale panel
    {
        scene::UIPanel* panel = new scene::UIPanel();
        menu_->addElement(std::unique_ptr<scene::UIElement>(panel));
        ui_elements_[Id("menu.scale_panel")] = panel;
        panel->setPosition(vec2(110, 0));
        panel->setDimensions(vec2(490, 50));
        panel->setVisible(false);
        
        newLabel_(Id("menu.scale_panel.hotkey"), "Hotkey: R", vec2(0, 0), vec2(490, 10), label_color, scene::UILabel::AlignRight, panel);
        newLabel_(Id("menu.scale_panel.directions0"), "Drag to change the size of the closest entity.", vec2(0, 0), vec2(490, 10), label_color, scene::UILabel::AlignLeft, panel);
    }
#pragma endregion

#pragma region rotate panel
    {
        scene::UIPanel* panel = new scene::UIPanel();
        menu_->addElement(std::unique_ptr<scene::UIElement>(panel));
        ui_elements_[Id("menu.rotate_panel")] = panel;
        panel->setPosition(vec2(110, 0));
        panel->setDimensions(vec2(490, 50));
        panel->setVisible(false);
        
        newLabel_(Id("menu.rotate_panel.hotkey"), "Hotkey: E", vec2(0, 0), vec2(490, 10), label_color, scene::UILabel::AlignRight, panel);
        newLabel_(Id("menu.rotate_panel.directions0"), "Drag to rotate the closest entity.", vec2(0, 0), vec2(490, 10), label_color, scene::UILabel::AlignLeft, panel);
    }
#pragma endregion

#pragma region decorate panel
    {
        scene::UIPanel* panel = new scene::UIPanel();
        menu_->addElement(std::unique_ptr<scene::UIElement>(panel));
        ui_elements_[Id("menu.decorate_panel")] = panel;
        panel->setPosition(vec2(110, 0));
        panel->setDimensions(vec2(490, 50));
        panel->setVisible(false);
        
        newLabel_(Id("menu.decorate_panel.hotkey"), "Hotkey: F", vec2(0, 0), vec2(490, 10), label_color, scene::UILabel::AlignRight, panel);
        newLabel_(Id("menu.decorate_panel.directions0"), "Click to assign a material and/or type to the closest entity.", vec2(0, 0), vec2(490, 10), label_color, scene::UILabel::AlignLeft, panel);
    }
#pragma endregion

    newLabel_(Id("menu.map_name_lbl"), "", vec2(5, 240), vec2(300, 10), label_color, scene::UILabel::AlignLeft, menu_);
    newLabel_(Id("menu.map_id_lbl"), "", vec2(5, 230), vec2(300, 10), label_color, scene::UILabel::AlignLeft, menu_);
   
    frame_time_label_ = newLabel_(Id("menu.frame_time_lbl"), "", vec2(400, 290), vec2(200, 10), label_color, scene::UILabel::AlignRight, menu_);

    last_focusable->setNextFocusElement(ui_elements_[Id("menu.look_btn")]);
    ui_.clearFocus();

    setMode(Id("EditorMode.look"));
}

///////////////////////////////////////////////////////////////////////////////
void Editor::run(const std::string& sw_id, const std::string& map_id)
{
    map_id_.sandwich = Id(sw_id);
    map_id_.resource = Id(map_id);

    std::shared_ptr<sw::Sandwich> sandwich = sw::open(map_id_.sandwich);
    if (sandwich)
    {
        scene_ = scene::loadScene(*sandwich, map_id_.resource);
        camera_.reset(new scene::CameraComponent(nullptr));
        onContextResized_(window_.getContextSize().x, window_.getContextSize().y);
    }
    else
        return;

    ((scene::UILabel*)ui_elements_[Id("menu.map_id_lbl")])->setText(map_id_.to_string());
    ((scene::UILabel*)ui_elements_[Id("menu.map_name_lbl")])->setText(scene_->getName());

    double last_frame_time = 0;
    double fps = 0;
    double last_sim_time = 0;

    while (true)
    {
        double frame_start = glfwGetTime();
        
        glfwPollEvents();

        if (window_.isClosePending())
            break;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        if (menu_->isVisible())
        {
            frame_time_label_->setText(std::to_string(1000.0f * last_frame_time) + " ms");
        }

        if (scene_)
        {
            scene::CameraComponent& camera = getCamera();

            if (last_sim_time == 0)
                last_sim_time = frame_start;

            camera.update(F32(frame_start - last_sim_time));
            camera.use();
            last_sim_time = frame_start;

            ivec2 ctx_size = window_.getContextSize();
            vec2 bottom_left = camera.getWorldPosition(ivec2(-1, ctx_size.y), ctx_size);
            if (bottom_left.y < -50.0f)
            {
                // display the kill zone quad

                vec2 bottom_right = camera.getWorldPosition(ctx_size, ctx_size);

                glColor4f(1, 0, 0, 0.4);
                glBegin(GL_QUADS);

                glVertex2f(bottom_left.x, -50.0f);
                glVertex2f(bottom_right.x, -50.0f);
                glVertex2f(bottom_right.x, bottom_right.y);
                glVertex2f(bottom_left.x, bottom_left.y);
                glEnd();
            }


            
            glBegin(GL_QUADS);
            glColor3f(1, 0, 0);
            glVertex2f(0, 0);
            glVertex2f(1, 0);
            glColor3f(0, 1, 0);
            glVertex2f(0, 0);
            glVertex2f(0, 1);
            glEnd();

            scene_->draw();
        }

        ui_.draw();

        GLenum gl_error;
        while ((gl_error = glGetError()) != GL_NO_ERROR)
        {
            PBJ_LOG(VWarning) << "OpenGL error while rendering frame!" << PBJ_LOG_NL
                                   << "Error Code: " << gl_error << PBJ_LOG_NL
                                   << "     Error: " << getGlErrorString(gl_error) << PBJ_LOG_END;
        }

        glfwSwapBuffers(window_.getGlfwHandle());

        double frame_time = last_frame_time = glfwGetTime() - frame_start;
        if (last_frame_time < (1.0 / 180.0))
        {
            std::this_thread::sleep_for(std::chrono::microseconds(int(1000000 * ((1.0 / 180.0) - last_frame_time))));
            frame_time = glfwGetTime() - frame_start;
        }
        fps = 1.0 / frame_time;
    }
}

///////////////////////////////////////////////////////////////////////////////
scene::UIElement* Editor::getUIElement(const Id& id)
{
    auto i = ui_elements_.find(id);
    if (i != ui_elements_.end())
        return i->second;

    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
void Editor::highlightUIButton(scene::UIButton* btn)
{
    if (btn)
    {
        btn->setStyle(scene::UIButton::SNormal,         sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("highlight_btn.normal")));
        btn->setStyle(scene::UIButton::SHovered,        sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("highlight_btn.hovered")));
        btn->setStyle(scene::UIButton::SActive,         sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("highlight_btn.active")));
        btn->setStyle(scene::UIButton::SFocused,        sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("highlight_btn.focused")));
        btn->setStyle(scene::UIButton::SFocusedHovered, sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("highlight_btn.focused_hovered")));
        btn->setStyle(scene::UIButton::SFocusedActive,  sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("highlight_btn.focused_active")));
        btn->setStyle(scene::UIButton::SDisabled,       sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("highlight_btn.disabled")));
    }
}

///////////////////////////////////////////////////////////////////////////////
void Editor::unhighlightUIButton(scene::UIButton* btn)
{
    if (btn)
    {
        btn->setStyle(scene::UIButton::SNormal,         sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.normal")));
        btn->setStyle(scene::UIButton::SHovered,        sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.hovered")));
        btn->setStyle(scene::UIButton::SActive,         sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.active")));
        btn->setStyle(scene::UIButton::SFocused,        sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.focused")));
        btn->setStyle(scene::UIButton::SFocusedHovered, sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.focused_hovered")));
        btn->setStyle(scene::UIButton::SFocusedActive,  sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.focused_active")));
        btn->setStyle(scene::UIButton::SDisabled,       sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.disabled")));
    }
}

///////////////////////////////////////////////////////////////////////////////
EditorMode* Editor::getCurrentMode()
{
    return current_mode_.get();
}

///////////////////////////////////////////////////////////////////////////////
void Editor::setMode(const Id& id)
{
    if (current_mode_ && current_mode_->getId() == id)
        return;

    if (id == Id("EditorMode.look"))
    {
        current_mode_.reset();
        current_mode_ = std::unique_ptr<EditorMode>(new LookEditorMode(*this));
    }
    else if (id == Id("EditorMode.add_clobber"))
    {
        current_mode_.reset();
        current_mode_ = std::unique_ptr<EditorMode>(new AddClobberEditorMode(*this));
    }
    else if (id == Id("EditorMode.move"))
    {
        current_mode_.reset();
        current_mode_ = std::unique_ptr<EditorMode>(new MoveEditorMode(*this));
    }
    else if (id == Id("EditorMode.rotate"))
    {
        current_mode_.reset();
        current_mode_ = std::unique_ptr<EditorMode>(new RotateEditorMode(*this));
    }
    else if (id == Id("EditorMode.scale"))
    {
        current_mode_.reset();
        current_mode_ = std::unique_ptr<EditorMode>(new ScaleEditorMode(*this));
    }
    else if (id == Id("EditorMode.decorate"))
    {
        current_mode_.reset();
        current_mode_ = std::unique_ptr<EditorMode>(new DecorateEditorMode(*this));
    }
    else if (id == Id("EditorMode.dupe"))
    {
        current_mode_.reset();
        current_mode_ = std::unique_ptr<EditorMode>(new DupeEditorMode(*this));
    }
    else if (id == Id("EditorMode.mimic"))
    {
        current_mode_.reset();
        current_mode_ = std::unique_ptr<EditorMode>(new MimicEditorMode(*this));
    }
}

///////////////////////////////////////////////////////////////////////////////
scene::CameraComponent& Editor::getCamera() const
{
    return *camera_;
}

///////////////////////////////////////////////////////////////////////////////
F64 Editor::getZoom() const
{
    return zoom_;
}

///////////////////////////////////////////////////////////////////////////////
void Editor::setZoom(F64 zoom)
{
    if (zoom_ != zoom)
    {
        zoom_ = zoom;
        ivec2 viewport = window_.getContextSize();
        ((scene::UILabel*)(ui_elements_[Id("menu.look_panel.zoom")]))->setText(std::to_string(1.0 / zoom_) + "x");
        onContextResized_(viewport.x, viewport.y);
    }
}

///////////////////////////////////////////////////////////////////////////////
std::pair<scene::Entity*, F32> Editor::getClosestEntity(const vec2& world_coords, bool include_spawnpoints, bool include_terrain)
{
    std::pair<scene::Entity*, F32> result;
    result.first = nullptr;
    result.second = -1;
    
    if (include_spawnpoints)
    {
        for (auto i(scene_->_spawnPoints.begin()), end(scene_->_spawnPoints.end()); i != end; ++i)
        {
            scene::Entity* entity = i->second.get();

            F32 distance = glm::length(entity->getTransform().getPosition() - world_coords);
            if (result.second < 0 || distance < result.second)
            {
                result.first = entity;
                result.second = distance;
            }
        }
    }

    if (include_terrain)
    {
        for (auto i(scene_->_terrain.begin()), end(scene_->_terrain.end()); i != end; ++i)
        {
            scene::Entity* entity = i->second.get();

            F32 distance = glm::length(entity->getTransform().getPosition() - world_coords);
            if (result.second < 0 || distance < result.second)
            {
                result.first = entity;
                result.second = distance;
            }
        }
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////
void Editor::addEntity(std::unique_ptr<scene::Entity>&& entity)
{
    scene_->addEntity(std::move(entity));
}

///////////////////////////////////////////////////////////////////////////////
void Editor::removeEntity(scene::Entity* entity)
{
    scene_->removeEntity(entity->getSceneId(), entity->getType());
}

///////////////////////////////////////////////////////////////////////////////
const std::string& Editor::getActiveMaterial() const
{
    return active_material_;
}

///////////////////////////////////////////////////////////////////////////////
const vec2& Editor::getActiveScale() const
{
    return active_scale_;
}

///////////////////////////////////////////////////////////////////////////////
void Editor::setActiveMaterial(const std::string& name, const vec2& scale)
{
    scene::UIButton* oldmat = (scene::UIButton*)ui_elements_[Id("menu.material_panel." + active_material_)];
    scene::UIButton* newmat = (scene::UIButton*)ui_elements_[Id("menu.material_panel." + name)];

    active_material_ = name;
    active_scale_ = scale;

    unhighlightUIButton(oldmat);
    //oldmat->setDisabled(false);

    highlightUIButton(newmat);
    //newmat->setDisabled(true);
}

///////////////////////////////////////////////////////////////////////////////
void Editor::saveMap()
{
    scene_->saveScene(map_id_.sandwich, map_id_.resource);
}

///////////////////////////////////////////////////////////////////////////////
void Editor::onContextResized_(I32 width, I32 height)
{
    // Place/scale ui correctly
    const ivec2 min_menu_size(600, 300);

    int menu_scale = std::max(1, std::min(width / min_menu_size.x, height / min_menu_size.y));
    vec2 menu_offset = vec2(I32(0.5 * (width - menu_scale * min_menu_size.x)),
                            I32(0.5 * (height - menu_scale * min_menu_size.y)));
    //if (menu_offset.y > menu_offset.x)
    //    menu_offset.y = menu_offset.x;

    if (scene_)
    {
        F32 ratio = width / F32(height);
        F32 hheight = float(zoom_ * 25.0f);
        F32 hwidth = ratio * hheight;
        getCamera().setProjection(glm::ortho(-hwidth, hwidth, -hheight, hheight));
    }

    menu_->setPosition(menu_offset);
    menu_->setScale(vec2(menu_scale, menu_scale));
    menu_->setDimensions(vec2(min_menu_size) * float(menu_scale));
}

///////////////////////////////////////////////////////////////////////////////
scene::UIButton* Editor::newButton_(const Id& id,
                                    const std::string& text,
                                    const vec2& position,
                                    const vec2& dimensions,
                                    const std::function<void()>& callback,
                                    scene::UIPanel* parent,
                                    scene::UIElement*& previous_focusable)
{
    scene::UIButton* btn = new scene::UIButton();
    parent->addElement(std::unique_ptr<scene::UIElement>(btn));
    ui_elements_[id] = btn;

    if (previous_focusable)
        previous_focusable->setNextFocusElement(btn);

    previous_focusable = btn;

    btn->setText(text);
    btn->setPosition(position);
    btn->setDimensions(dimensions);
    btn->setClickCallback(callback);

    return btn;
}

scene::UILabel* Editor::newLabel_(const Id& id,
                                  const std::string& text,
                                  const vec2& position,
                                  const vec2& dimensions,
                                  const color4& color,
                                  scene::UILabel::Align align,
                                  scene::UIPanel* parent)
{
    scene::UILabel* lbl = new scene::UILabel();
    parent->addElement(std::unique_ptr<scene::UILabel>(lbl));
    ui_elements_[id] = lbl;

    lbl->setText(text);
    lbl->setPosition(position);
    lbl->setDimensions(dimensions);
    lbl->setTextColor(color);
    lbl->setAlign(align);
    lbl->setFont(&engine_.getResourceManager().getTextureFont(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_font"))));

    return lbl;
}

} // namespace pbj

#endif
