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

using pbj::scene::Entity;

namespace pbj {

#pragma region statics
/// \brief     The client instance pointer.
unique_ptr<Game> Game::_instance = unique_ptr<Game>(nullptr);


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
          _instance = unique_ptr<Game>(new Game());
     return _instance.get();
}

void Game::destroyInstance()
{
     if(_instance != 0)
     {
          _instance->stop();
          _instance.reset();
     }
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
Game::Game() :
     _dt(0.0f),
     _running(false),
     _engine(getEngine()),
     _window(*getEngine().getWindow())
{}

////////////////////////////////////////////////////////////////////////////////
/// \fn     Game::~Game()
///
/// \brief     Destructor.
///
/// \author     Peter Bartosch
/// \date     2013-08-05
////////////////////////////////////////////////////////////////////////////////
Game::~Game()
{}
#pragma endregion

#pragma region init_funcs
bool Game::init(U32 fps)
{
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

     //setup physics, using variables instead of straight numbers so I can
     //remember what does what.
     _world = getEngine().getWorld();
     _world->SetContactListener(this);
     _physSettings = PhysicsSettings();
    
     //remove when making for reals
     initTestScene();

    //seems like an odd place to setup gl matrices, but there we go
    /*ivec2 ctxtSize = _window.getContextSize();
    F32 ratio = ctxtSize.x/(F32)ctxtSize.y;
    glViewport(0, 0, ctxtSize.x, ctxtSize.y);
    glClear(GL_COLOR_BUFFER_BIT);*/
    //_scene.setupCamera(glm::ortho(-ratio*grid_height/2.0f, ratio*grid_height/2.0f, -grid_height/2.0f, grid_height/2.0f, 0.1f, -0.1f));

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
     return true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::initTestScene()
///
/// \brief    Initialises the test scene.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
////////////////////////////////////////////////////////////////////////////////
void Game::initTestScene()
{
    //first define terrain
    _scene.addEntity(unique_ptr<Entity>(makeTerrain(-37.0f, 8.0f, 15.0f, 5.0f)));
	_scene.addEntity(unique_ptr<Entity>(makeTerrain(-15.0f, 0.0f, 25.0f, 5.0f)));
	_scene.addEntity(unique_ptr<Entity>(makeTerrain(-7.5f, 13.75f, 10.0f, 2.5f)));
	_scene.addEntity(unique_ptr<Entity>(makeTerrain(0.0f, -20.0f, 100.0f, 5.0f)));
	_scene.addEntity(unique_ptr<Entity>(makeTerrain(0.0f, 20.0f, 5.0f, 15.0f)));
	_scene.addEntity(unique_ptr<Entity>(makeTerrain(0.0f, 30.0f, 100.0f, 10.0f)));
	_scene.addEntity(unique_ptr<Entity>(makeTerrain(10.0f, -5.0f, 5.0f, 5.0f)));
    _scene.addEntity(unique_ptr<Entity>(makeTerrain(37.0f, 5.0f, 10.0f, 5.0f)));
    
	//second add spawn points to scene
	_scene.addEntity(unique_ptr<Entity>(makeSpawnPoint(-15.0f, 3.5f)));
	_scene.addEntity(unique_ptr<Entity>(makeSpawnPoint(-7.5f, 16.0f)));
	_scene.addEntity(unique_ptr<Entity>(makeSpawnPoint(0.0f, 0.0f)));
	_scene.addEntity(unique_ptr<Entity>(makeSpawnPoint(10.0f, -1.5f)));
	_scene.addEntity(unique_ptr<Entity>(makeSpawnPoint(37.0f, 3.5f)));

    //add the local player to the scene
    vec2 spawnLoc = _scene.getRandomSpawnPoint()->getTransform().getPosition();
    U32 id = _scene.addEntity(unique_ptr<Entity>(makePlayer(be::Id("Player"), spawnLoc.x, spawnLoc.y, false)));
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

    //add some UI to the scene
    //This does not work due to issues with UIRoot and input registration
    /*scene::UILabel label;
    label.setPosition(vec2(0,0));
    _scene.ui.panel.addElement(unique_ptr<scene::UILabel>(&label));
    */
	_scene.initUI();
}
#pragma endregion

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
          
          physUpdate();
          if(nonPhysTimer >= _dt)
          {
                update();
                draw();
                nonPhysTimer-=_dt;
          }

          F64 frameTime = lastFrameTime = glfwGetTime() - frameStart;

          if(lastFrameTime < _physSettings.dt)
          {
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
     //right now not much else that I can think of, but we'll see.
}

////////////////////////////////////////////////////////////////////////////////
/// \fn     bool Game::update()
///
/// \brief     Updates this object.
///
/// \author     Peter Bartosch
/// \date     2013-08-05
///
/// \return     true if the next loop should happen; false otherwise.  This is being
///            changed to a void eventually.
////////////////////////////////////////////////////////////////////////////////
bool Game::update()
{
    _scene.update(_dt);
	
	//check for any respawns that need to be done
	if(!_toRespawn.empty())
	{
		F64 t = glfwGetTime();
		while(!_toRespawn.empty() &&
				_toRespawn.front()->getPlayerComponent()->getTimeOfDeath() + 2 <= t)
		 {  // five second delay to respawn
			vec2 spwn = _scene.getRandomSpawnPoint()->getTransform().getPosition();
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

    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn bool Game::physUpdate()
///
/// \brief Does the update for the physics portion of the game
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return     true if the next loop should happen; false otherwise.  This is being
///            changed to a void eventually.
/// 
/// \details This needs to be separate loop because it is likely that the
///             physics (and thus collision) step is shorter than the normal update
///             and draw step.  This will take care of anything being done that
///             relates to physics.
////////////////////////////////////////////////////////////////////////////////
bool Game::physUpdate()
{
     _world->Step(_physSettings.dt, _physSettings.velocityIterations,
                          _physSettings.positionIterations);
     
     //after all other physics updates, clear forces
     _world->ClearForces();

    //after we're done with the physics step, we need to disable anything queued
    //for it
    while(!_toDisable.empty())
    {
        _toDisable.front()->disable();
        _toDisable.pop();
    }
     return true;
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

     _scene.draw();

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
/// \brief    Executes the context resized action.
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
/// \fn    void Game::BeginContact(b2Contact* contact)
///
/// \brief    Begins a contact.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param [in,out]    contact    If non-null, the contact.
////////////////////////////////////////////////////////////////////////////////
void Game::BeginContact(b2Contact* contact)
{
    //handle collisions for the entire game here
    Entity* a = (Entity*)(contact->GetFixtureA()->GetBody()->GetUserData());
    Entity* b = (Entity*)(contact->GetFixtureB()->GetBody()->GetUserData());
    if(!a || !b)
    {
        PBJ_LOG(pbj::VError) << "Collision between untracked rigidbodies. "<< PBJ_LOG_END;
        return;
    }

	if(b->getType() == Entity::EntityType::Player && a->getType() != Entity::EntityType::Player)
		std::swap(a, b);

    if(a->getType() == Entity::EntityType::Player)
    {
		scene::PlayerComponent* p = a->getPlayerComponent();
		switch(b->getType())
		{
		case Entity::EntityType::Terrain:
			p->enableJump();
			break;
		case Entity::EntityType::Bullet:
		{
			scene::PlayerComponent* q = ((Entity*)b->getBulletComponent()->getShooter())->getPlayerComponent();
			I32 dmg = (I32)std::floor(glm::length2(b->getRigidbody()->getVelocity()) / 15.0f);
			p->takeDamage(dmg);
			q->setBulletsHit(q->getBulletsHit()+1);
			if(p->isDead())
			{
				if(p->getId() == q->getId())
				{   //self-kill
					p->setDeaths(p->getDeaths()+1);
					std::cerr<<p->getId().to_useful_string()<<" suicided ("
								<< p->getDeaths() << ")" << std::endl << std::endl;
				}
				else
				{
					p->setTimeOfDeath(glfwGetTime());
					p->setDeaths(p->getDeaths()+1);
					q->setKills(q->getKills()+1);
					std::cerr << p->getId().to_useful_string() << " died ("
								<< p->getDeaths() << ")" << std::endl << q->getId().to_useful_string()
								<< " got the kill (" << q->getKills() << ")"
								<< std::endl << std::endl;
				}
				_toRespawn.push(a);
				_toDisable.push(a);
			}

			_toDisable.push(b);
			break;
		}
		default:
			break;
		}
        
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::EndContact(b2Contact* contact)
///
/// \brief    Ends a contact.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param [in,out]    contact    If non-null, the contact.
////////////////////////////////////////////////////////////////////////////////
void Game::EndContact(b2Contact* contact)
{
    //std::cerr<<"EndContact"<<std::endl;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::PreSolve(b2Contact* contact, const b2Manifold* manifold)
///
/// \brief    Pre solve.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param [in,out]    contact    If non-null, the contact.
/// \param    manifold            The manifold.
////////////////////////////////////////////////////////////////////////////////
void Game::PreSolve(b2Contact* contact, const b2Manifold* manifold)
{
    //handle presolve
    //std::cerr<<"PreSolve"<<std::endl;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::PostSolve(b2Contact* contact,
///     const b2ContactImpulse* impulse)
///
/// \brief    Posts a solve.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param [in,out]    contact    If non-null, the contact.
/// \param    impulse                The impulse.
////////////////////////////////////////////////////////////////////////////////
void Game::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
    //handle post solve
    //std::cerr<<"PostSolve"<<std::endl;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::onKeyboard(I32 keycode, I32 scancode, I32 action,
///     I32 modifiers)
///
/// \brief    Executes the keyboard action.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    keycode      The keycode.
/// \param    scancode     The scancode.
/// \param    action        The action.
/// \param    modifiers    The modifiers.
////////////////////////////////////////////////////////////////////////////////
void Game::onKeyboard(I32 keycode, I32 scancode, I32 action, I32 modifiers)
{
    if(action == GLFW_PRESS || action == GLFW_REPEAT)
        checkMovement(keycode, action);
    if(action == GLFW_RELEASE)
    {
        _instance->_running = !(keycode == GLFW_KEY_ESCAPE);
        
          if(keycode == GLFW_KEY_H)
          {
                help();
          }
        else if(keycode == _controls.keyJump[0] || keycode == _controls.keyJump[1])
        {
            _scene.getLocalPlayer()->getPlayerComponent()->endThrust();
        }

    }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::onMouseLeftDown(I32 mods)
///
/// \brief    Executes the mouse left down action.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    mods    The mods.
////////////////////////////////////////////////////////////////////////////////
void Game::onMouseLeftDown(I32 mods)
{
    F64 x,y;
    glfwGetCursorPos(getEngine().getWindow()->getGlfwHandle(), &x, &y);
	ivec2 screenCoords = ivec2((I32)std::floor(x), (I32)std::floor(y));
    ivec2 size = getEngine().getWindow()->getContextSize();
	vec2 worldPos = _scene.getCurrentCamera()->getWorldPosition(screenCoords, size);
	_scene.getLocalPlayer()->getPlayerComponent()->fire(worldPos.x, worldPos.y);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::checkMovement(I32 keycode, I32 action)
///
/// \brief    Check movement.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    keycode    The keycode.
/// \param    action     The action.
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
    }
    else if(keycode == _controls.keyFire2[0] || keycode == _controls.keyFire2[1])
    {
    }
    else if(keycode == _controls.keyJump[0] || keycode == _controls.keyJump[1])
    {
        _scene.getLocalPlayer()->getPlayerComponent()->doThrust();
    }
    else if(keycode == _controls.keyAction[0] || keycode == _controls.keyAction[1])
    {
    }
}
#pragma endregion
////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::help()
///
/// \brief    Helps this object.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
////////////////////////////////////////////////////////////////////////////////
void Game::help()
{
    std::cerr << std::endl << std::endl;
    std::cerr << "HELP MENU" << std::endl;
    std::cerr << std::endl << std::endl;
    std::cerr << "--Controls--" << std::endl;
    std::cerr << std::endl;
    std::cerr << "w - Press w to jump" << std::endl;
    std::cerr << "a - Press a to move left" << std::endl;
    std::cerr << "s - Press s to duck(tentative)" << std::endl;
    std::cerr << "d - Press d to move right" << std::endl;
    std::cerr << "h - Press h to display help menu" << std::endl;
    std::cerr << "ESC - Press ESC to exit the game" << std::endl;
    std::cerr << std::endl << std::endl;
    std::cerr << "Move the mouse to change the fire direction" << std::endl;
    std::cerr << "Press the left mouse button to fire the weapon" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::spawnBullet(const vec2& position, const vec2& velocity)
///
/// \brief    Spawn bullet.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    position    The position.
/// \param    velocity    The velocity.
////////////////////////////////////////////////////////////////////////////////
void Game::spawnBullet(const vec2& position, const vec2& velocity, void* shooter)
{
    Entity* bullet = _scene.getBullet(_bulletRing[_curRingIdx++]);
    bullet->getTransform().setPosition(position);
    bullet->getTransform().updateOwnerRigidbody();
    bullet->getRigidbody()->setVelocity(velocity);
    bullet->getRigidbody()->setAngularVelocity(6.28318f);
	bullet->getBulletComponent()->setShooter(shooter);
	bullet->enable();
    if(_curRingIdx >= 100)
        _curRingIdx -= 100;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Game::disableBullet(Entity* e)
///
/// \brief	Disables the bullet.
///
/// \author	Peter Bartosch
/// \date	2013-08-14
///
/// \param [in,out]	e	If non-null, the Entity* to process.
////////////////////////////////////////////////////////////////////////////////
void Game::disableBullet(Entity* e)
{
	_toDisable.push(e);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Game::respawnPlayer(Entity* e)
///
/// \brief	Respawn player.
///
/// \author	Peter Bartosch
/// \date	2013-08-14
///
/// \param [in,out]	e	If non-null, the Entity* to process.
////////////////////////////////////////////////////////////////////////////////
void Game::respawnPlayer(Entity* e)
{
	_toDisable.push(e);
	_toRespawn.push(e);
}

#pragma region entity_makes
////////////////////////////////////////////////////////////////////////////////
/// \fn    Entity* Game::makeBullet()
///
/// \brief    Makes the bullet.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    null if it fails, else.
////////////////////////////////////////////////////////////////////////////////
Entity* Game::makeBullet()
{
    Entity* e = new Entity();
    e->setType(Entity::EntityType::Bullet);
    e->getTransform().setScale(0.5f, 0.5f);
    e->addBulletComponent();
    e->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("bullets"))));
    e->setShape(new ShapeTriangle());
    e->addRigidbody(physics::Rigidbody::BodyType::Dynamic, _world);
    e->getRigidbody()->setBullet(true);
    e->getRigidbody()->setActive(false);
    return e;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    Entity* Game::makePlayer()
///
/// \brief    Makes the player.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    null if it fails, else.
////////////////////////////////////////////////////////////////////////////////
Entity* Game::makePlayer(be::Id id, F32 x, F32 y, bool addAI)
{
    Entity* p = new Entity();
    p->setType(Entity::Player);
    p->getTransform().setPosition(vec2(x, y));
    p->getTransform().setScale(vec2(1.0f, 2.0f));
	/*if(addAI)
		p->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("bots"))));
	else
		p->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("player"))));*/
    
	p->setShape(new ShapeSquare());
    p->addRigidbody(physics::Rigidbody::BodyType::Dynamic, _world);
    p->getRigidbody()->setFixedRotation(true);
    
	p->addPlayerComponent(id);
	p->getPlayerComponent()->setMaxAmmo(1000);
	p->getPlayerComponent()->setAmmoRemaining(1000);
	
	p->addAudioSource();
	p->getAudioSource()->addAudioBuffer("fire", _engine.getResourceManager().getSound(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("wpnfire"))));
	p->getAudioSource()->addAudioBuffer("dmg", _engine.getResourceManager().getSound(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("dmg"))));
	p->getAudioSource()->addAudioBuffer("death", _engine.getResourceManager().getSound(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("death"))));
	p->getAudioSource()->updatePosition();
	p->getAudioSource()->updateVelocity();

	if(addAI)
		p->addAIComponent();

    p->enableDraw();
    return p;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    Entity* Game::makeTerrain()
///
/// \brief    Makes the terrain.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    null if it fails, else.
///
/// \details Since we can't change the size of a Rigidbody yet, this is a kind
///             useless function unless something changes.
////////////////////////////////////////////////////////////////////////////////
Entity* Game::makeTerrain(F32 x, F32 y, F32 width, F32 height)
{
    Entity* t = new Entity();
    t->setType(Entity::EntityType::Terrain);
    t->getTransform().setPosition(x, y);
    t->getTransform().setScale(width, height);
    t->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("terrain"))));
    t->setShape(new ShapeSquare());
    t->addRigidbody(Rigidbody::BodyType::Static, _world);
    t->enableDraw();
    return t;
}

Entity* Game::makeSpawnPoint(F32 x, F32 y)
{
    Entity* s = new Entity();
    s->setType(Entity::EntityType::SpawnPoint);
    s->getTransform().setPosition(x, y);
    s->getTransform().setScale(1.0f, 1.0f);
    s->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("red"))));
    s->setShape(new ShapeSquare());

#ifndef PBJ_EDITOR
    //disable this when not testing
    //s->enableDraw();
	s->disableDraw();
#else
    // When in the editor, draw spawnpoints no matter what.
    s->enableDraw();
#endif
    return s;
}
#pragma endregion
} // namespace pbj