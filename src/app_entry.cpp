///////////////////////////////////////////////////////////////////////////////
/// \file   app_entry.cpp
/// \author Benjamin Crist
///
/// \brief  Application entry point.
/// \details Parses command line parameters, initializes game engine, loads
///         configuration data, and starts game.

// Auto-link with libraries
#ifdef _WIN32
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "OpenAL32.lib")
#pragma comment (lib, "alut.lib")
#ifdef DEBUG
#pragma comment (lib, "glew32sd.lib")
#pragma comment (lib, "glfw3sd.lib")
#pragma comment (lib, "Box2D_d.lib")

#else
#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "glfw3s.lib")
#pragma comment (lib, "Box2D.lib")

#endif // DEBUG
#endif // _WIN32

// Is this a unit testing build?
#ifdef PBJ_TEST
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#else

///////////////////////////////////////////////////////////////////////////////
// Workaround for WinMain entry point
#if defined(_WIN32) && !defined(DEBUG)
#include <windows.h>
int main(int argc, char* argv[]);

/// \brief  Win32 API entry point; redirects to main().
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
    return main(__argc, __argv);
}

#endif // defined(_WIN32) && !defined(DEBUG)

#include "pbj/game.h"
#include "pbj/editor.h"

#include <iostream>
#include <fstream>
#include <random>

///////////////////////////////////////////////////////////////////////////////
/// \brief  Application entry point
/// \details Parses any command line arguments, then initializes game engine.
int main(int argc, char* argv[])
{
#ifdef BE_CRT_BUILD
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#ifdef DEBUG
    int verbosity = pbj::VAll;
#else
    int verbosity = pbj::VErrorsAndWarnings;
#endif
    std::ofstream cerr_log_file;
    std::string cerr_log("pbjgame.log");

    // Set the appropriate verbosity level
    be::setVerbosity(verbosity);

    // Redirect PBJ_LOG_STREAM to a log file if not in DEBUG mode
#ifndef DEBUG
    if (cerr_log.length() > 0)
    {
        PBJ_LOG(pbj::VNotice) << "Redirecting log to " << cerr_log << PBJ_LOG_END;
        cerr_log_file.open(cerr_log, std::ofstream::trunc);
        PBJ_LOG_STREAM.rdbuf(cerr_log_file.rdbuf());
    }
#endif

    std::cout << "PBJgame " << PBJ_VERSION_MAJOR << '.' << PBJ_VERSION_MINOR << " (" << __DATE__ " " __TIME__ << ')' << std::endl
              << PBJ_COPYRIGHT << std::endl;


    // Initialize game engine
    pbj::Engine e(&argc, argv);

    ///////////////////////////////////////////////////////////////////////////
    // If this is the editor, create an Editor object.
#ifdef PBJ_EDITOR

    if (argc < 3)
    {
        PBJ_LOG(pbj::VError) << "No map specified to edit!" << PBJ_LOG_END;

        PBJ_LOG_STREAM << PBJ_LOG_END << "Usage:" << PBJ_LOG_END
                       << "   editor <sandwich id> <map id>" << PBJ_LOG_END << PBJ_LOG_END;
        return -1;
    }

    pbj::Editor editor;

    editor.run(argv[1], argv[2]);

#else
    ///////////////////////////////////////////////////////////////////////////
    // If this is the game, create a Game object.
    pbj::Game game;
    game.loadScene(game.getRandomSceneId());
    pbj::I32 exitCode = game.run();

    return exitCode;
#endif
};

#endif
