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
//#include <b2WorldCallbacks.h>

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
	struct PhysicsSettings
	{
		PhysicsSettings():
			dt(1.0f/60.0f),
			velocityIterations(8),
			positionIterations(3)
		{}
		
		F32 dt;
		I32 velocityIterations;
		I32 positionIterations;
	};
	
	////////////////////////////////////////////////////////////////////////////
	/// \class		Game
	///
	/// \brief		Game.
	///
	/// \author		Peter Bartosch
	/// \date		2013-08-05
	/// \details	The Game class manages and runs the game.  It inherits from
    ///             b2ContactListener so that it can capture collision events.
	////////////////////////////////////////////////////////////////////////////
	class Game : b2ContactListener
	{
	public:
		static Game* instance();
		static void destroyInstance();

		~Game();

		bool init(U32);
		I32 run();
		void stop();
        
	protected:
        virtual void BeginContact(b2Contact*);
        virtual void EndContact(b2Contact*);
        //virtual void PreSolve(b2Contact*, cosnt b2Manifold*);
        //virtual void PostSolve(b2Contact*, const b2ContactImpulse*);
    
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
		bool _running;
		Engine& _engine;
		Window& _window;
		b2World* _world;
		PhysicsSettings _physSettings;

		//this should be a container for multiple scenes.  Right now only one.
		pbj::scene::Scene _scene;
	};

	
} // namespace pbj

#endif