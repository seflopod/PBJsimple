#ifndef GAME_H_
#include "pbj/game.h"
#endif

using namespace pbj;

#pragma region statics
/// \brief	The client instance pointer.
Game* Game::_instance = 0;
scene::Entity* p = new scene::Entity();
////////////////////////////////////////////////////////////////////////////////
/// \fn Game* Game::instance()
///
/// \brief Returns a pointer to the single instance of Game.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return A pointer to a Game instance.  If \c _instance is null when this
/// 		is called, a new instance of Game is created and a pointer to that
/// 		is returned.
////////////////////////////////////////////////////////////////////////////////
Game* Game::instance()
{
	if(_instance == 0) //no instance yet
		_instance = new Game();
	return _instance;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Game::destroyInstance()
///
/// \brief Destroys the static instance of Game.
///
/// \author Peter Bartosch
/// \date 2013-08-08
////////////////////////////////////////////////////////////////////////////////
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
/// \fn	Game::Game()
///
/// \brief	Default constructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
Game::Game() :
	_dt(0.0f),
	_running(false),
	_engine(getEngine()),
	_window(*getEngine().getWindow())
{}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Game::~Game()
///
/// \brief	Destructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
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

    InputController::registerKeyUpListener(
		[&](I32 keycode, I32 scancode, I32 modifiers) {
		
        _instance->_running = !(keycode == GLFW_KEY_ESCAPE);
        if(keycode == GLFW_KEY_H)
        {
            help();
        }

		if(keycode == GLFW_KEY_D)
		{
			move();
		}
    });

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

	//setup physics, using variables instead of straight numbers so I can
	//remember what does what.
	_world = getEngine().getWorld();
	_physDt = 1.0f/60.0f;
	I32 velIter = 10;
	I32 posIter = 8;
	_world->Step((float32)_physDt, (int32)velIter, (int32)posIter);



	//remove when making for reals
	initTestScene();

	//seems like an odd place to setup gl matrices, but there we go
	ivec2 ctxtSize = _window.getContextSize();
	GLdouble ratio = ctxtSize.x/(GLdouble)ctxtSize.y;
	glViewport(0, 0, ctxtSize.x, ctxtSize.y);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.0f, 1.0f, 0.1f, -0.1f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	_running = true;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 Game::run()
///
/// \brief	Runs the main loop for the game.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	The exit code for the client.
////////////////////////////////////////////////////////////////////////////////
I32 Game::run()
{
	F64 lastFrameTime = 0.0;
	F64 fps = 0.0;
	F32 nonPhysTimer = 0.0f;

	while(_running)
	{
		F64 frameStart = glfwGetTime();

		physUpdate();
		if(nonPhysTimer >= _dt)
		{
			update();
			draw();
			nonPhysTimer-=_dt;
		}

		F64 frameTime = lastFrameTime = glfwGetTime() - frameStart;

		if(lastFrameTime < _physDt)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(I32(
				1000000 * (_physDt- lastFrameTime))));
			frameTime = glfwGetTime() - frameStart;
		}
		nonPhysTimer+=_physDt;

		fps = 1.0/frameTime;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Game::stop()
///
/// \brief	Stops this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Game::stop()
{
	_running = false;
	//right now not much else that I can think of, but we'll see.
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Game::update()
///
/// \brief	Updates this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	true if the next loop should happen; false otherwise.  This is being
/// 		changed to a void eventually.
////////////////////////////////////////////////////////////////////////////////
bool Game::update()
{
	// TODO:
	// fit game engine in here
	glfwPollEvents();

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
/// \return	true if the next loop should happen; false otherwise.  This is being
/// 		changed to a void eventually.
/// 
/// \details This needs to be separate loop because it is likely that the
/// 		 physics (and thus collision) step is shorter than the normal update
/// 		 and draw step.  This will take care of anything being done that
/// 		 relates to physics.
////////////////////////////////////////////////////////////////////////////////
bool Game::physUpdate()
{
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.0f, 1.0f, 0.1f, -0.1f);
}

void Game::initTestScene()
{
	//scene::Entity* e = new scene::Entity();
	//e->setType(scene::Entity::EntityType::Terrain);
	//e->enableDraw();
	//_scene.addEntity(std::unique_ptr<scene::Entity>(e));

	p->setType(scene::Entity::EntityType::Player);
	p->enableDraw();
	_scene.addEntity(std::unique_ptr<scene::Entity>(p));
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

void Game::move()
{
	vec2 moveP = _trans.getPosition();
	p->getTransform()->move(moveP.x += 0.1, moveP.y);
	//moveP.x += 1;
}