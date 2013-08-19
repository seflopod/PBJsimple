///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/editor.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::Editor functions.

#include "pbj/editor.h"

#include "pbj/gfx/texture_font.h"
#include "pbj/scene/ui_root.h"
#include "pbj/scene/ui_button.h"
#include "pbj/input_controller.h"

#include "pbj/sw/sandwich_open.h"

#include "pbj/look_editor_mode.h"
#include "pbj/add_editor_mode.h"
#include "pbj/move_editor_mode.h"
#include "pbj/rotate_editor_mode.h"
#include "pbj/scale_editor_mode.h"
#include "pbj/decorate_editor_mode.h"
#include "pbj/clobber_editor_mode.h"
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
      menu_visible_counter_(0)
{
    initUI();

    window_.registerContextResizeListener([=](I32 width, I32 height) { onContextResized_(width, height); });

    InputController::registerKeyAllListener([&](I32 keycode, I32 scancode, I32 action, I32 modifiers)
    {

        if (keycode == GLFW_KEY_SPACE ||
            keycode == GLFW_KEY_A ||
            keycode == GLFW_KEY_Q ||
            keycode == GLFW_KEY_W ||
            keycode == GLFW_KEY_E ||
            keycode == GLFW_KEY_R ||
            keycode == GLFW_KEY_F ||
            keycode == GLFW_KEY_Y ||
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
                case GLFW_KEY_Q: setMode(Id("EditorMode.look"));     break;
                case GLFW_KEY_A: setMode(Id("EditorMode.add"));      break;
                case GLFW_KEY_W: setMode(Id("EditorMode.move"));     break;
                case GLFW_KEY_E: setMode(Id("EditorMode.rotate"));   break;
                case GLFW_KEY_R: setMode(Id("EditorMode.scale"));    break;
                case GLFW_KEY_F: setMode(Id("EditorMode.decorate")); break;
                case GLFW_KEY_Y: setMode(Id("EditorMode.clobber"));  break;
                case GLFW_KEY_D: setMode(Id("EditorMode.dupe"));     break;
                case GLFW_KEY_G: setMode(Id("EditorMode.mimic"));    break;
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
}

///////////////////////////////////////////////////////////////////////////////
Editor::~Editor()
{
}


///////////////////////////////////////////////////////////////////////////////
void Editor::initUI()
{
    sw::ResourceId id;
    id.sandwich = Id(PBJ_ID_PBJBASE);

    scene::UIElement* last_focusable = &ui_.panel;

    id.resource = Id("menu_panel");
    ui_.panel.setStyle(id);
    ui_.panel.setVisible(false);
    
    
    menu_ = new scene::UIPanel();
    ui_.panel.addElement(std::unique_ptr<scene::UIElement>(menu_));
    
    scene::UIButton
        *menu_b_look     = newButton_(Id("menu.look_btn"),     "Look",     vec2(0, 0 * 22),       vec2(100, 20), [=]() { setMode(Id("EditorMode.look")); }, menu_, last_focusable),
        *menu_b_add      = newButton_(Id("menu.add_btn"),      "Add",      vec2(0, 1 * 22),       vec2(100, 20), [=]() { setMode(Id("EditorMode.add")); }, menu_, last_focusable),
        *menu_b_move     = newButton_(Id("menu.move_btn"),     "Move",     vec2(0, 2 * 22),       vec2(100, 20), [=]() { setMode(Id("EditorMode.move")); }, menu_, last_focusable),
        *menu_b_rotate   = newButton_(Id("menu.rotate_btn"),   "Rotate",   vec2(0, 3 * 22),       vec2(100, 20), [=]() { setMode(Id("EditorMode.rotate")); }, menu_, last_focusable),
        *menu_b_scale    = newButton_(Id("menu.scale_btn"),    "Scale",    vec2(0, 4 * 22),       vec2(100, 20), [=]() { setMode(Id("EditorMode.scale")); }, menu_, last_focusable),
        *menu_b_decorate = newButton_(Id("menu.decorate_btn"), "Decorate", vec2(0, 5 * 22),       vec2(100, 20), [=]() { setMode(Id("EditorMode.decorate")); }, menu_, last_focusable),
        *menu_b_clobber  = newButton_(Id("menu.clobber_btn"),  "Clobber",  vec2(0, 6 * 22),       vec2(100, 20), [=]() { setMode(Id("EditorMode.clobber")); }, menu_, last_focusable),
        *menu_b_dupe     = newButton_(Id("menu.dupe_btn"),     "Dupe",     vec2(0, 7 * 22),       vec2(100, 20), [=]() { setMode(Id("EditorMode.dupe")); }, menu_, last_focusable),
        *menu_b_mimic    = newButton_(Id("menu.mimic_btn"),    "Mimic",    vec2(0, 8 * 22),       vec2(100, 20), [=]() { setMode(Id("EditorMode.mimic")); }, menu_, last_focusable),
        *menu_b_save     = newButton_(Id("menu.save_btn"),     "Save",     vec2(0, 300 - 2 * 22), vec2(100, 20), [=]() { }, menu_, last_focusable),
        *menu_b_exit     = newButton_(Id("menu.exit_btn"),     "Exit",     vec2(0, 300 - 1 * 22), vec2(100, 20), [&]() { window_.requestClose(); }, menu_, last_focusable);
    
    frame_time_label_ = new scene::UILabel();
    menu_->addElement(std::unique_ptr<scene::UIElement>(frame_time_label_));
    frame_time_label_->setAlign(scene::UILabel::AlignRight);
    frame_time_label_->setDimensions(vec2(200, 10));
    frame_time_label_->setPosition(vec2(400, 290));

    id.resource = Id("std_font");
    frame_time_label_->setFont(&engine_.getResourceManager().getTextureFont(id));
    frame_time_label_->setTextColor(color4(0.5f, 0.6f, 0.65f, 1.0f));

    last_focusable->setNextFocusElement(menu_b_look);
    ui_.clearFocus();
    
    ivec2 wnd_size(window_.getSize());
    onContextResized_(wnd_size.x, wnd_size.y);

    setMode(Id("EditorMode.look"));
}

///////////////////////////////////////////////////////////////////////////////
void Editor::run()
{
    double last_frame_time = 0;
    double fps = 0;

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

        glBegin(GL_TRIANGLES);
        glColor3f(1, 1, 1);
        glVertex2f(-1, -1);
        glVertex2f(1, -1);
        glVertex2f(1, 1);

        glEnd();

        if (menu_->isVisible())
        {
            frame_time_label_->setText(std::to_string(1000.0f * last_frame_time) + " ms");
        }

        scene_->draw();
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
    else if (id == Id("EditorMode.add"))
    {
        current_mode_.reset();
        current_mode_ = std::unique_ptr<EditorMode>(new AddEditorMode(*this));
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
    else if (id == Id("EditorMode.clobber"))
    {
        current_mode_.reset();
        current_mode_ = std::unique_ptr<EditorMode>(new ClobberEditorMode(*this));
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
void Editor::onContextResized_(I32 width, I32 height)
{
    // Place/scale ui correctly
    const ivec2 min_menu_size(600, 300);

    int menu_scale = std::max(1, std::min(width / min_menu_size.x, height / min_menu_size.y));
    vec2 menu_offset = vec2(I32(0.5 * (width - menu_scale * min_menu_size.x)),
                            I32(0.5 * (height - menu_scale * min_menu_size.y)));
    //if (menu_offset.y > menu_offset.x)
    //    menu_offset.y = menu_offset.x;


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

} // namespace pbj
