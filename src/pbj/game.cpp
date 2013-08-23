////////////////////////////////////////////////////////////////////////////////
/// \file    C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
///         PBJsimple\src\pbj\game.cpp
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

using pbj::scene::Entity;

namespace pbj {

#pragma region statics
/// \brief     The client instance pointer.
unique_ptr<Game> Game::_instance;


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
    if(_instance.get() == 0) //no instance yet
    {
        PBJ_LOG(VError) << "No game object available!" << PBJ_LOG_END;
        assert(false);
        //throw std::runtime_error("No game object available!");
    }
    return _instance.get();
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
     _running(false),
     _paused(false),
     _engine(getEngine()),
     _window(*getEngine().getWindow())
{
    const U32 fps = 30;

    _instance.reset(this);

    _dt = 1.0f/fps;
    _window.registerContextResizeListener([=](I32 width, I32 height) {
            _instance->onContextResized(width, height);
        });

    //Register for input event handling
    InputController::registerKeyAllListener(std::bind(onKeyboard, this));
        /*[&](I32 keycode, I32 scancode, I32 action,I32 modifiers) {
            onKeyboard(keycode, scancode, action, modifiers);
        });*/

    InputController::registerMouseLeftDownListener(std::bind(onMouseLeftDown, this));
        /*[&](I32 mods) {
            onMouseLeftDown(mods);
        });*/

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    std::vector<Id> sws = sw::getSandwichIds();
    for (Id id : sws)
    {
        getSceneIds(id);
    }


    /*
    //make all the bullets we'll ever need
    for(I32 i=0;i<100;++i)
    {
        _bulletRing[i] = _scene.addEntity(unique_ptr<Entity>(makeBullet()));
        _scene.getBullet(_bulletRing[i])->getRigidbody()->setBullet(false);
        _scene.getBullet(_bulletRing[i])->disable();
    }

    _curRingIdx = 0;
    _running = true;
    _bulletNum = 0;
    */
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
    stop();
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
            _scene_ids.push_back(sw::ResourceId(sw_id, Id(stmt.getUInt64(0))));
        }
    }
}

