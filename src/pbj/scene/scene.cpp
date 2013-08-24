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
#define PBJSQL_LOAD_SCENE \
    "SELECT name FROM sw_maps WHERE id = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load a scene's entities from a sandwich.
#define PBJSQL_GET_ENTITIES \
    "SELECT entity_id FROM sw_map_entities WHERE map_id = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to save a scene's name to a sandwich.
#define PBJSQL_SAVE_SCENE \
    "INSERT OR REPLACE INTO sw_maps (id, name) VALUES (?,?)"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to clear a scene's entities in a sandwich.
#define PBJSQL_CLEAR_ENTITIES \
    "DELETE FROM sw_map_entities WHERE map_id = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load an entity from a sandwich.
#define PBJSQL_LOAD_ENTITY \
    "SELECT entity_type, rotation, " \
    "pos_x, pos_y, scale_x, scale_y, material_sw_id, material_id " \
    "FROM sw_map_entities WHERE map_id = ? AND entity_id = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to save an entity to a sandwich.
#define PBJSQL_SAVE_ENTITY \
    "INSERT INTO sw_map_entities " \
    "(map_id, entity_id, entity_type, rotation, " \
    "pos_x, pos_y, scale_x, scale_y, material_sw_id, material_id) " \
    "VALUES (?,?,?,?,?,?,?,?,?,?)"

