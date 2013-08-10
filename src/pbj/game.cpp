#ifndef GAME_H_
#include "pbj/game.h"
#endif

using namespace pbj;

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

    InputController::registerKeyUpListener(
        [&](I32 keycode, I32 scancode, I32 modifiers) {
        
        _instance->_running = !(keycode == GLFW_KEY_ESCAPE);
    });

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    //setup physics, using variables instead of straight numbers so I can
    //remember what does what.
    _world = getEngine().getWorld();
    _physSettings = PhysicsSettings();



    //remove when making for reals
    initTestScene();

    //seems like an odd place to setup gl matrices, but there we go
	F32 oScale = 100.0f;
    ivec2 ctxtSize = _window.getContextSize();
    GLdouble ratio = ctxtSize.x/(GLdouble)ctxtSize.y;
    glViewport(0, 0, ctxtSize.x, ctxtSize.y);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio*oScale, ratio*oScale, -oScale, oScale, 0.1f, -0.1f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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
    // TODO:
    // fit game engine in here
    _scene.update();

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
    scene::Entity* e = new scene::Entity();
    e->setType(scene::Entity::EntityType::Terrain);
	e->getTransform()->setPosition(vec2(0.0f, 75.0f));
	e->addRigidbody(Rigidbody::BodyType::Dynamic, _world);
    e->enableDraw();
    _scene.addEntity(std::unique_ptr<scene::Entity>(e));
}

void Game::BeginContact(b2Contact* contact)
{
	//handle collisions for the entire game here
}

void Game::EndContact(b2Contact* contact)
{
	//handle end of collisions for the entire game here
}
