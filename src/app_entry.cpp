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
#ifdef DEBUG
#pragma comment (lib, "glew32sd.lib")
#pragma comment (lib, "glfw3sd.lib")
#else
#pragma comment (lib, "glew32s.lib")
#pragma comment (lib, "glfw3s.lib")
#endif // DEBUG
#endif // _WIN32

// Is this a unit testing build?
#ifdef PBJ_TEST
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#else

#include "pbj/game.h"

#include <iostream>
#include <fstream>
#include <random>

#if defined(_WIN32) && !defined(DEBUG)
#include <windows.h>
int main(int argc, char* argv[]);

///////////////////////////////////////////////////////////////////////////////
/// \brief  Win32 API entry point; redirects to main().
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
   return main(__argc, __argv);
}

#endif // defined(_WIN32) && !defined(DEBUG)


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

   if (argc != 1)
   {
      std::cout << "PBJgame " << PBJ_VERSION_MAJOR << '.' << PBJ_VERSION_MINOR << " (" << __DATE__ " " __TIME__ << ')' << std::endl
                << PBJ_COPYRIGHT << std::endl;
      return 1;
   }
   
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

   // TODO: start game
};

#endif
