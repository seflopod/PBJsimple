////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJsimple\src\pbj\game.cpp
///
/// \brief	Implements the game class.
////////////////////////////////////////////////////////////////////////////////
#ifndef GAME_H_
#include "pbj/game.h"
#endif

#include <stdio.h>
#include "pbj/scene/ui_label.h"

using pbj::scene::Entity;

namespace pbj {

#pragma region statics
/// \brief    The client instance pointer.
Game* Game::_instance = 0;


////////////////////////////////////////////////////////////////////////////////
/// \fn Game* Game::instance()
///
/// \brief Returns a pointer to the single instance of Game.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return A pointer to a Game instance.  If \c _instance is null when this
///         is called, a new instance of Game is created and a pointer to that
///         is returned.
////////////////////////////////////////////////////////////////////////////////
Game* Game::instance()
{
    if(_instance == 0) //no instance yet
        _instance = new Game();
    return _instance;
}

void Game::destroyInstance()
{
    if(_instance != 0)
    {
        _instance->stop();
        delete _instance;
        _instance = 0;
    }
}
#pragma endregion

#pragma region ctor_dtor
////////////////////////////////////////////////////////////////////////////////
/// \fn    Game::Game()
///
/// \brief    Default constructor.
///
/// \author    Peter Bartosch
/// \date    2013-08-05
////////////////////////////////////////////////////////////////////////////////
Game::Game() :
    _dt(0.0f),
    _running(false),
    _engine(getEngine()),
    _window(*getEngine().getWindow())
{}

////////////////////////////////////////////////////////////////////////////////
/// \fn    Game::~Game()
///
/// \brief    Destructor.
///
/// \author    Peter Bartosch
/// \date    2013-08-05
////////////////////////////////////////////////////////////////////////////////
Game::~Game()
{}
#pragma endregion

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
	
	initBasicMaterials();
    //remove when making for reals
    initTestScene();

