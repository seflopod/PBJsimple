////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJsimple\include\pbj\game.h
///
/// \brief	Declares the game class.
////////////////////////////////////////////////////////////////////////////////
#ifndef GAME_H_
#define GAME_H_

#include <memory>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <thread>
#include <queue>
#include <map>
#include <Box2D/Dynamics/b2WorldCallbacks.h>

#include "pbj/_pbj.h"
#include "pbj/engine.h"
#include "pbj/scene/scene.h"
#include "pbj/scene/entity.h"
#include "pbj/gfx/material.h"
#include "pbj/input_controller.h"

using std::queue;
using std::unique_ptr;
using pbj::Engine;
using pbj::Window;
using pbj::InputController;
using pbj::scene::Entity;
using pbj::gfx::Material;

namespace pbj
{
	////////////////////////////////////////////////////////////////////////////
	/// \struct	PhysicsSettings
	///
	/// \brief	Physics settings used for the calculations of the Box2D physics
	/// 		engine.
	///
	/// \author	Peter Bartosch
	/// \date	2013-08-13
	////////////////////////////////////////////////////////////////////////////
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
    /// \struct	GameControls
    ///
    /// \brief	Game controls.
    ///
    /// \author	Peter Bartosch
    /// \date	2013-08-13
    /// \details Rather than hardcode the inputs it made sense to create a
    /// 		 struct that could be customized.
    ////////////////////////////////////////////////////////////////////////////
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
		
		void spawnBullet(const vec2&, const vec2&);
		void disableBullet(Entity*);
		void respawnPlayer(Entity*);

	private:
		static unique_ptr<Game> _instance;

		Game();
		
		void initTestScene();
		void initBasicMaterials();

		bool update();
        bool physUpdate();

		void draw();

		void onContextResized(I32, I32);
		void onKeyboard(I32, I32, I32, I32);
		void onMouseLeftDown(I32);
		void checkMovement(I32, I32);
		virtual void BeginContact(b2Contact*);
        virtual void EndContact(b2Contact*);
        virtual void PreSolve(b2Contact*, const b2Manifold*);
        virtual void PostSolve(b2Contact*, const b2ContactImpulse*);

		Entity* makeBullet();
		Entity* makePlayer(F32, F32, bool);
		Entity* makeTerrain(F32, F32, F32, F32);
		Entity* makeSpawnPoint(F32, F32);

		F32 _dt;
		bool _running;
		Engine& _engine;
		Window& _window;
		b2World* _world;
		PhysicsSettings _physSettings;
		queue<Entity*> _toDisable;
		queue<Entity*> _toRespawn;
		GameControls _controls;
		U32 _bulletRing[100];
		I32 _curRingIdx;

		//this should be a container for multiple scenes.  Right now only one.
		pbj::scene::Scene _scene;
	};

	
} // namespace pbj

#endif