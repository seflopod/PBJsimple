#ifndef GAME_H_
#include "pbj/game.h"
#endif

using namespace pbj;

#pragma region statics
/// \brief	The client instance pointer.
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
    });

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

	//remove when making for reals
	initTestScene();

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

	while(_running)
	{
		F64 frameStart = glfwGetTime();

		update();
		draw();

		F64 frameTime = lastFrameTime = glfwGetTime() - frameStart;

		if(lastFrameTime < _dt)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(I32(
				1000000 * (_dt- lastFrameTime))));
			frameTime = glfwGetTime() - frameStart;
		}

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
	//nothing doing for now
}

void Game::initTestScene()
{
	scene::Entity* e = new scene::Entity();
	e->setType(scene::Entity::EntityType::Terrain);
	e->enableDraw();
	_scene.addEntity(std::unique_ptr<scene::Entity>(e));
}