////////////////////////////////////////////////////////////////////////////////
/// \file Z:\Documents\PBJsimple\src\pbj\scene\scene.cpp
///
/// \brief Implements the scene class.

#include "pbj/scene/scene.h"

#include "pbj/sw/sandwich_open.h"
#include "be/bed/transaction.h"
#include "pbj/game.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#pragma region SQL queries
///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load a scene's name from a sandwich.
#define PBJSQL_LOAD \
    "SELECT name FROM sw_maps WHERE id = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load a scene's entities from a sandwich.
#define PBJSQL_GET_ENTITIES \
    "SELECT entity_id FROM sw_map_entities WHERE map_id = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to save a scene's name to a sandwich.
#define PBJSQL_SAVE \
    "INSERT OR REPLACE INTO sw_maps (id, name) VALUES (?,?)"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to clear a scene's entities in a sandwich.
#define PBJSQL_CLEAR_ENTITIES \
    "DELETE FROM sw_map_entities WHERE map_id = ?"

#ifdef BE_ID_NAMES_ENABLED
#define PBJSQLID_GET_NAME        PBJSQL_GET_NAME
#define PBJSQLID_GET_ENTITIES    PBJSQL_GET_ENTITIES
#define PBJSQLID_SET_NAME        PBJSQL_SET_NAME
#define PBJSQLID_CLEAR_ENTITIES  PBJSQL_CLEAR_ENTITIES
#else
// TODO: precalculate ids.
#define PBJSQLID_GET_NAME        PBJSQL_GET_NAME
#define PBJSQLID_GET_ENTITIES    PBJSQL_GET_ENTITIES
#define PBJSQLID_SET_NAME        PBJSQL_SET_NAME
#define PBJSQLID_CLEAR_ENTITIES  PBJSQL_CLEAR_ENTITIES
#endif

#pragma endregion

