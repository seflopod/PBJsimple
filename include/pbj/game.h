////////////////////////////////////////////////////////////////////////////////
/// \file   C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
///         PBJsimple\include\pbj\game.h
///
/// \brief  Declares the game class.
////////////////////////////////////////////////////////////////////////////////
#ifndef PBJ_GAME_H_
#define PBJ_GAME_H_

#include "pbj/_pbj.h"
#include "pbj/engine.h"
#include "pbj/scene/scene.h"
#include "pbj/scene/entity.h"
#include "pbj/gfx/material.h"
#include "pbj/input_controller.h"
#include "be/id.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <thread>
#include <queue>
#include <map>

#define PBJ_GAME_MAX_FPS 120.0f
#define PBJ_GAME_MIN_PHYSICS_TIMESTEP 0.05

namespace pbj {

////////////////////////////////////////////////////////////////////////////
/// \struct GameControls
///
/// \brief  Game controls.
///
/// \author Peter Bartosch
/// \date   2013-08-13
/// \details Rather than hardcode the inputs it made sense to create a
///          struct that could be customized.
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
/// \class      Game
///
/// \brief      Game.
///
/// \author     Peter Bartosch
/// \date       2013-08-05
/// \details    The Game class manages and runs the game.  It inherits from
///             b2ContactListener so that it can capture collision events.
////////////////////////////////////////////////////////////////////////////
class Game
{
public:
    static Game* instance();

    Game();
    ~Game();

    void help();

    I32 run();

    scene::Scene* getScene();

    sw::ResourceId getRandomSceneId();
    void loadScene(const sw::ResourceId& scene_id);

private:
    void draw();

    void getSceneIds(const Id& sw_id);

    void onContextResized(I32, I32);
    void onKeyboard(I32, I32, I32, I32);
    void onMouseLeftDown(I32);
    void checkMovement(I32, I32);




    static Game* _instance;

    Engine& _engine;
    Window& _window;

    std::mt19937 _prng;

    std::vector<sw::ResourceId> _scene_ids;
    std::unique_ptr<scene::Scene> _scene;

    F32 _dt;
    bool _paused;

    GameControls _controls;
};

} // namespace pbj

#endif
