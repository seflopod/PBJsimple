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
#include <queue>
#include <map>
//#include <b2WorldCallbacks.h>

#include "pbj/_pbj.h"
#include "pbj/engine.h"
#include "pbj/scene/scene.h"
#include "pbj/scene/entity.h"
#include "pbj/gfx/material.h"
#include "pbj/input_controller.h"

using std::queue;
using pbj::Engine;
using pbj::Window;
using pbj::InputController;
using pbj::scene::Entity;
using pbj::gfx::Material;

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
        I32 left[2];
        I32 right[2];
        I32 up[2];
        I32 down[2];
        I32 keyFire1[2];
        I32 keyFire2[2];
        I32 keyJump[2];
        I32 keyAction[2];
        I32 mouseFire1;
        I32 mouseFire2;
        I32 mouseJump;
        I32 mouseAction;
        
        GameControls()
        {
            left[0] = GLFW_KEY_A;
            left[1] = GLFW_KEY_LEFT;

            right[0] = GLFW_KEY_D;
            right[1] = GLFW_KEY_RIGHT;

            up[0] = GLFW_KEY_W;
            up[1] = GLFW_KEY_UP;

            down[0] = GLFW_KEY_S;
            down[1] = GLFW_KEY_DOWN;
            keyFire1[0] = GLFW_KEY_LEFT_CONTROL;
            keyFire1[1] = GLFW_KEY_Z;

            keyFire2[0] = GLFW_KEY_LEFT_ALT;
            keyFire2[1] = GLFW_KEY_X;

            keyJump[0] = GLFW_KEY_SPACE;
            keyJump[1] = GLFW_KEY_LEFT_SHIFT;

            keyAction[0] = GLFW_KEY_E;
            keyAction[1] = GLFW_KEY_ENTER;

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
	class Game : public b2ContactListener
	{
	public:
		static const int grid_height = 50;

		static Game* instance();
		static void destroyInstance();

		~Game();
		void help();

		bool init(U32);
		I32 run();
		void stop();
		
		void spawnBullet(vec2&, vec2&);
	protected:
        
    
	private:
		typedef queue<std::unique_ptr<Entity>> BulletQueue;
		typedef std::map<std::string,std::shared_ptr<Material>> MaterialMap;

		static Game* _instance;

		Game();
		
		bool update();
        bool physUpdate();

		void draw();

		void onContextResized(I32, I32);

		void onKeyboard(I32, I32, I32, I32);
		void onMouseLeftDown(I32);
		void checkMovement(I32, I32);
		void initTestScene();
		void initBasicMaterials();

		virtual void BeginContact(b2Contact*);
        virtual void EndContact(b2Contact*);
        virtual void PreSolve(b2Contact*, const b2Manifold*);
        virtual void PostSolve(b2Contact*, const b2ContactImpulse*);

		Entity* makeBullet();
		Entity* makePlayer();
		Entity* makeTerrain();

		//Enginey stuff
		F32 _dt;
		bool _running;
		Engine& _engine;
		Window& _window;
		b2World* _world;
		PhysicsSettings _physSettings;

		GameControls _controls;
		BulletQueue _bullets;
		MaterialMap _materials;

		//this should be a container for multiple scenes.  Right now only one.
		pbj::scene::Scene _scene;
	};

	
} // namespace pbj

#endif