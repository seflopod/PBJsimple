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
	
    struct GameControls
    {
        U8 left[2];
        U8 right[2];
        U8 up[2];
        U8 down[2];
        U8 keyFire1[2];
        U8 keyFire2[2];
        U8 keyJump[2];
        U8 keyAction[2];
        I32 mouseFire1;
        I32 mouseFire2;
        I32 mouseJump;
        I32 mouseAction;
        
        GameControls()
        {
            left = { GLFW_KEY_A, GLFW_KEY_LEFT };
            right = { GLFW_KEY_D, GLFW_KEY_RIGHT };
            up = { GLFW_KEY_W, GLFW_KEY_UP };
            down = { GLFW_KEY_S, GLFW_KEY_DOWN };
            keyFire1 = { GLFW_KEY_LEFT_CONTROL, GLFW_KEY_Z };
            keyFire2 = { GLFW_KEY_LEFT_ALT, GLFW_KEY_X };
            keyJump = { GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT };
            keyAction = { GLFW_KEY_E, GLFW_KEY_ENTER };
            mouseFire1 = GLFW_MOUSE_BUTTON_LEFT;
            mouseFire2 = GLFW_MOUSE_BUTTON_RIGHT;
            mouseJump = -1; //meaning undefined, I hope
            mouseAction = GLFW_MOUSE_BUTTON_MIDDLE;
        }
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