sw::ResourceId Game::getRandomSceneId() const
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

    //add the local player to the scene
    vec2 spawnLoc = _scene->getRandomSpawnPoint()->getTransform().getPosition();
    U32 player_id = _scene->addEntity(unique_ptr<Entity>(makePlayer(be::Id("Player"), spawnLoc.x, spawnLoc.y, false)));
    _scene.setLocalPlayer(id);
    _scene.getLocalPlayer()->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("player1_outline"))));

    //since the player will be the focus of the camera, reduce the volume of its output
    _scene.getLocalPlayer()->getAudioSource()->setGain(0.65f);

    //add other player Entities
    I32 ids[4];
    for(I32 i=0;i<4;++i)
    {
        spawnLoc = _scene.getRandomSpawnPoint()->getTransform().getPosition();
        char name[9] = "CPU ";
        if(i<10)
            name[4] = (char)(49+i);
        else //I'm assuming that there will never be more than 19 ai players
        {
            name[4] = '1';
            name[5] = (char)(49+i-10);
        }
        name[8] = '\0';
        ids[i] = _scene.addEntity(unique_ptr<Entity>(makePlayer(be::Id(std::string(name)),spawnLoc.x, spawnLoc.y, true)));
    }
    _scene.getPlayer(ids[0])->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("player2_outline"))));
    _scene.getPlayer(ids[1])->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("player3_outline"))));
    _scene.getPlayer(ids[2])->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("player4_outline"))));
    _scene.getPlayer(ids[3])->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("player5_outline"))));

    //add the camera
    Entity* e = new Entity();
    e->setType(Entity::EntityType::Camera);
    e->addAudioListener();
    e->addCamera();

    //now that the camera is made, do matrix setup
    ivec2 ctxtSize = _window.getContextSize();
    F32 ratio = ctxtSize.x/(F32)ctxtSize.y;
    glViewport(0, 0, ctxtSize.x, ctxtSize.y);
    glClear(GL_COLOR_BUFFER_BIT);
    mat4 ortho = glm::ortho(-ratio*grid_height/2.0f, ratio*grid_height/2.0f,
                            -grid_height/2.0f, grid_height/2.0f,
                            0.1f, -0.1f);
    e->getCamera()->setProjection(ortho);

    //and finally add the camera to the scene and mark it as the current camera
    id = _scene.addEntity(unique_ptr<Entity>(e));
    _scene.setCurrentCamera(id);

    //add the UI to the scene.
    _scene.initUI();

    _scene.getWorld()->SetContactListener(this);
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
    F64 lastFrameTime = 0.0;
    F64 fps = 0.0;
    F32 nonPhysTimer = 0.0f;

    //start main loop
    while(_running)
    {
        F64 frameStart = glfwGetTime();

        glfwPollEvents();

        if(_paused)
            continue;

        //since physics is in Scene, run that loop first
        _scene->physUpdate(_physSettings.dt, _physSettings.velocityIterations,
        _physSettings.positionIterations);

        if(nonPhysTimer >= _dt)
        {   //physics should execute more than drawing and other loops, so
            //only do those after a certain amount of time.
            update();
            draw();
            nonPhysTimer-=_dt;
        }

        F64 frameTime = lastFrameTime = glfwGetTime() - frameStart;

        if(lastFrameTime < _physSettings.dt)
        {   //make sure all frames last at least as long as the dt for phsyics
            std::this_thread::sleep_for(std::chrono::microseconds(I32(
            1000000 * (_physSettings.dt - lastFrameTime))));
            frameTime = glfwGetTime() - frameStart;
        }
        nonPhysTimer += _physSettings.dt;

        fps = 1.0/frameTime;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn     void Game::stop()
///
/// \brief     Stops this object.
///
/// \author     Peter Bartosch
/// \date     2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Game::stop()
{
    _running = false;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn     bool Game::update()
///
/// \brief     Updates for the game.
///
/// \author     Peter Bartosch
/// \date     2013-08-05
///
////////////////////////////////////////////////////////////////////////////////
void Game::update()
{
    _scene->update(_dt);

    //check for any respawns that need to be done
    if(!_toRespawn.empty())
    {
        F64 t = glfwGetTime();

        //if the front of the queue isn't ready to respawn, we can assume that
        //nothing behind it is ready.
        while(!_toRespawn.empty() &&
                _toRespawn.front()->getPlayerComponent()->getTimeOfDeath()+2<=t)
        {   // five second delay to respawn
            vec2 spwn = _scene->getRandomSpawnPoint()->getTransform().getPosition();
            _toRespawn.front()->enable();
            _toRespawn.front()->getTransform().setPosition(spwn.x, spwn.y);
            _toRespawn.front()->getTransform().updateOwnerRigidbody();
            _toRespawn.front()->getRigidbody()->setVelocity(vec2(0.0f,0.0f));
            _toRespawn.front()->getPlayerComponent()->setHealth(
            _toRespawn.front()->getPlayerComponent()->getMaxHealth());
            _toRespawn.front()->getPlayerComponent()->setAmmoRemaining(
            _toRespawn.front()->getPlayerComponent()->getMaxAmmo());
            _toRespawn.pop();
        }
    }

    if(_window.isClosePending() && _running)
        _running = false;
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

    //error pump
    GLenum glError;
    while((glError = glGetError()) != GL_NO_ERROR)
    {
        PBJ_LOG(VWarning) << "OpenGL error while rendering" << PBJ_LOG_NL
                          << "Error code: " << glError << PBJ_LOG_NL
                          << "Error:        " << getGlErrorString(glError)
                          << PBJ_LOG_END;
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
        _instance->_running = !(keycode == GLFW_KEY_ESCAPE);

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
                _scene.getLocalPlayer()->getPlayerComponent()->endThrust();
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
    vec2 worldPos = _scene.getCurrentCamera()->getWorldPosition(screenCoords, size);

    //mouse left-click means fire for the main player.
    _scene.getLocalPlayer()->getPlayerComponent()->fire(worldPos.x, worldPos.y);
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
        _scene.getLocalPlayer()->getPlayerComponent()->moveLeft();
    }
    else if(keycode == _controls.right[0] || keycode == _controls.right[1])
    {
        _scene.getLocalPlayer()->getPlayerComponent()->moveRight();
    }
    else if(keycode == _controls.up[0] || keycode == _controls.up[1])
    {
        _scene.getLocalPlayer()->getPlayerComponent()->jump();
    }
    else if(keycode == _controls.down[0] || keycode == _controls.down[1])
    {
        _scene.getLocalPlayer()->getPlayerComponent()->stop();
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
        _scene.getLocalPlayer()->getPlayerComponent()->doThrust();
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
scene::Scene* Game::getScene() { return _scene.get(); }




} // namespace pbj