namespace pbj {
namespace scene {

////////////////////////////////////////////////////////////////////////////////
/// \fn Scene::Scene()
///
/// \brief Initializes a new instance of the Scene class.
///
/// \author Peter Bartosch
/// \date 2013-08-08
Scene::Scene()
    : _engine(getEngine()),
      _window(*getEngine().getWindow()),
      _prng(std::mt19937::result_type(time(nullptr))),
      _physWorld(b2Vec2(0.0f, -9.822f)),
      _nextEntityId(1),
      _curCameraId(U32(-1)),
      _localPlayerId(U32(-1)),
      _nextBulletId(U32(-1))
{
    _physWorld.SetAllowSleeping(true);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Scene::makeHud()
///
/// \brief  Initialises the user interface.
///
/// \author Josh Douglas
/// \date   2013-08-22
void Scene::makeHud()
{
    sw::ResourceId font_id(Id(PBJ_ID_PBJBASE), Id("std_font"));
    const gfx::TextureFont* font = &_engine.getResourceManager().getTextureFont(font_id);

    // Draws the UI to the screen with seperate lines for the kills/deaths and health
    I32 player = 0;
    I32 left = 20;
    I32 top = 20;
    I32 kd_offset = 20;
    I32 spacing = 50;
    for (EntityMap::iterator it=_players.begin(); it!=_players.end(); ++it)
    {
        //Setup positioning and color
        UILabel* lbl = new UILabel();
        _player_health_lbl[player] = lbl;
        _ui.panel.addElement(std::unique_ptr<UIElement>(lbl));
        lbl->setAlign(scene::UILabel::AlignLeft);
        lbl->setFont(font);
        lbl->setTextScale(vec2(2, 2));
        lbl->setTextColor(it->second->getMaterial()->getColor());
        lbl->setDimensions(vec2(200, 10));
        lbl->setPosition(vec2(left, top));

        //Setup positioning and color
        lbl = new UILabel();
        _player_kd_lbl[player] = lbl;
        _ui.panel.addElement(std::unique_ptr<UIElement>(lbl));
        lbl->setAlign(scene::UILabel::AlignLeft);
        lbl->setFont(font);
        lbl->setTextScale(vec2(2, 2));
        lbl->setTextColor(it->second->getMaterial()->getColor());
        lbl->setDimensions(vec2(200, 10));
        lbl->setPosition(vec2(left, top + kd_offset));

        ++player;
        top += spacing;
    }
}


////////////////////////////////////////////////////////////////////////////////
/// \fn void Scene::draw()
///
/// \brief Draws the scene
///
/// \author Peter Bartosch
/// \author Josh Douglas (UI)
/// \date 2013-08-08
///
/// \details This will go through each EntityMap for every drawable EntityType
///          and draw its members.  This will also draw the UI.
void Scene::draw()
{
    // Set up scene camera
    Entity* current_camera = getCurrentCamera();
    if (current_camera)
        current_camera->getCamera()->use();

    // Draw spawn points if this is the editor
#ifdef PBJ_EDITOR
    for (auto i(_spawnPoints.begin()), end(_spawnPoints.end()); i != end; ++i)
        i->second->draw();
#endif

    // Draw terrain
    for (auto i(_terrain.begin()), end(_terrain.end()); i != end; ++i)
        i->second->draw();

    // Draw bullets
    for (auto i(_bullets.begin()), end(_bullets.end()); i != end; ++i)
        i->second->draw();

    // Draw players
    for (auto i(_players.begin()), end(_players.end()); i != end; ++i)
        i->second->draw();

    // Draw UI/HUD
    _ui.draw();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Scene::update(F32 dt)
///
/// \brief  Updates the scene.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  dt  The delta time.
///
/// \details Like the draw method this updates each EntityMap in the scene
void Scene::update(F32 dt)
{
    // Update spawn points
    for (auto i(_spawnPoints.begin()), end(_spawnPoints.end()); i != end; ++i)
        i->second->update(dt);

    // Update terrain
    for (auto i(_terrain.begin()), end(_terrain.end()); i != end; ++i)
        i->second->update(dt);

    // Update bullets
    for (auto i(_bullets.begin()), end(_bullets.end()); i != end; ++i)
        i->second->update(dt);

    // Update players
    for (auto i(_players.begin()), end(_players.end()); i != end; ++i)
        i->second->update(dt);
    
    // Update current camera's target position/velocity
    Entity* local_player = getLocalPlayer();
    Entity* current_camera = getCurrentCamera();
    if (local_player && current_camera)
    {
        current_camera->getCamera()->setTargetPosition(local_player->getTransform().getPosition());
        current_camera->getCamera()->setTargetVelocity(local_player->getRigidbody()->getVelocity());
    }

    // Update cameras
    for (auto i(_cameras.begin()), end(_cameras.end()); i != end; ++i)
        i->second->update(dt);

    // Update bullets
    for (auto i(_bullets.begin()), end(_bullets.end()); i != end; ++i)
        i->second->update(dt);

    // Check for any respawns that need to be done
    if(!_toRespawn.empty())
    {
        F64 t = glfwGetTime();

        //if the front of the queue isn't ready to respawn, we can assume that
        //nothing behind it is ready.
        while (!_toRespawn.empty() &&
                _toRespawn.front()->getPlayerComponent()->getTimeOfDeath() + 2 <= t) // 2 second delay to respawn
        {
            Entity* e = _toRespawn.front();
            vec2 spwn = getRandomSpawnPoint()->getTransform().getPosition();

            e->enable();
            e->getTransform().setPosition(spwn.x, spwn.y);
            e->getTransform().updateOwnerRigidbody();
            e->getRigidbody()->setVelocity(vec2(0.0f,0.0f));
            PlayerComponent* pc = e->getPlayerComponent();
            pc->setHealth(pc->getMaxHealth());
            pc->setAmmoRemaining(pc->getMaxAmmo());
            pc->setFuelRemaining(pc->getMaxFuel());

            _toRespawn.pop();
        }
    }


    // Update HUD
    // Updates the UI to display the correct number of kills
    // deaths and health, Josh
    U32 player = 0;
    for (auto i(_players.begin()), end(_players.end()); i != end; ++i)
    {
        PlayerComponent* p = i->second->getPlayerComponent();
        std::string kills = std::to_string(p->getKills());
        std::string deaths = std::to_string(p->getDeaths());

        std::ostringstream osh;
        osh << p->getId().to_useful_string()
            << std::setprecision(3)
            << " Health: "
            << ((p->getHealth() / (F32)p->getMaxHealth()) * 100);
        _player_health_lbl[player]->setText(osh.str());

        std::ostringstream oss;
        oss << p->getId().to_useful_string()
            << " Kills: " << kills
            << " Deaths: " << deaths;
        _player_kd_lbl[player]->setText(oss.str());

        if (++player >= 5)
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Scene::physUpdate()
///
/// \brief Does the update for the physics portion of the game
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \details This needs to be separate loop because it is likely that the
///          physics (and thus collision) step is shorter than the normal update
///          and draw step.  This will take care of anything being done that
///          relates to physics.
void Scene::physUpdate(F32 dt)
{
    _physWorld.Step(dt, _physVelocityIterations, _physPositionIterations);

    //after all other physics updates, clear forces
    _physWorld.ClearForces();

    //after we're done with the physics step, we need to disable anything queued
    //for it
    for (Entity* entity : _toDisable)
        entity->disable();

    _toDisable.clear();
}



////////////////////////////////////////////////////////////////////////////////
/// \fn void Scene::setName(const std::string& name)
///
/// \brief  Sets map name.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  name    The name.
void Scene::setName(const std::string& name)
{
    _name = name;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn const std::string& Scene::getName() const
///
/// \brief  Gets map name.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \return The map name.
const std::string& Scene::getName() const
{
    return _name;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn U32 Scene::addEntity(unique_ptr<Entity>&& e)
///
/// \brief Adds an entity to the scene
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \param [in] e A unique pointer to the Entity to add.
U32 Scene::addEntity(unique_ptr<Entity>&& e)
{
    U32 id = _nextEntityId;
    e->setSceneId(id);
    ++_nextEntityId;

    //Place the Entity in the appropriate map depending on its type
    switch(e->getType())
    {
    case Entity::EntityType::Terrain:    _terrain[id] = std::move(e); break;
    case Entity::EntityType::Player:     _players[id] = std::move(e); break;
    case Entity::EntityType::SpawnPoint: _spawnPoints[id] = std::move(e); break;
    case Entity::EntityType::Bullet:     _bullets[id] = std::move(e); break;
    case Entity::EntityType::Camera:     _cameras[id] = std::move(e); break;
    default:                             _others[id] = std::move(e); break;
    }

    return id;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Scene::removeEntity(U32 id, EntityType et)
///
/// \brief Removes an Entity from an EntityMap.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \param id The id of the Entity.
/// \param et The EntityType
///
/// \details The id for the Entity was returned when it was added with
///          Scene::addEntity.
void Scene::removeEntity(U32 id, Entity::EntityType et)
{
    switch(et)
    {
    case Entity::EntityType::Terrain:
        _terrain.erase(id);
        break;
    case Entity::EntityType::Player:
        _players.erase(id);
        break;
    case Entity::EntityType::SpawnPoint:
        _spawnPoints.erase(id);
        break;
    case Entity::EntityType::Bullet:
        _bullets.erase(id);
        break;
    default:
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Entity* Scene::getLocalPlayer()
///
/// \brief  Gets local player.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return A pointer to the local player Entity.
////////////////////////////////////////////////////////////////////////////////
Entity* Scene::getLocalPlayer()
{
     if (_localPlayerId == U32(-1))
          return nullptr;

     return _players[_localPlayerId].get();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Entity* Scene::getBullet(U32 id)
///
/// \brief  Gets a bullet.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  id  The scene id of the bullet to get.
///
/// \return null if it fails, else the bullet.
////////////////////////////////////////////////////////////////////////////////
Entity* Scene::getBullet(U32 id)
{
    EntityMap::iterator it = _bullets.find(id);
    if(it != _bullets.end())
        return _bullets[id].get();

    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Entity* Scene::getPlayer(U32 id)
///
/// \brief  Gets a player.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  id  The scene id of the player to get.
///
/// \return null if it fails, else the player.
////////////////////////////////////////////////////////////////////////////////
Entity* Scene::getPlayer(U32 id)
{
    EntityMap::iterator it = _players.find(id);
    if(it != _players.end())
        return _players[id].get();

    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Entity* Scene::getTerrain(U32 id)
///
/// \brief  Gets a terrain.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  id  The scene id of the terrain to get.
///
/// \return null if it fails, else the terrain.
////////////////////////////////////////////////////////////////////////////////
Entity* Scene::getTerrain(U32 id)
{
    EntityMap::iterator it = _terrain.find(id);
    if(it != _terrain.end())
        return _terrain[id].get();

    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Entity* Scene::getSpawnPoint(U32 id)
///
/// \brief  Gets spawn point.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  id  The scene id of the spawn point to get.
///
/// \return null if it fails, else the spawn point.
////////////////////////////////////////////////////////////////////////////////
Entity* Scene::getSpawnPoint(U32 id)
{
    EntityMap::iterator it = _spawnPoints.find(id);
    if(it != _spawnPoints.end())
        return _spawnPoints[id].get();

    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Entity* Scene::getRandomSpawnPoint()
///
/// \brief  Gets a random spawn point.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return A pointer to the Entity of a random element in the EntityMap for
///         spawn points.
////////////////////////////////////////////////////////////////////////////////
Entity* Scene::getRandomSpawnPoint()
{
    std::uniform_int_distribution<I32> dist(0, _spawnPoints.size()-1);
    I32 stop = dist(_rnd);
    I32 count = 0;
    EntityMap::iterator it = _spawnPoints.begin();

    while(it!=_spawnPoints.end() && count != stop)
    {
        it++;
        count++;
    }

    if(it == _spawnPoints.end())
        return nullptr;

    return it->second.get();
}



////////////////////////////////////////////////////////////////////////////////
/// \fn void Scene::setCurrentCamera(U32 id)
///
/// \brief  Sets current camera.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  id  The scene id of the camera to make current.
////////////////////////////////////////////////////////////////////////////////
void Scene::setCurrentCamera(U32 id)
{
    if(_cameras.find(id) != _cameras.end())
        _curCamera = _cameras[id]->getCamera();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn CameraComponent* Scene::getCurrentCamera() const
///
/// \brief  Gets current camera.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return A pointer to the CameraComponent that is used as the current camera.
////////////////////////////////////////////////////////////////////////////////
CameraComponent* Scene::getCurrentCamera() const { return _curCamera; }

////////////////////////////////////////////////////////////////////////////////
/// \fn b2World* Scene::getWorld() const
///
/// \brief Gets the physics world.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return A pointer to the b2World for Box2D simulations.
////////////////////////////////////////////////////////////////////////////////
b2World* Scene::getWorld() const { return _physWorld.get(); }



////////////////////////////////////////////////////////////////////////////////
/// \fn    Entity* Game::makePlayer()
///
/// \brief    Makes the player.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    A pointer to an Entity that has all the characteristics of a
///            player.
////////////////////////////////////////////////////////////////////////////////
Entity* Scene::makePlayer(const std::string& name, const vec2& position, bool local_player)
{
    Entity* e = new Entity();
    e->setType(Entity::Player);
    e->getTransform().setPosition(vec2(x, y));
    e->getTransform().setScale(vec2(1.0f, 2.0f));

    e->setShape(new ShapeSquare());
    e->addRigidbody(physics::Rigidbody::BodyType::Dynamic, &_physWorld);
    e->getRigidbody()->setFixedRotation(true);

    e->addPlayerComponent(name);
    e->getPlayerComponent()->setMaxAmmo(1000);
    e->getPlayerComponent()->setAmmoRemaining(1000);

    e->addAudioSource();
    e->getAudioSource()->addAudioBuffer("fire", _engine.getResourceManager().getSound(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("wpnfire"))));
    e->getAudioSource()->addAudioBuffer("dmg", _engine.getResourceManager().getSound(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("dmg"))));
    e->getAudioSource()->addAudioBuffer("death", _engine.getResourceManager().getSound(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("death"))));
    e->getAudioSource()->updatePosition();
    e->getAudioSource()->updateVelocity();

    if (local_player)
        _localPlayerId = ;
    else
        p->addAIComponent();

    p->enableDraw();
    return p;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    Entity* Game::makeBullet()
///
/// \brief    Makes a bullet.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    A pointer to an Entity that has all the characteristics of a
///            bullet.
////////////////////////////////////////////////////////////////////////////////
Entity* Game::makeBullet()
{
    Entity* e = new Entity();
    e->setType(Entity::EntityType::Bullet);
    e->getTransform().setScale(0.5f, 0.5f);
    e->addBulletComponent();
    e->setMaterial(&_engine.getResourceManager().getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("bullet"))));
    e->setShape(new ShapeTriangle());
    e->addRigidbody(physics::Rigidbody::BodyType::Dynamic, _scene->getWorld());
    e->getRigidbody()->setBullet(true);
    e->getRigidbody()->setActive(false);
    return e;
}



////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::spawnBullet(const vec2& position, const vec2& velocity)
///
/// \brief    Spawn bullet.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    position    The position of the bullet.
/// \param    velocity    The velocity for the bullet to have.
////////////////////////////////////////////////////////////////////////////////
void Game::spawnBullet(const vec2& position, const vec2& velocity, void* shooter)
{
    Entity* bullet = _scene->getBullet(_bulletRing[_curRingIdx++]);
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
/// \fn void Game::disableBullet(Entity* e)
///
/// \brief  Disables a bullet.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param [in] e   Sends a the id of the passed Entity to the Scene so that it
///                 can be disabled at the end of the physics loop.
////////////////////////////////////////////////////////////////////////////////
void Game::disableBullet(Entity* e)
{
    _scene->addToDisable(e->getSceneId());
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::respawnPlayer(Entity* e)
///
/// \brief    Respawn player.
///
/// \author    Peter Bartosch
/// \date    2013-08-14
///
/// \param [in,out]    e    If non-null, the Entity* to process.
////////////////////////////////////////////////////////////////////////////////
void Game::respawnPlayer(Entity* e)
{
    _scene->addToDisable(e->getSceneId());
    _toRespawn.push(e);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Game::BeginContact(b2Contact* contact)
///
/// \brief    Handles the beginning of contact between two rigidbodies.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param [in,out]    contact    The contact data.
////////////////////////////////////////////////////////////////////////////////
void Game::BeginContact(b2Contact* contact)
{
    //handle collisions for the entire game here

    //Anything colliding should be an Entity, so cast and check
    Entity* a = (Entity*)(contact->GetFixtureA()->GetBody()->GetUserData());
    Entity* b = (Entity*)(contact->GetFixtureB()->GetBody()->GetUserData());
    if(!a || !b)
    {   //if either was not an Entity we have some issues and should do any more
        //processing
        PBJ_LOG(pbj::VError) << "Collision between untracked rigidbodies. "
                                << PBJ_LOG_END;
        return;
    }

    //To cut down on the number of if-checks, make sure the player Entity is
    //in a (assuming one of the objects is a player).
    if(b->getType() == Entity::EntityType::Player &&
        a->getType() != Entity::EntityType::Player)
        std::swap(a, b);

    //if one of the objects is a player, we have some stuff to do.  Let's do it.
    if(a->getType() == Entity::EntityType::Player)
    {
        //make life easier by getting a pointer to the PlayerComponent on the
        //Entity.
        scene::PlayerComponent* p = a->getPlayerComponent();

        switch(b->getType())
        {
        case Entity::EntityType::Terrain:
        {   //A player hitting terrain usually means that it can jump again
            p->enableJump();
            break;
        }
        case Entity::EntityType::Bullet:
        {   //First we need to do damage calculations and then we need to check
            //for death.
            scene::PlayerComponent* q = ((Entity*)(b->getBulletComponent()->getShooter()))->getPlayerComponent();

            //being a little too fancy with damage.  Base damage taken on
            //velocity of bullet.
            I32 dmg = 100 + (I32)std::floor(
                        glm::length2(b->getRigidbody()->getVelocity()) / 15.0f);

            //scoring stuff.  Do damage to the hit party, make sure the shooter
            //knows that its bullet did the damage.
            p->takeDamage(dmg);
            q->setBulletsHit(q->getBulletsHit()+1);

            if(p->isDead())
            {   //If the player is dead we need to check how that death happened
                //then make sure it knows it died and ready the respawn
                p->setDeaths(p->getDeaths()+1);
                p->setTimeOfDeath(glfwGetTime());

                if(p->getId() == q->getId())
                {   //self-kill
                    std::cerr<<p->getId().to_useful_string()<<" suicided ("
                                << p->getDeaths() << ")" << std::endl << std::endl;
                }
                else
                {
                    //make sure the shooter gets credit for the kill
                    q->setKills(q->getKills()+1);
                    std::cerr << p->getId().to_useful_string() << " died ("
                                << p->getDeaths() << ")" << std::endl << q->getId().to_useful_string()
                                << " got the kill (" << q->getKills() << ")"
                                << std::endl << std::endl;
                }
                _toRespawn.push(a); //queue for respawning players
                _scene.addToDisable(a->getSceneId()); //queue to remove an Entity from game.
            }

            //since the other object was a bullet, disable it so that disappears
            //after hitting someone
            _scene.addToDisable(b->getSceneId());
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
/// \brief    Preforms actions necessary after contact has ended.
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
/// \brief    Handles the pre solve event for the physics world.
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
/// \brief    Handles the post solve event for the physics world.
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
/// \fn std::unique_ptr<Scene> loadScene(sw::Sandwich& sandwich,
///     const Id& map_id)
///
/// \brief  Loads a scene.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param [in,out] sandwich    The sandwich.
/// \param  map_id              Identifier for the map.
///
/// \return The scene.
////////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Scene> loadScene(sw::Sandwich& sandwich, const Id& map_id)
{
     std::unique_ptr<Scene> s;

     try
     {
          std::string map_name;

          db::StmtCache& cache = sandwich.getStmtCache();
          db::CachedStmt stmt = cache.hold(Id(PBJ_SCENE_SCENE_SQLID_GET_NAME), PBJ_SCENE_SCENE_SQL_GET_NAME);

          stmt.bind(1, map_id.value());
          if (stmt.step())
          {
                map_name = stmt.getText(0);
          }

          s.reset(new Scene());
          db::CachedStmt s2 = cache.hold(Id(PBJ_SCENE_SCENE_SQLID_GET_ENTITIES), PBJ_SCENE_SCENE_SQL_GET_ENTITIES);

          s2.bind(1, map_id.value());
          while (s2.step())
          {
                Id entity_id(s2.getUInt64(0));
                loadEntity(sandwich, map_id, entity_id, *s);
          }
     }
     catch (const db::Db::error& err)
     {
        s.reset(new Scene());
        PBJ_LOG(VWarning) << "Database error while loading scene!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "     Map ID: " << map_id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_NL
                          << "        SQL: " << err.sql() << PBJ_LOG_END;
   }
   catch (const std::exception& err)
   {
       s.reset(new Scene());
       PBJ_LOG(VWarning) << "Exception while loading scene!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "     Map ID: " << map_id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_END;
   }

    return s;
}


////////////////////////////////////////////////////////////////////////////////
/// \fn void saveScene(const Id& sandwich_id, const Id& map_id)
///
/// \brief  Saves a scene.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  sandwich_id Identifier for the sandwich.
/// \param  map_id      Identifier for the map.
////////////////////////////////////////////////////////////////////////////////
void Scene::saveScene(const Id& sandwich_id, const Id& map_id)
{
   try
   {
      std::shared_ptr<sw::Sandwich> sandwich = sw::openWritable(sandwich_id);
      if (!sandwich)
         throw std::runtime_error("Could not open sandwich for writing!");

      db::Db& db = sandwich->getDb();
      db::Transaction transaction(db, db::Transaction::Immediate);
      db::CachedStmt& stmt = sandwich->getStmtCache().hold(Id(PBJ_SCENE_SCENE_SQLID_SET_NAME), PBJ_SCENE_SCENE_SQL_SET_NAME);
      stmt.bind(1, map_id.value());
      stmt.bind(2, getMapName());
      stmt.step();

      db::CachedStmt& s2 = sandwich->getStmtCache().hold(Id(PBJ_SCENE_SCENE_SQLID_CLEAR_ENTITIES), PBJ_SCENE_SCENE_SQL_CLEAR_ENTITIES);
      s2.bind(1, map_id.value());
      s2.step();

      transaction.commit();

      for (auto i(_spawnPoints.begin()), end(_spawnPoints.end()); i != end; ++i)
      {
          i->second->saveEntity(sandwich_id, map_id);
      }

      for (auto i(_terrain.begin()), end(_terrain.end()); i != end; ++i)
      {
          i->second->saveEntity(sandwich_id, map_id);
      }

   }
   catch (const db::Db::error& err)
   {
      PBJ_LOG(VWarning) << "Database error while saving scene!" << PBJ_LOG_NL
                       << "Sandwich ID: " << sandwich_id << PBJ_LOG_NL
                       <<      "Map ID: " << map_id << PBJ_LOG_NL
                       << "  Exception: " << err.what() << PBJ_LOG_NL
                       << "        SQL: " << err.sql() << PBJ_LOG_END;
   }
   catch (const std::runtime_error& err)
   {
      PBJ_LOG(VWarning) << "Exception while saving scene!" << PBJ_LOG_NL
                       << "Sandwich ID: " << sandwich_id << PBJ_LOG_NL
                       <<      "Map ID: " << map_id << PBJ_LOG_NL
                       << "  Exception: " << err.what() << PBJ_LOG_END;
   }
}

} // namespace pbj::scene
} // namespace pbj
