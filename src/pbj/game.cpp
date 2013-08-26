////////////////////////////////////////////////////////////////////////////////
/// \file   pbj\game.cpp
///
/// \brief    Implements the game class.
////////////////////////////////////////////////////////////////////////////////
#ifndef GAME_H_
#include "pbj/game.h"
#endif

#include <stdio.h>
#include "pbj/scene/ui_label.h"
#include "pbj/scene/player_component.h"
#include "pbj/sw/sandwich_open.h"

namespace pbj {

#pragma region statics
/// \brief     The client instance pointer.
Game* Game::_instance;

////////////////////////////////////////////////////////////////////////////////
/// \fn Game* Game::instance()
///
/// \brief Returns a pointer to the single instance of Game.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return A pointer to a Game instance.  If \c _instance is null when this
///            is called, a new instance of Game is created and a pointer to that
///            is returned.
////////////////////////////////////////////////////////////////////////////////
Game* Game::instance()
{
    if(_instance == 0) //no instance yet
    {
        PBJ_LOG(VError) << "No game object available!" << PBJ_LOG_END;
        assert(false);
        //throw std::runtime_error("No game object available!");
    }
    return _instance;
}
#pragma endregion

#pragma region ctor_dtor
////////////////////////////////////////////////////////////////////////////////
/// \fn     Game::Game()
///
/// \brief     Default constructor.
///
/// \author     Peter Bartosch
/// \date     2013-08-05
////////////////////////////////////////////////////////////////////////////////
Game::Game()
    : _prng(std::mt19937::result_type(time(nullptr))),
     _paused(false),
     _engine(getEngine()),
     _window(*getEngine().getWindow())
{
    const U32 fps = 30;

    _instance = this;

    _dt = 1.0f/fps;
    _window.registerContextResizeListener([=](I32 width, I32 height) {
            _instance->onContextResized(width, height);
        });

    //Register for input event handling
    InputController::registerKeyAllListener(
        [&](I32 keycode, I32 scancode, I32 action,I32 modifiers) {
            onKeyboard(keycode, scancode, action, modifiers);
        });

    InputController::registerMouseLeftDownListener(
        [&](I32 mods) {
            onMouseLeftDown(mods);
        });

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    std::vector<Id> sws = sw::getSandwichIds();
    for (Id id : sws)
    {
        PBJ_LOG(VInfo) << "Sandwich: " << id << PBJ_LOG_END;
        getSceneIds(id);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn     Game::~Game()
///
/// \brief     Destructor.
///
/// \author     Peter Bartosch
/// \date     2013-08-05
////////////////////////////////////////////////////////////////////////////////
Game::~Game()
{
}
#pragma endregion


void Game::getSceneIds(const Id& sw_id)
{
    std::shared_ptr<sw::Sandwich> ptr = sw::open(sw_id);

    if (ptr)
    {
        db::CachedStmt& stmt = ptr->getStmtCache().hold(Id("SELECT id FROM sw_maps"), "SELECT id FROM sw_maps");

        while (stmt.step())
        {
            PBJ_LOG(VInfo) << "Found Map: " << sw::ResourceId(sw_id, Id(stmt.getUInt64(0))) << PBJ_LOG_END;
            _scene_ids.push_back(sw::ResourceId(sw_id, Id(stmt.getUInt64(0))));
        }
    }
}

sw::ResourceId Game::getRandomSceneId()
{
    std::uniform_int_distribution<int> dist(0, _scene_ids.size() - 1);
    return _scene_ids[dist(_prng)];
}

void Game::loadScene(const sw::ResourceId& scene_id)
{
    auto ptr = sw::open(scene_id.sandwich);
    if (!ptr)
        return;

    _scene = scene::loadScene(*ptr, scene_id.resource);

    PBJ_LOG(VInfo) << "Loading Scene: " << scene_id << ": " << (_scene ? "success" : "failed"
        ) << PBJ_LOG_END;

    //add the local player to the scene
    vec2 spawnLoc = _scene->getRandomSpawnPoint()->getTransform().getPosition();
    U32 player_id = _scene->makePlayer("Player", spawnLoc, true);
    _scene->getLocalPlayer()->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("player1_outline"))));

    //since the player will be the focus of the camera, reduce the volume of its output
    _scene->getLocalPlayer()->getAudioSource()->setGain(0.65f);