#ifdef BE_ID_NAMES_ENABLED
#define PBJSQLID_LOAD_SCENE      PBJSQL_LOAD_SCENE
#define PBJSQLID_GET_ENTITIES    PBJSQL_GET_ENTITIES
#define PBJSQLID_SAVE_SCENE      PBJSQL_SAVE_SCENE
#define PBJSQLID_CLEAR_ENTITIES  PBJSQL_CLEAR_ENTITIES
#define PBJSQLID_LOAD_ENTITY     PBJSQL_LOAD_ENTITY
#define PBJSQLID_SAVE_ENTITY     PBJSQL_SAVE_ENTITY
#else
// TODO: precalculate ids.
#define PBJSQLID_LOAD_SCENE      PBJSQL_LOAD_SCENE
#define PBJSQLID_GET_ENTITIES    PBJSQL_GET_ENTITIES
#define PBJSQLID_SAVE_SCENE      PBJSQL_SAVE_SCENE
#define PBJSQLID_CLEAR_ENTITIES  PBJSQL_CLEAR_ENTITIES
#define PBJSQLID_LOAD_ENTITY     PBJSQL_LOAD_ENTITY
#define PBJSQLID_SAVE_ENTITY     PBJSQL_SAVE_ENTITY
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
    _bulletMaterial = &_resources.getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("bullet")));
    _spawnPointMaterial = &_resources.getMaterial(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("spawnpoint")));

    _physWorld.SetAllowSleeping(true);
    _physWorld.SetContactListener(this);
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
        osh << p->getName()
            << std::setprecision(3)
            << " Health: "
            << ((p->getHealth() / (F32)p->getMaxHealth()) * 100);
        _player_health_lbl[player]->setText(osh.str());

        std::ostringstream oss;
        oss << p->getName()
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
/// \fn b2World* Scene::getWorld()
///
/// \brief Gets the physics world.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return A pointer to the b2World for Box2D simulations.
////////////////////////////////////////////////////////////////////////////////
b2World* Scene::getWorld()
{
    return &_physWorld;
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
U32 Scene::addEntity(std::unique_ptr<Entity>&& e)
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
void Scene::initBulletRing()
{
    //make all the bullets we'll ever need
    for (size_t i = 0; i < _maxBullets; ++i)
    {
        U32 id = _bulletRing[i] = makeBullet();

        Entity* e = getBullet(id);

        e->getRigidbody()->setBullet(false);
        e->disable();
    }

    _curRingIndex = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    U32 Scene::makeBullet()
///
/// \brief    Makes a bullet.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    A pointer to an Entity that has all the characteristics of a
///            bullet.
U32 Scene::makeBullet()
{
    Entity* e = new Entity();
    e->setType(Entity::EntityType::Bullet);
    addEntity(std::unique_ptr<Entity>(e));

    e->getTransform().setScale(0.5f, 0.5f);
    e->addBulletComponent();
    e->setMaterial(_bulletMaterial);
    e->setShape(new gfx::ShapeTriangle());
    e->addRigidbody(physics::Rigidbody::BodyType::Dynamic, &_physWorld);
    e->getRigidbody()->setBullet(true);
    e->getRigidbody()->setActive(false);

    return e->getSceneId();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    U32 Scene::makePlayer(const std::string& name, const vec2& position, bool local_player)
///
/// \brief    Makes the player.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    A pointer to an Entity that has all the characteristics of a
///            player.
U32 Scene::makePlayer(const std::string& name, const vec2& position, bool local_player)
{
    Entity* e = new Entity();
    e->setType(Entity::Player);
    addEntity(std::unique_ptr<Entity>(e));
    U32 id = e->getSceneId();

    e->getTransform().setPosition(position);
    e->getTransform().setScale(vec2(1.0f, 2.0f));
    e->setShape(new gfx::ShapeSquare());
    e->enableDraw();
    
    e->addRigidbody(physics::Rigidbody::BodyType::Dynamic, &_physWorld);
    e->getRigidbody()->setFixedRotation(true);

    e->addPlayerComponent(name);
    e->getPlayerComponent()->setMaxAmmo(30);
    e->getPlayerComponent()->setAmmoRemaining(30);

    e->addAudioSource();
    e->getAudioSource()->addBuffer("fire", _engine.getResourceManager().getSound(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("wpnfire"))));
    e->getAudioSource()->addBuffer("dmg", _engine.getResourceManager().getSound(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("dmg"))));
    e->getAudioSource()->addBuffer("death", _engine.getResourceManager().getSound(sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("death"))));
    e->getAudioSource()->updatePosition();
    e->getAudioSource()->updateVelocity();

    if (local_player)
        _localPlayerId = id;
    else
        e->addAIComponent();

    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 Scene::makeTerrain(const vec2& position, const vec2& scale, F32 rotation, const gfx::Material* material)
{
    Entity* e = new Entity();
    e->setType(Entity::Terrain);
    addEntity(std::unique_ptr<Entity>(e));
    U32 id = e->getSceneId();

    e->getTransform().setPosition(position);
    e->getTransform().setScale(scale);
    e->getTransform().setRotation(rotation);
    e->setShape(new gfx::ShapeSquare());
    e->setMaterial(material);
    e->enableDraw();

    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 Scene::makeSpawnPoint(const vec2& position)
{
    Entity* e = new Entity();
    e->setType(Entity::SpawnPoint);
    addEntity(std::unique_ptr<Entity>(e));
    U32 id = e->getSceneId();

    e->getTransform().setPosition(position);
    e->getTransform().setScale(vec2(1, 2));
    e->getTransform().setRotation(0);
    e->setShape(new gfx::ShapeSquare());
    e->setMaterial(_spawnPointMaterial);
#ifdef PBJ_EDITOR
    // When in the editor, draw spawnpoints no matter what.
    e->enableDraw();
#endif

    return id;
}

////////////////////////////////////////////////////////////////////////////////
U32 Scene::makeCamera()
{
    Entity* e = new Entity();
    e->setType(Entity::Camera);
    addEntity(std::unique_ptr<Entity>(e));
    U32 id = e->getSceneId();

    e->getTransform().setPosition(vec2());
    e->getTransform().setScale(vec2(1, 1));
    e->getTransform().setRotation(0);

    e->addAudioListener();
    e->addCamera();

    return id;
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
Entity* Scene::getPlayer(U32 id)
{
    EntityMap::iterator it = _players.find(id);
    if(it != _players.end())
        return _players[id].get();

    return nullptr;
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
Entity* Scene::getLocalPlayer()
{
     if (_localPlayerId == U32(-1))
          return nullptr;

     return _players[_localPlayerId].get();
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
/// \author Ben Crist
/// \date   2013-08-22
///
/// \return A pointer to the Entity of a random element in the EntityMap for
///         spawn points.
Entity* Scene::getRandomSpawnPoint()
{
    std::uniform_int_distribution<I32> dist(1, _spawnPoints.size());
    I32 count = dist(_prng);
    for (auto i(_spawnPoints.begin()), end(_spawnPoints.end()); i != end; ++i)
    {
        if (--count == 0)
        {
            return i->second.get();
        }
    }

    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Entity* Scene::getCamera(U32 id)
///
/// \brief  Gets a camera.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  id  The scene id of the camera to get.
///
/// \return null if it fails, else the camera.
Entity* Scene::getCamera(U32 id)
{
    EntityMap::iterator it = _cameras.find(id);
    if(it != _cameras.end())
        return _cameras[id].get();

    return nullptr;
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
void Scene::setCurrentCamera(U32 id)
{
    Entity* e = getCamera(id);
    _curCameraId = e ? e->getSceneId() : U32(-1);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Entity* Scene::getCurrentCamera() const
///
/// \brief  Gets current camera.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return A pointer to the camera entity that is used as the current camera.
Entity* Scene::getCurrentCamera()
{
    return getCamera(_curCameraId);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Scene::spawnBullet(const vec2& position, const vec2& velocity)
///
/// \brief    Spawn bullet.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    position    The position of the bullet.
/// \param    velocity    The velocity for the bullet to have.
void Scene::spawnBullet(const vec2& position, const vec2& velocity, Entity* shooter)
{
    Entity* bullet = getBullet(_bulletRing[_curRingIndex++]);
    if(_curRingIndex >= _maxBullets)
        _curRingIndex -= _maxBullets;

    bullet->getTransform().setPosition(position);
    bullet->getTransform().updateOwnerRigidbody();
    bullet->getRigidbody()->setVelocity(velocity);
    bullet->getRigidbody()->setAngularVelocity(6.28318f);
    bullet->getBulletComponent()->setShooter(shooter);
    bullet->enable();
    
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Scene::disableBullet(U32 id)
///
/// \brief  Disables a bullet.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  id    The ID of the bullet to disable at the end of the physics
///               loop.
void Scene::disableBullet(U32 id)
{
    Entity* e = getBullet(id);

    if (e)
        _toDisable.push_back(e);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Scene::respawnPlayer(U32 id)
///
/// \brief    Respawn player.
///
/// \author    Peter Bartosch
/// \date    2013-08-14
///
/// \param  id  The ID of the player to respawn.
void Scene::respawnPlayer(U32 id)
{
    Entity* e = getPlayer(id);
    
    if (e)
    {
        _toDisable.push_back(e);
        _toRespawn.push(e);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Scene::BeginContact(b2Contact* contact)
///
/// \brief    Handles the beginning of contact between two rigidbodies.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param [in,out]    contact    The contact data.
void Scene::BeginContact(b2Contact* contact)
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

            // Only let the player jump again if they hit the terrain from the top(ish)
            b2WorldManifold worldManifold;
            contact->GetWorldManifold(&worldManifold);
            if (worldManifold.normal.y >= 0)
                p->enableJump();

            break;
        }

        case Entity::EntityType::Bullet:
        {   //First we need to do damage calculations and then we need to check
            //for death.
            scene::PlayerComponent* q = b->getBulletComponent()->getShooter()->getPlayerComponent();

            //being a little too fancy with damage.  Base damage taken on
            //velocity of bullet.
            I32 dmg = 50 + I32(glm::length2(b->getRigidbody()->getVelocity()) / 12.0f);

            //scoring stuff.  Do damage to the hit party, make sure the shooter
            //knows that its bullet did the damage.
            p->takeDamage(dmg);
            q->setBulletsHit(q->getBulletsHit()+1);

            if(p->isDead())
            {   //If the player is dead we need to check how that death happened
                //then make sure it knows it died and ready the respawn
                p->setDeaths(p->getDeaths()+1);
                p->setTimeOfDeath(glfwGetTime());

                if(p == q)
                {   //self-kill
                    std::cerr << p->getName() << " suicided ("
                              << p->getDeaths() << ")" << std::endl << std::endl;
                }
                else
                {
                    //make sure the shooter gets credit for the kill
                    q->setKills(q->getKills()+1);
                    std::cerr << p->getName() << " died ("
                                << p->getDeaths() << ")" << std::endl << q->getName()
                                << " got the kill (" << q->getKills() << ")"
                                << std::endl << std::endl;
                }
                _toRespawn.push(a); //queue for respawning players
                _toDisable.push_back(a); //queue to remove an Entity from game.
            }

            //since the other object was a bullet, disable it so that disappears
            //after hitting someone
            _toDisable.push_back(b);
            break;
        }
        default:
            break;
        }

    }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Scene::EndContact(b2Contact* contact)
///
/// \brief    Preforms actions necessary after contact has ended.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param [in,out]    contact    If non-null, the contact.
////////////////////////////////////////////////////////////////////////////////
void Scene::EndContact(b2Contact* contact)
{
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Scene::PreSolve(b2Contact* contact, const b2Manifold* manifold)
///
/// \brief    Handles the pre solve event for the physics world.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param [in,out]    contact    If non-null, the contact.
/// \param    manifold            The manifold.
////////////////////////////////////////////////////////////////////////////////
void Scene::PreSolve(b2Contact* contact, const b2Manifold* manifold)
{
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    void Scene::PostSolve(b2Contact* contact,
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
void Scene::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
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
          db::CachedStmt stmt = cache.hold(Id(PBJSQLID_LOAD_SCENE), PBJSQL_LOAD_SCENE);

          stmt.bind(1, map_id.value());
          if (stmt.step())
          {
                map_name = stmt.getText(0);
          }

          s.reset(new Scene());
          db::CachedStmt s2 = cache.hold(Id(PBJSQLID_GET_ENTITIES), PBJSQL_GET_ENTITIES);

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
/// \fn void loadEntity(sw::Sandwich& sandwich, const Id& map_id, const Id& entity_id)
///
/// \brief  Loads an entity and adds it to the provided scene.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param [in,out] sandwich    The sandwich.
/// \param  map_id              Identifier for the map.
/// \param  entity_id           Identifier for the entity.
////////////////////////////////////////////////////////////////////////////////
void loadEntity(sw::Sandwich& sandwich, const Id& map_id, const Id& entity_id, Scene& scene)
{
    try
    {
        db::StmtCache& cache = sandwich.getStmtCache();
        db::CachedStmt stmt = cache.hold(Id(PBJSQLID_LOAD_ENTITY), PBJSQL_LOAD_ENTITY);

        stmt.bind(1, map_id.value());
        stmt.bind(2, entity_id.value());
        if (stmt.step())
        {
            Entity::EntityType type = static_cast<Entity::EntityType>(stmt.getUInt(0));
            F32 rotation = float(stmt.getDouble(1));
            vec2 position = vec2(float(stmt.getDouble(2)), float(stmt.getDouble(3)));
            vec2 scale = vec2(float(stmt.getDouble(4)), float(stmt.getDouble(5)));

            const gfx::Material* material = nullptr;
            if (stmt.getType(7) != SQLITE_NULL)
            {
                sw::ResourceId material_id;
                material_id.sandwich = (stmt.getType(6) == SQLITE_NULL)
                                        ? sandwich.getId()
                                        : Id(stmt.getUInt64(6));
                material_id.resource = Id(stmt.getUInt64(7));
                material = &scene._resources.getMaterial(material_id);
            }

            switch (type)
            {
            case Entity::Terrain:
                scene.makeTerrain(position, scale, rotation, material);
                break;

            case Entity::SpawnPoint:
                scene.makeSpawnPoint(position);
                break;

            case Entity::Camera:
                // TODO: camera loading
                // U32 makeCamera();
                break;

            case Entity::Player:
                // TODO: player loading
                // U32 makePlayer(const std::string& name, const vec2& position, bool local_player);
                break;

            case Entity::Bullet:
                // TODO: bullet loading
                // U32 makeBullet();
                break;

            default:
                break;
            }
        }
        else
            throw std::runtime_error("Entity not found!");
    }
    catch (const db::Db::error& err)
    {
        PBJ_LOG(VWarning) << "Database error while loading entity!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "     Map ID: " << map_id << PBJ_LOG_NL
                          << "  Entity ID: " << entity_id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_NL
                          << "        SQL: " << err.sql() << PBJ_LOG_END;
   }
   catch (const std::exception& err)
   {
      PBJ_LOG(VWarning) << "Exception while loading entity!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "     Map ID: " << map_id << PBJ_LOG_NL
                          << "  Entity ID: " << entity_id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_END;
   }
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
      db::CachedStmt& stmt = sandwich->getStmtCache().hold(Id(PBJSQLID_SAVE_SCENE), PBJSQL_SAVE_SCENE);
      stmt.bind(1, map_id.value());
      stmt.bind(2, getName());
      stmt.step();

      db::CachedStmt& s2 = sandwich->getStmtCache().hold(Id(PBJSQLID_CLEAR_ENTITIES), PBJSQL_CLEAR_ENTITIES);
      s2.bind(1, map_id.value());
      s2.step();

      transaction.commit();

      for (auto i(_spawnPoints.begin()), end(_spawnPoints.end()); i != end; ++i)
      {
          saveEntity(sandwich_id, map_id, i->second.get());
      }

      for (auto i(_terrain.begin()), end(_terrain.end()); i != end; ++i)
      {
          saveEntity(sandwich_id, map_id, i->second.get());
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


////////////////////////////////////////////////////////////////////////////////
/// \fn void saveEntity(const Id& sandwich_id, const Id& map_id)
///
/// \brief  Saves an entity.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  sandwich_id Identifier for the sandwich.
/// \param  map_id      Identifier for the map.
////////////////////////////////////////////////////////////////////////////////
void saveEntity(const Id& sandwich_id, const Id& map_id, Entity* entity)
{
    if (!entity)
        return;

    try
    {
      std::shared_ptr<sw::Sandwich> sandwich = sw::openWritable(sandwich_id);
      if (!sandwich)
         throw std::runtime_error("Could not open sandwich for writing!");

      db::Db& db = sandwich->getDb();
      db::Transaction transaction(db, db::Transaction::Immediate);

      db::CachedStmt& stmt = sandwich->getStmtCache().hold(Id(PBJSQLID_SAVE_ENTITY), PBJSQL_SAVE_ENTITY);
      stmt.bind(1, map_id.value());
      stmt.bind(2, entity->getSceneId());
      stmt.bind(3, entity->getType());
      stmt.bind(4, entity->getTransform().getRotation());
      stmt.bind(5, entity->getTransform().getPosition().x);
      stmt.bind(6, entity->getTransform().getPosition().y);
      stmt.bind(7, entity->getTransform().getScale().x);
      stmt.bind(8, entity->getTransform().getScale().y);
      if (entity->getMaterial())
      {
          stmt.bind(9, entity->getMaterial()->getId().sandwich.value());
          stmt.bind(10, entity->getMaterial()->getId().resource.value());
      }
      else
      {
          stmt.bind(9);
          stmt.bind(10);
      }
      stmt.step();

      transaction.commit();
   }
   catch (const db::Db::error& err)
   {
      PBJ_LOG(VWarning) << "Database error while saving scene!" << PBJ_LOG_NL
                       << "Sandwich ID: " << sandwich_id << PBJ_LOG_NL
                       << "     Map ID: " << map_id << PBJ_LOG_NL
                       << "  Entity ID: " << entity->getSceneId() << PBJ_LOG_NL
                       << "  Exception: " << err.what() << PBJ_LOG_NL
                       << "        SQL: " << err.sql() << PBJ_LOG_END;
   }
   catch (const std::runtime_error& err)
   {
      PBJ_LOG(VWarning) << "Exception while saving scene!" << PBJ_LOG_NL
                       << "Sandwich ID: " << sandwich_id << PBJ_LOG_NL
                       << "     Map ID: " << map_id << PBJ_LOG_NL
                       << "  Entity ID: " << entity->getSceneId() << PBJ_LOG_NL
                       << "  Exception: " << err.what() << PBJ_LOG_END;
   }
}

} // namespace pbj::scene
} // namespace pbj
