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

#include <iostream>
#include <thread>

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
Editor::Editor()
    : engine_(getEngine()),
      window_(*getEngine().getWindow())
{
    initUI();

    window_.registerContextResizeListener([=](I32 width, I32 height) { onContextResized_(width, height); });

    InputController::registerKeyAllListener([&](I32 keycode, I32 scancode, I32 action, I32 modifiers)
    {
        if (keycode == GLFW_KEY_GRAVE_ACCENT && action == GLFW_RELEASE && ui_.getFocus() == &ui_.panel)
        {
            if (menu_->isVisible())
                menu_->setVisible(false);
            else
                menu_->setVisible(true);
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
    last_created_focusable_element_ = &ui_.panel;

    
    menu_ = new scene::UIPanel();
    ui_.panel.addElement(std::unique_ptr<scene::UIElement>(menu_));

    scene::UIButton
        *menu_b_place   = newButton_("Place",   vec2(0, 0 * 25),  vec2(100, 22), [=]() { }, menu_),
        *menu_b_move    = newButton_("Move",    vec2(0, 1 * 25),  vec2(100, 22), [=]() { }, menu_),
        *menu_b_rotate  = newButton_("Rotate",  vec2(0, 2 * 25),  vec2(100, 22), [=]() { }, menu_),
        *menu_b_scale   = newButton_("Scale",   vec2(0, 3 * 25),  vec2(100, 22), [=]() { }, menu_),
        *menu_b_change  = newButton_("Change",  vec2(0, 4 * 25),  vec2(100, 22), [=]() { }, menu_),
        *menu_b_clobber = newButton_("Clobber", vec2(0, 5 * 25),  vec2(100, 22), [=]() { }, menu_),
        *menu_b_dupe    = newButton_("Dupe",    vec2(0, 6 * 25),  vec2(100, 22), [=]() { }, menu_),
        *menu_b_mimic   = newButton_("Mimic",   vec2(0, 7 * 25),  vec2(100, 22), [=]() { }, menu_),
        *menu_b_save    = newButton_("Save",    vec2(0, 9 * 30),  vec2(100, 22), [=]() { }, menu_),
        *menu_b_exit    = newButton_("Exit",    vec2(0, 10 * 30), vec2(100, 22), [&]() { window_.requestClose(); }, menu_);

    frame_time_label_ = new scene::UILabel();
    menu_->addElement(std::unique_ptr<scene::UIElement>(frame_time_label_));
    frame_time_label_->setAlign(scene::UILabel::AlignRight);
    frame_time_label_->setDimensions(vec2(200, 22));
    frame_time_label_->setPosition(vec2(550, 375));
    frame_time_label_->setFont(&engine_.getResourceManager().getTextureFont(sw::ResourceId(Id("__pbjbase__"), Id("std_font"))));
    frame_time_label_->setTextColor(color4(0.5f, 0.6f, 0.65f, 1.0f));

    menu_b_exit->setNextFocusElement(menu_b_place);
    
    ivec2 wnd_size(window_.getSize());
    onContextResized_(wnd_size.x, wnd_size.y);
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
void Editor::onContextResized_(I32 width, I32 height)
{
    // Place/scale ui correctly
    const ivec2 min_menu_size(600, 300);

    int menu_scale = std::max(1, std::min(width / min_menu_size.x, height / min_menu_size.y));
    vec2 menu_offset = vec2(I32(0.5 * (width - menu_scale * min_menu_size.x)),
                            I32(0.5 * (height - menu_scale * min_menu_size.y)));
    if (menu_offset.y > menu_offset.x)
        menu_offset.y = menu_offset.x;


    ui_.panel.setPosition(menu_offset);
    ui_.panel.setScale(vec2(menu_scale, menu_scale));
    ui_.panel.setDimensions(vec2(min_menu_size) * float(menu_scale));
}

///////////////////////////////////////////////////////////////////////////////
scene::UIButton* Editor::newButton_(const std::string& text,
                                    const vec2& position,
                                    const vec2& dimensions,
                                    const std::function<void()>& callback,
                                    scene::UIPanel* parent)
{
    scene::UIButton* btn = new scene::UIButton();
    parent->addElement(std::unique_ptr<scene::UIElement>(btn));

    if (last_created_focusable_element_)
        last_created_focusable_element_->setNextFocusElement(btn);

    last_created_focusable_element_ = btn;

    btn->setText(text);
    btn->setPosition(position);
    btn->setDimensions(dimensions);
    
    useButtonConfigs_(btn, "a");

    for (int i = 0; i < 7; ++i)
    {
        scene::UIButtonStateConfig bsc = bsc_a_[i];
        bsc.click_callback = callback;
        btn->setStateConfig(bsc);

        bsc = bsc_b_[i];
        bsc.click_callback = callback;
        btn->setStateConfig(bsc);
    }

    return btn;
}

} // namespace pbj
