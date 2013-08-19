///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/engine.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::Engine functions.

#include "pbj/engine.h"
#include "pbj/_gl.h"
#include "pbj/sw/sandwich_open.h"
#include "pbj/input_controller.h"

#include <cassert>
#include <iostream>

namespace pbj {
namespace {

Engine* process_engine_ = nullptr;

// GLFW error handler
void glfwError(int error, const char* description)
{
    PBJ_LOG(VError) << "GLFW Error!" << PBJ_LOG_NL
                    << "       Code: " << error << PBJ_LOG_NL
                    << "Description: " << description << PBJ_LOG_END;
}

} // namespace be::(anon)

///////////////////////////////////////////////////////////////////////////////
/// \brief  Engine constructor.
///
/// \details Only one Engine may be constructed.  It should be created as a
///         local variable in main().
Engine::Engine()
{
    if (process_engine_)
        throw std::runtime_error("Engine already initialized!");

    process_engine_ = this;

    glfwSetErrorCallback(glfwError);

    if (!glfwInit())
        PBJ_LOG(VError) << "GLFW could not be initialized!" << PBJ_LOG_END;

    sw::readDirectory("./");

    std::shared_ptr<sw::Sandwich> config_sandwich;
    config_sandwich = sw::open(Id("__pbjconfig__"));

    Id window_settings_id;
    std::string window_title;

#ifdef PBJ_EDITOR
    window_settings_id = Id("__editor__");
    window_title = "PBJ Editor";
#else
    window_settings_id = Id("__game__");
    window_title = "PBJ";
#endif    

    WindowSettings window_settings;

    if (config_sandwich)
        window_settings = loadWindowSettings(*config_sandwich, window_settings_id);

    Window* wnd = new Window(window_settings);
    window_.reset(wnd);

    wnd->setTitle(window_title);
    
    wnd->registerContextResizeListener(
        [](I32 width, I32 height)
        {
            glViewport(0, 0, width, height);
        }
    );

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    PBJ_LOG(VInfo) << glGetString(GL_VERSION) << PBJ_LOG_END;

    InputController::init(wnd->getGlfwHandle());

    //no harm in showing a window before physics is started	
	//For testing in a world with no gravity use 0.0f, 0.0f
	world_ = std::unique_ptr<b2World>(new b2World(b2Vec2(0.0f, -9.822f)));
	world_->SetAllowSleeping(true);

    wnd->show();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destructor.
Engine::~Engine()
{
    pbj::InputController::destroy();

    window_.reset();
	world_.reset();
    glfwTerminate();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the engine's window object.
///
/// \return The Window object.
Window* Engine::getWindow() const
{
    return window_.get();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn b2World* Engine::getWorld() const
///
/// \brief Gets the physics world.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return A pointer to the b2World for Box2D simulations.
////////////////////////////////////////////////////////////////////////////////
b2World* Engine::getWorld() const
{
	return world_.get();
}

////////////////////////////////////////////////////////////////////////////////
sw::ResourceManager& Engine::getResourceManager()
{
    return resource_mgr_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the engine object.
///
/// \details If no engine object exists, this will result in an assertion
///         failure.
///
/// \return The process' engine object.
Engine& getEngine()
{
    assert(process_engine_);
    return *process_engine_;
}

} // namespace pbj