	//seems like an odd place to setup gl matrices, but there we go
	ivec2 ctxtSize = _window.getContextSize();
	GLdouble ratio = ctxtSize.x/(GLdouble)ctxtSize.y;
	glViewport(0, 0, ctxtSize.x, ctxtSize.y);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio*grid_height/2, ratio*grid_height/2, -grid_height/2, grid_height/2, 0.1f, -0.1f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//make all the bullets we'll ever need
	for(I32 i=0;i<1000;++i)
	{
		_bullets.push(std::unique_ptr<Entity>(makeBullet()));
	}
    _running = true;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    I32 Game::run()
///
/// \brief    Runs the main loop for the game.
///
/// \author    Peter Bartosch
/// \date    2013-08-05
///
/// \return    The exit code for the client.
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
		//std::cerr<<fps<<std::endl;
		//This does not work due to issues with UIRoot and input registration
		/*I8 fpsCStr[13];
		sprintf_s((char*)fpsCStr,12,"FPS: %.4d", fps);
		fpsCStr[12] = '\0';
		((scene::UILabel*)_scene.ui.panel.getElementAt(ivec2(0,0)))->setText((char*)fpsCStr);
		*/
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::stop()
///
/// \brief    Stops this object.
///
/// \author    Peter Bartosch
/// \date    2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Game::stop()
{
    _running = false;
    //right now not much else that I can think of, but we'll see.
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    bool Game::update()
///
/// \brief    Updates this object.
///
/// \author    Peter Bartosch
/// \date    2013-08-05
///
/// \return    true if the next loop should happen; false otherwise.  This is being
///         changed to a void eventually.
////////////////////////////////////////////////////////////////////////////////
bool Game::update()
{
    _scene.update(_dt);

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
/// \return    true if the next loop should happen; false otherwise.  This is being
///         changed to a void eventually.
/// 
/// \details This needs to be separate loop because it is likely that the
///          physics (and thus collision) step is shorter than the normal update
///          and draw step.  This will take care of anything being done that
///          relates to physics.
////////////////////////////////////////////////////////////////////////////////
bool Game::physUpdate()
{
    _world->Step(_physSettings.dt, _physSettings.velocityIterations,
                    _physSettings.positionIterations);
    
    //after all other physics updates, clear forces
    _world->ClearForces();
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

    ivec2 ctxtSize = _window.getContextSize();
	GLdouble ratio = ctxtSize.x/(GLdouble)ctxtSize.y;
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio*grid_height/2, ratio*grid_height/2, -grid_height/2, grid_height/2, 0.1f, -0.1f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
    _scene.draw();

    //error pump
    GLenum glError;
    while((glError = glGetError()) != GL_NO_ERROR)
    {
        PBJ_LOG(VWarning) << "OpenGL error while rendering" << PBJ_LOG_NL
                            << "Error code: " << glError << PBJ_LOG_NL
                            << "Error:      " << getGlErrorString(glError)
                            << PBJ_LOG_END;
    }

    glfwSwapBuffers(_window.getGlfwHandle());
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Game::onContextResized(I32 width, I32 height)
///
/// \brief	Executes the context resized action.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	width 	The width.
/// \param	height	The height.
////////////////////////////////////////////////////////////////////////////////
void Game::onContextResized(I32 width, I32 height)
{
    GLdouble ratio = width/(GLdouble)height;
    glViewport(0, 0, width, height);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Game::initTestScene()
///
/// \brief	Initialises the test scene.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
void Game::initTestScene()
{
   //add the local player to the scene
	U32 id = _scene.addEntity(std::unique_ptr<Entity>(makePlayer()));
    _scene.setLocalPlayer(id);
	_scene.getLocalPlayer()->getTransform()->setPosition(0.0f, -15.0f);
	_scene.getLocalPlayer()->getTransform()->updateOwnerRigidbody();
	//add terrain to the scene
	_scene.addEntity(std::unique_ptr<Entity>(makeTerrain(0.0f, -15.0f, 100.0f,
															10.0f)));
	id = _scene.addEntity(std::unique_ptr<Entity>(makeTerrain(-15.0f, 0.0f,
																25.0f, 5.0f)));

	_scene.addEntity(std::unique_ptr<Entity>(makeTerrain(37.0f, 5.0f, 10.0f, 5.0f)));

	_scene.addEntity(std::unique_ptr<Entity>(makeTerrain(10.0f, -5.0f, 5.0f, 5.0f)));

	_scene.addEntity(std::unique_ptr<Entity>(makeTerrain(-37.0f, 8.0f, 15.0f, 5.0f)));

	_scene.addEntity(std::unique_ptr<Entity>(makeTerrain(0.0f, 30.0f, 100.0f,
															10.0f)));

	_scene.addEntity(std::unique_ptr<Entity>(makeTerrain(0.0f, 20.0f, 5.0f, 15.0f)));

	_scene.addEntity(std::unique_ptr<Entity>(makeTerrain(-7.5f, 13.75f, 10.0f, 2.5f)));

	//add some UI to the scene
	//This does not work due to issues with UIRoot and input registration
	/*scene::UILabel label;
	label.setPosition(vec2(0,0));
	_scene.ui.panel.addElement(std::unique_ptr<scene::UILabel>(&label));
	*/
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Game::BeginContact(b2Contact* contact)
///
/// \brief	Begins a contact.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param [in,out]	contact	If non-null, the contact.
////////////////////////////////////////////////////////////////////////////////
void Game::BeginContact(b2Contact* contact)
{
	//handle collisions for the entire game here
	//std::cerr<<"BeginContact"<<std::endl;
	Entity* const a = (Entity* const)(contact->GetFixtureA()->GetBody()->GetUserData());
	Entity* const b = (Entity* const)(contact->GetFixtureB()->GetBody()->GetUserData());
	if(!a || !b)
	{
		PBJ_LOG(pbj::VError) << "Collision between untracked rigidbodies. "<< PBJ_LOG_END;
		return;
	}

	if(a->getType() == Entity::EntityType::Player && b->getType() == Entity::EntityType::Terrain)
	{
		a->getPlayerComponent()->enableJump();
	}
	else if(b->getType() == Entity::EntityType::Player && a->getType() == Entity::EntityType::Terrain)
	{
		b->getPlayerComponent()->enableJump();
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Game::EndContact(b2Contact* contact)
///
/// \brief	Ends a contact.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param [in,out]	contact	If non-null, the contact.
////////////////////////////////////////////////////////////////////////////////
void Game::EndContact(b2Contact* contact)
{
	//std::cerr<<"EndContact"<<std::endl;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Game::PreSolve(b2Contact* contact, const b2Manifold* manifold)
///
/// \brief	Pre solve.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param [in,out]	contact	If non-null, the contact.
/// \param	manifold	   	The manifold.
////////////////////////////////////////////////////////////////////////////////
void Game::PreSolve(b2Contact* contact, const b2Manifold* manifold)
{
	//handle presolve
	//std::cerr<<"PreSolve"<<std::endl;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Game::PostSolve(b2Contact* contact,
/// 	const b2ContactImpulse* impulse)
///
/// \brief	Posts a solve.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param [in,out]	contact	If non-null, the contact.
/// \param	impulse		   	The impulse.
////////////////////////////////////////////////////////////////////////////////
void Game::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	//handle post solve
	//std::cerr<<"PostSolve"<<std::endl;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Game::help()
///
/// \brief	Helps this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
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
/// \fn	void Game::onKeyboard(I32 keycode, I32 scancode, I32 action,
/// 	I32 modifiers)
///
/// \brief	Executes the keyboard action.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	keycode  	The keycode.
/// \param	scancode 	The scancode.
/// \param	action   	The action.
/// \param	modifiers	The modifiers.
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
/// \fn	void Game::checkMovement(I32 keycode, I32 action)
///
/// \brief	Check movement.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	keycode	The keycode.
/// \param	action 	The action.
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Game::spawnBullet(const vec2& position, const vec2& velocity)
///
/// \brief	Spawn bullet.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	position	The position.
/// \param	velocity	The velocity.
////////////////////////////////////////////////////////////////////////////////
void Game::spawnBullet(const vec2& position, const vec2& velocity)
{
	if(!_bullets.empty())
	{
		std::unique_ptr<Entity> e(std::move(_bullets.front()));
		e->getTransform()->setPosition(position);
		e->getRigidbody()->setActive(true);
		e->getTransform()->updateOwnerRigidbody();
		e->getRigidbody()->setVelocity(velocity);
		e->getRigidbody()->setAngularVelocity(60.0f);
		_scene.addEntity(std::move(e));
		_bullets.pop();
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Game::onMouseLeftDown(I32 mods)
///
/// \brief	Executes the mouse left down action.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	mods	The mods.
////////////////////////////////////////////////////////////////////////////////
void Game::onMouseLeftDown(I32 mods)
{
	F64 x,y;
	glfwGetCursorPos(getEngine().getWindow()->getGlfwHandle(), &x, &y);
	ivec2 size = getEngine().getWindow()->getContextSize();
	double ratio = size.x/(double)size.y;
	x = x/((double)size.x) * (2*grid_height*ratio) - grid_height*ratio;
	y = grid_height * (1 - y/size.y) - grid_height/2;
	_scene.getLocalPlayer()->getPlayerComponent()->fire((F32)x/2.0f,(F32)y);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Game::initBasicMaterials()
///
/// \brief	Initialises the basic materials.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
void Game::initBasicMaterials()
{
	_materials["red"] = std::shared_ptr<Material>((new Material()));
	_materials["red"]->setColor(color4(1.0f, 0.0f, 0.0f, 1.0f));

	_materials["green"] = std::shared_ptr<Material>((new Material()));
	_materials["green"]->setColor(color4(0.0f, 1.0f, 0.0f, 1.0f));

	_materials["blue"] = std::shared_ptr<Material>((new Material()));
	_materials["blue"]->setColor(color4(0.0f, 0.0f, 1.0f, 1.0f));

	_materials["cyan"] = std::shared_ptr<Material>((new Material()));
	_materials["cyan"]->setColor(color4(0.0f, 1.0f, 1.0f, 1.0f));

	_materials["magenta"] = std::shared_ptr<Material>((new Material()));
	_materials["magenta"]->setColor(color4(1.0f, 0.0f, 1.0f, 1.0f));

	_materials["yellow"] = std::shared_ptr<Material>((new Material()));
	_materials["yellow"]->setColor(color4(1.0f, 1.0f, 0.0f, 1.0f));

	_materials["black"] = std::shared_ptr<Material>((new Material()));
	_materials["black"]->setColor(color4(0.0f, 0.0f, 0.0f, 1.0f));

	_materials["white"] = std::shared_ptr<Material>((new Material()));
	_materials["white"]->setColor(color4(1.0f, 1.0f, 1.0f, 1.0f));
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Entity* Game::makeBullet()
///
/// \brief	Makes the bullet.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	null if it fails, else.
////////////////////////////////////////////////////////////////////////////////
Entity* Game::makeBullet()
{
	Entity* e = new Entity();
	e->init();
	e->setType(Entity::EntityType::Bullet);
	e->getTransform()->setScale(0.5f, 0.5f);
	e->addMaterial(_materials["cyan"]);
	e->addShape(new ShapeTriangle());
	e->addRigidbody(physics::Rigidbody::BodyType::Dynamic, _world);
	e->getRigidbody()->setBullet(true);
	e->getRigidbody()->setActive(false);
	return e;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Entity* Game::makePlayer()
///
/// \brief	Makes the player.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	null if it fails, else.
////////////////////////////////////////////////////////////////////////////////
Entity* Game::makePlayer()
{
	Entity* p = new Entity();
	p->init();
	p->setType(Entity::Player);
	p->getTransform()->setPosition(vec2(0.0f, 25.0f));
	p->getTransform()->setScale(vec2(1.0f, 2.0f));
	p->addMaterial(_materials["magenta"]);
	p->addShape(new ShapeSquare());
	p->addRigidbody(physics::Rigidbody::BodyType::Dynamic, _world);
	p->getRigidbody()->setFixedRotation(true);
	p->addPlayerComponent();
	p->enableDraw();
	return p;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Entity* Game::makeTerrain()
///
/// \brief	Makes the terrain.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	null if it fails, else.
///
/// \details Since we can't change the size of a Rigidbody yet, this is a kind
///          useless function unless something changes.
////////////////////////////////////////////////////////////////////////////////
Entity* Game::makeTerrain(F32 x, F32 y, F32 width, F32 height)
{
	Entity* t = new Entity();
	t->init();
	t->setType(Entity::EntityType::Terrain);
	t->getTransform()->setPosition(x, y);
	t->getTransform()->setScale(width, height);
	t->addMaterial(_materials["green"]);
	t->addShape(new ShapeSquare());
	t->addRigidbody(Rigidbody::BodyType::Static, _world);
	t->enableDraw();
	return t;
}

} // namespace pbj