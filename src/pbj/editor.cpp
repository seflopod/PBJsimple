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
        if (keycode == GLFW_KEY_SPACE && action == GLFW_RELEASE && scene_.ui.getFocus() == &scene_.ui.panel)
        {
            //if (menu_->isVisible())
            //    menu_->setVisible(false);
            //else if (action == GLFW_RELEASE)
            //    menu_->setVisible(true);
        }
    });

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

///////////////////////////////////////////////////////////////////////////////
Editor::~Editor()
{
}


///////////////////////////////////////////////////////////////////////////////
void Editor::initUI()
{
  /*  generateButtonStateConfigs_(color3(0.5f, 0.6f, 0.65f), bsc_a_, "a");
    generateButtonStateConfigs_(color3(0.6f, 0.5f, 0.45f), bsc_b_, "b");

    last_created_focusable_element_ = &scene_.ui.panel;

    scene::UIPanel* menu_parent = new scene::UIPanel();
    menu_ = new scene::UIPanel();
    scene_.ui.panel.addElement(std::unique_ptr<scene::UIElement>(menu_parent));
    menu_parent->addElement(std::unique_ptr<scene::UIElement>(menu_));
    ui_elements_[Id("menu")] = menu_;

    scene::UIButton
        *menu_b_databases = newButton_(Id("menu_b_databases"), "Databases", vec2(0, 0 * 30),  vec2(100, 22), [=]() { setMode(MDatabases); },    menu_),
        *menu_b_levels    = newButton_(Id("menu_b_levels"),    "Levels",    vec2(0, 1 * 30),  vec2(100, 22), [=]() { setMode(MLevels); },       menu_),
        *menu_b_world     = newButton_(Id("menu_b_world"),     "World",     vec2(0, 2 * 30),  vec2(100, 22), [=]() { setMode(MWorld); },        menu_),
        *menu_b_objects   = newButton_(Id("menu_b_objects"),   "Objects",   vec2(0, 3 * 30),  vec2(100, 22), [=]() { setMode(MObjects); },      menu_),
        *menu_b_textures  = newButton_(Id("menu_b_textures"),  "Textures",  vec2(0, 4 * 30),  vec2(100, 22), [=]() { setMode(MTextures); },     menu_),
        *menu_b_settings  = newButton_(Id("menu_b_settings"),  "Settings",  vec2(0, 11 * 30), vec2(100, 22), [=]() { setMode(MSettings); },     menu_),
        *menu_b_exit      = newButton_(Id("menu_b_exit"),      "Exit",      vec2(0, 12 * 30), vec2(100, 22), [&]() { window_.requestClose(); }, menu_);

    frame_time_label_ = new scene::UILabel();
    menu_->addElement(std::unique_ptr<scene::UIElement>(frame_time_label_));
    frame_time_label_->setAlign(scene::UILabel::AlignRight);
    frame_time_label_->setDimensions(vec2(200, 22));
    frame_time_label_->setPosition(vec2(550, 375));
    frame_time_label_->setFont(builtins_.getTextureFont(Id("TextureFont.default")).getHandle());
    frame_time_label_->setTextColor(color4(0.5f, 0.6f, 0.65f, 1.0f));

    //menu_b_levels->setDisabled(true);
    //menu_b_textures->setDisabled(true);
    menu_b_settings->setDisabled(true);
    menu_b_exit->setNextFocusElement(menu_b_databases);
    
    newRootPanel_(MDatabases, Id("p_databases"), color3(0.1f, 0.12f, 0.13f));
    newRootPanel_(MLevels,    Id("p_levels"),    color3(0.1f, 0.12f, 0.13f));
    newRootPanel_(MObjects,   Id("p_objects"),   color3(0.1f, 0.12f, 0.13f));
    newRootPanel_(MTextures,  Id("p_textures"),  color3(0.1f, 0.12f, 0.13f));
    newRootPanel_(MSettings,  Id("p_settings"),  color3(0.1f, 0.12f, 0.13f));


    scene::UIListbox
        *lb_databases = newListbox_(Id("lb_databases"), color3(0.5f, 0.6f, 0.65f), vec2(50, 50), vec2(300, 500), panels_[MDatabases]);

    lb_databases->model = std::unique_ptr<scene::UIListboxModel>(new DbListboxModel());

    scene::UIPanel* p_world = new scene::UIPanel();
    menu_->addElement(std::unique_ptr<scene::UIElement>(p_world));
    panels_[MWorld] = p_world;
    ui_elements_[Id("p_world")] = p_world;
    p_world->setVisible(false);

    ivec2 wnd_size(window_.getSize());
    onContextResized_(wnd_size.x, wnd_size.y);  */
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

        //if (menu_->isVisible())
        //{
        //    frame_time_label_->setText(std::to_string(1000.0f * last_frame_time) + " ms");
        //}

        scene_.ui.draw();

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
    scene_.ui.panel.setDimensions(vec2(width, height));


    // Place/scale menu panel correctly
    const ivec2 min_menu_size(760, 400);
    int menu_scale = std::max(1, std::min(width / min_menu_size.x, height / min_menu_size.y));
    vec2 menu_offset = vec2(I32(0.5 * (width - menu_scale * min_menu_size.x)),
                            I32(0.5 * (height - menu_scale * min_menu_size.y)));
    if (menu_offset.y > menu_offset.x)
        menu_offset.y = menu_offset.x;


}

} // namespace pbj