    //add other player Entities
    U32 ids[4];
    for (U32 i=0; i < 4; ++i)
    {
        spawnLoc = _scene->getRandomSpawnPoint()->getTransform().getPosition();
        std::string name = "CPU ";
        name.append(std::to_string(i));
        ids[i] = _scene->makePlayer(std::string(name), spawnLoc, false);
    }
    _scene->getPlayer(ids[0])->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("player2_outline"))));
    _scene->getPlayer(ids[1])->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("player3_outline"))));
    _scene->getPlayer(ids[2])->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("player4_outline"))));
    _scene->getPlayer(ids[3])->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("player5_outline"))));

    //add the camera
    U32 camera_id = _scene->makeCamera();
    _scene->setCurrentCamera(camera_id);

    


    //now that the camera is made, do matrix setup
    ivec2 wnd_size = _window.getContextSize();
    glViewport(0, 0, wnd_size.x, wnd_size.y);
    F32 ratio = wnd_size.x / F32(wnd_size.y);
    F32 hheight = 25.0f;
    F32 hwidth = ratio * hheight;
    _scene->getCurrentCamera()->getCamera()->setProjection(glm::ortho(-hwidth, hwidth, -hheight, hheight));
    
    //add the UI to the scene.
    _scene->makeHud();
}

#pragma region run_game
////////////////////////////////////////////////////////////////////////////////
/// \fn     I32 Game::run()
///
/// \brief     Runs the main loop for the game.
///
/// \author     Peter Bartosch
/// \date     2013-08-05
///
/// \return     The exit code for the client.
////////////////////////////////////////////////////////////////////////////////
I32 Game::run()
{
    F64 last_frame_start = -1.0;
    F64 last_frame_time = -1.0;
    F64 fps = -1.0;

    while (true)
    {
        glfwPollEvents();

        if (_window.isClosePending())
            break;

        F64 frame_start = glfwGetTime();
        F64 delta_t = frame_start - last_frame_start;
        fps = 1.0 / delta_t;

        if (_scene)
        {
            if (last_frame_start >= 0 && !_paused)
            {
                if (delta_t > 1.0)
                    delta_t = PBJ_GAME_MIN_PHYSICS_TIMESTEP;

                if (delta_t > PBJ_GAME_MIN_PHYSICS_TIMESTEP)
                {
                    I32 steps = I32(std::ceil(delta_t / F64(PBJ_GAME_MIN_PHYSICS_TIMESTEP)));
                    F64 interval = delta_t / steps;

                    while (--steps >= 0)
                    {
                        _scene->physUpdate(F32(interval));
                    }
                }
                else
                {
                    _scene->physUpdate(F32(delta_t));
                }

                _scene->update(F32(delta_t));
            }

            draw();
        }

        last_frame_start = frame_start;
        F64 frame_time = last_frame_time = glfwGetTime() - frame_start;
        if (last_frame_time < (1.0 / PBJ_GAME_MAX_FPS))
        {
            std::this_thread::sleep_for(std::chrono::microseconds(
                int(1000000 * ((1.0 / PBJ_GAME_MAX_FPS) - last_frame_time))));
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Game::draw()
///
/// \brief Draws the game by drawing the scene.
///
/// \author Peter Bartosch
/// \date 2013-08-08
////////////////////////////////////////////////////////////////////////////////
void Game::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (_scene)
        _scene->draw();

    GLenum gl_error;
    while ((gl_error = glGetError()) != GL_NO_ERROR)
    {
        PBJ_LOG(VWarning) << "OpenGL error while rendering frame!" << PBJ_LOG_NL
                                << "Error Code: " << gl_error << PBJ_LOG_NL
                                << "     Error: " << getGlErrorString(gl_error) << PBJ_LOG_END;
    }

    glfwSwapBuffers(_window.getGlfwHandle());
}
#pragma endregion

#pragma region event_handling
////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::onContextResized(I32 width, I32 height)
///
/// \brief    Deals with window resizing.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    width     The width.
/// \param    height    The height.
////////////////////////////////////////////////////////////////////////////////
void Game::onContextResized(I32 width, I32 height)
{
    GLdouble ratio = width/(GLdouble)height;
    glViewport(0, 0, width, height);
}


////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::onKeyboard(I32 keycode, I32 scancode, I32 action,
///     I32 modifiers)
///
/// \brief    Processes keyboard input for the game.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    keycode      The keycode.
/// \param    scancode     The scancode.
/// \param    action        The action (GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT).
/// \param    modifiers    The modifiers for the keyboard action.
////////////////////////////////////////////////////////////////////////////////
void Game::onKeyboard(I32 keycode, I32 scancode, I32 action, I32 modifiers)
{
    if(action == GLFW_PRESS || action == GLFW_REPEAT)
        checkMovement(keycode, action);

    if(action == GLFW_RELEASE)
    {
        if (keycode == GLFW_KEY_R)
            loadScene(getRandomSceneId());

        if (keycode == GLFW_KEY_ESCAPE)
            _window.requestClose();

        if(keycode == GLFW_KEY_H)
        {   //This should show the help menu.  It pauses the game while doing so.
            _paused = true;
            help();
        }
        else if(keycode == _controls.keyJump[0] || keycode == _controls.keyJump[1])
        {
            if(_paused) //unpause if paused
                _paused = false;
            else
                _scene->getLocalPlayer()->getPlayerComponent()->endThrust();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::onMouseLeftDown(I32 mods)
///
/// \brief    Executes the mouse left down action, which in this case is
///           shooting a bullet.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    mods    The modifiers for the mouse click.  These are unused.
////////////////////////////////////////////////////////////////////////////////
void Game::onMouseLeftDown(I32 mods)
{
    F64 x,y;
    glfwGetCursorPos(getEngine().getWindow()->getGlfwHandle(), &x, &y);

    //do conversion for mouse click from screen coordinates to world coordinates
    ivec2 screenCoords = ivec2((I32)std::floor(x), (I32)std::floor(y));
    ivec2 size = getEngine().getWindow()->getContextSize();
    vec2 worldPos = _scene->getCurrentCamera()->getCamera()->getWorldPosition(screenCoords, size);

    //mouse left-click means fire for the main player.
    _scene->getLocalPlayer()->getPlayerComponent()->fire(worldPos.x, worldPos.y);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::checkMovement(I32 keycode, I32 action)
///
/// \brief    Check to see if a keypress should be used for movement.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    keycode    The keycode.
/// \param    action     The action (GLFW_PRESS, GLFW_RELEASE, etc)
////////////////////////////////////////////////////////////////////////////////
void Game::checkMovement(I32 keycode, I32 action)
{
    //this is a bit simplistic (no modifiers are taken into account), but for
    //now it will do
    if(keycode == _controls.left[0] || keycode == _controls.left[1])
    {
        _scene->getLocalPlayer()->getPlayerComponent()->moveLeft();
    }
    else if(keycode == _controls.right[0] || keycode == _controls.right[1])
    {
        _scene->getLocalPlayer()->getPlayerComponent()->moveRight();
    }
    else if(keycode == _controls.up[0] || keycode == _controls.up[1])
    {
        _scene->getLocalPlayer()->getPlayerComponent()->jump();
    }
    else if(keycode == _controls.down[0] || keycode == _controls.down[1])
    {
        _scene->getLocalPlayer()->getPlayerComponent()->stop();
    }
    else if(keycode == _controls.keyFire1[0] || keycode == _controls.keyFire1[1])
    {
        //do nothing
    }
    else if(keycode == _controls.keyFire2[0] || keycode == _controls.keyFire2[1])
    {
        //do nothing
    }
    else if(keycode == _controls.keyJump[0] || keycode == _controls.keyJump[1])
    {
        _scene->getLocalPlayer()->getPlayerComponent()->doThrust();
    }
    else if(keycode == _controls.keyAction[0] || keycode == _controls.keyAction[1])
    {
        //do nothing
    }
}
#pragma endregion
////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::help()
///
/// \brief    Displays the help menu in STDERR.
///
/// \author    Josh Douglas
/// \date    2013-08-13
////////////////////////////////////////////////////////////////////////////////
void Game::help()
{
    std::cerr << std::endl << std::endl;
    std::cerr << "HELP MENU" << std::endl;
    std::cerr << std::endl << std::endl;
    std::cerr << "--Controls--" << std::endl;
    std::cerr << std::endl;
    std::cerr << "w - Press w to activate thrusters" << std::endl;
    std::cerr << "a - Press a to move left" << std::endl;
    std::cerr << "s - Press s to quickly stop and accelerate down" << std::endl;
    std::cerr << "d - Press d to move right" << std::endl;
    std::cerr << "space - jump" << std::endl;
    std::cerr << "Mouse left click - fire bullet" << std::endl;
    std::cerr << "h - Press h to display help menu" << std::endl;
    std::cerr << "ESC - Press ESC to exit the game" << std::endl;
    std::cerr << std::endl << std::endl;
    std::cerr << "Move the mouse to change the fire direction" << std::endl;
    std::cerr << "Press the left mouse button to fire the weapon" << std::endl
                << std::endl;
    std::cerr << "Press space to continue" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn scene::Scene* Game::getScene()
///
/// \brief  Gets the current scene.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return A pointer to the current scene.
////////////////////////////////////////////////////////////////////////////////
scene::Scene* Game::getScene()
{
    return _scene.get();
}

} // namespace pbj
