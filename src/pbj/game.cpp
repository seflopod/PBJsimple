#ifndef GAME_H_
#include "pbj/game.h"
#endif

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
		Entity* e = new Entity();
		e->init();
		e->setType(Entity::EntityType::Bullet);
		e->getTransform()->setScale(0.5f, 0.5f);
		e->addRigidbody(physics::Rigidbody::BodyType::Dynamic, _world);
		e->getRigidbody()->setBullet(true);
		e->getRigidbody()->setActive(false);
		
		_bullets.push(std::unique_ptr<Entity>(e));
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

void Game::onContextResized(I32 width, I32 height)
{
    GLdouble ratio = width/(GLdouble)height;
    glViewport(0, 0, width, height);
}

void Game::initTestScene()
{
    Entity* p = new Entity();
	p->init();
	p->enableDraw();
    p->setType(Entity::Player);
	p->getTransform()->setPosition(vec2(0.0f, 25.0f));
	p->getTransform()->setScale(vec2(1.0f, 2.0f));
	p->addRigidbody(physics::Rigidbody::BodyType::Dynamic, _world);
	p->addPlayerComponent();
	_scene.addEntity(std::unique_ptr<Entity>(p));
    _scene.setLocalPlayer(p->getSceneId());

	Entity* t = new Entity();
	t->init();
	t->setType(Entity::EntityType::Terrain);
	t->getTransform()->setPosition(0.0f, -15.0f);
	t->getTransform()->setScale(100.0f, 10.0f);
	t->addRigidbody(Rigidbody::BodyType::Static, _world);
    //e->getTransform()->updateOwnerRigidbody();
	t->enableDraw();
	_scene.addEntity(std::unique_ptr<Entity>(t));
}

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

void Game::EndContact(b2Contact* contact)
{
	//std::cerr<<"EndContact"<<std::endl;
}

void Game::PreSolve(b2Contact* contact, const b2Manifold* manifold)
{
	//handle presolve
	//std::cerr<<"PreSolve"<<std::endl;
}

void Game::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	//handle post solve
	//std::cerr<<"PostSolve"<<std::endl;
}
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

void Game::spawnBullet(vec2& position, vec2& velocity)
{
	if(!_bullets.empty())
	{
		std::unique_ptr<Entity> e(std::move(_bullets.front()));
		e->getTransform()->setPosition(position);
		e->getRigidbody()->setActive(true);
		e->getTransform()->updateOwnerRigidbody();
		e->getRigidbody()->setVelocity(velocity);
		_scene.addEntity(std::move(e));
		_bullets.pop();
	}
}

void Game::onMouseLeftDown(I32 mods)
{
	F64 x,y;
	glfwGetCursorPos(getEngine().getWindow()->getGlfwHandle(), &x, &y);
	ivec2 size = getEngine().getWindow()->getContextSize();
	double ratio = size.x/(double)size.y;
	x = x/((double)size.x) * (2*grid_height*ratio) - grid_height*ratio;
	y = grid_height * (1 - y/size.y) - grid_height/2;
	_scene.getLocalPlayer()->getPlayerComponent()->fire((F32)x,(F32)y);
}

} // namespace pbj