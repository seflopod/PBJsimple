////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\include\pbj\game.h
///
/// \brief	Declares the game class.
////////////////////////////////////////////////////////////////////////////////
#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <thread>

#include "pbj/_pbj.h"
#include "pbj/engine.h"
#include "pbj/scene/scene.h"
#include "pbj/scene/entity.h"
#include "pbj/input_controller.h"

using pbj::Engine;
using pbj::Window;
using pbj::InputController;

namespace pbj
{
	////////////////////////////////////////////////////////////////////////////
	/// \class		Game
	///
	/// \brief		Game.
	///
	/// \author		Peter Bartosch
	/// \date		2013-08-05
	/// \details	The Game class manages and runs the game.
	////////////////////////////////////////////////////////////////////////////
	class Game
	{
	public:
		static Game* instance();
		static void destroyInstance();

		~Game();

		bool init(U32);
		I32 run();
		void stop();
		
	private:
		static Game* _instance;

		Game();
		
		void initTestScene();
		bool update();
		bool physUpdate();
		
		void draw();

		void onContextResized(I32, I32);

		//Enginey stuff
		F32 _dt;
		F32 _physDt;
		bool _running;
		Engine& _engine;
		Window& _window;
		b2World* _world;

		//this should be a container for multiple scenes.  Right now only one.
		pbj::scene::Scene _scene;
	};

	
} // namespace pbj

#endif