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
#include "be/id.h"

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
	class Game
	{
	public:
		static const int grid_height = 33;

		static Game* instance();

        Game();
		~Game();

		void help();

		I32 run();
		void stop();
		
		void spawnBullet(const vec2&, const vec2&, void*);
		void disableBullet(Entity*);
		void respawnPlayer(Entity*);

		scene::Scene& currentScene();

        sw::ResourceId getRandomSceneId() const;
        void loadScene(const sw::ResourceId& scene_id);
		
	private:
        void update();
		void draw();

        void getSceneIds(const Id& sw_id);

        void onContextResized(I32, I32);
		void onKeyboard(I32, I32, I32, I32);
		void onMouseLeftDown(I32);
		void checkMovement(I32, I32);

        Entity* makeBullet();
		Entity* makePlayer(be::Id, F32, F32, bool);


        static unique_ptr<Game> _instance;

		Engine& _engine;
		Window& _window;

		std::mt19937 _prng;

        std::vector<sw::ResourceId> _scene_ids;
        std::unique_ptr<scene::Scene> _scene;

        F32 _dt;
		bool _running;
		bool _paused;
		
		GameControls _controls;
	};

	
} // namespace pbj

#endif