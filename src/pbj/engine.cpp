///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/engine.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::Engine functions.

#include "pbj/engine.h"
#include "pbj/_gl.h"
#include "pbj/_al.h"
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



////////////////////////////////////////////////////////////////////////////////
/// \fn Engine::Engine(int* argc, char** argv)
///
/// \brief  Alternate constructor that takes command line arguments for
///         initializing alut.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param [in,out] argc    If non-null, the argc.
/// \param [in,out] argv    If non-null, the argv.
////////////////////////////////////////////////////////////////////////////////
Engine::Engine(int* argc, char** argv)
{
    if (process_engine_)
        throw std::runtime_error("Engine already initialized!");

    process_engine_ = this;

    //init audio
    alutInit(argc, argv);

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

    wnd->show();
}
///////////////////////////////////////////////////////////////////////////////
/// \brief  Destructor.
Engine::~Engine()
{
    pbj::InputController::destroy();

    window_.reset();
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
/// \fn sw::ResourceManager& Engine::getResourceManager()
///
/// \brief  Gets resource manager.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \return The resource manager.
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


