////////////////////////////////////////////////////////////////////////////////
/// \file Z:\Documents\PBJsimple\src\pbj\scene\scene.cpp
///
/// \brief Implements the scene class.
////////////////////////////////////////////////////////////////////////////////

#include "pbj/scene/scene.h"

#include "pbj/sw/sandwich_open.h"
#include "be/bed/transaction.h"
#include "pbj/game.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load a scene's name from a sandwich.
#define PBJ_SCENE_SCENE_SQL_GET_NAME "SELECT name FROM sw_maps WHERE id = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load a scene's entities from a sandwich.
#define PBJ_SCENE_SCENE_SQL_GET_ENTITIES "SELECT entity_id FROM sw_map_entities WHERE map_id = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to save a scene's name to a sandwich.
#define PBJ_SCENE_SCENE_SQL_SET_NAME "INSERT OR REPLACE INTO sw_maps " \
                "(id, name) VALUES (?,?)"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to clear a scene's entities in a sandwich.
#define PBJ_SCENE_SCENE_SQL_CLEAR_ENTITIES "DELETE FROM sw_map_entities WHERE map_id = ?"

#ifdef BE_ID_NAMES_ENABLED
#define PBJ_SCENE_SCENE_SQLID_GET_NAME         PBJ_SCENE_SCENE_SQL_GET_NAME
#define PBJ_SCENE_SCENE_SQLID_GET_ENTITIES    PBJ_SCENE_SCENE_SQL_GET_ENTITIES
#define PBJ_SCENE_SCENE_SQLID_SET_NAME         PBJ_SCENE_SCENE_SQL_SET_NAME
#define PBJ_SCENE_SCENE_SQLID_CLEAR_ENTITIES PBJ_SCENE_SCENE_SQL_CLEAR_ENTITIES
#else
// TODO: precalculate ids.
#define PBJ_SCENE_SCENE_SQLID_GET_NAME         PBJ_SCENE_SCENE_SQL_GET_NAME
#define PBJ_SCENE_SCENE_SQLID_GET_ENTITIES    PBJ_SCENE_SCENE_SQL_GET_ENTITIES
#define PBJ_SCENE_SCENE_SQLID_SET_NAME         PBJ_SCENE_SCENE_SQL_SET_NAME
#define PBJ_SCENE_SCENE_SQLID_CLEAR_ENTITIES PBJ_SCENE_SCENE_SQL_CLEAR_ENTITIES
#endif

using pbj::Game;
namespace pbj {
namespace scene {

////////////////////////////////////////////////////////////////////////////////
/// \fn Scene::Scene()
///
/// \brief Initializes a new instance of the Scene class.
///
/// \author Peter Bartosch
/// \date 2013-08-08
////////////////////////////////////////////////////////////////////////////////
Scene::Scene()
    : _curCamera(nullptr),
	  _nextEntityId(1),
      _localPlayerId(U32(-1)),
      engine_(getEngine())
{
    std::random_device rd;
	_rnd = std::ranlux24_base(rd());

	//For testing in a world with no gravity use 0.0f, 0.0f
	_physWorld = std::unique_ptr<b2World>(new b2World(b2Vec2(0.0f, -9.822f)));
	_physWorld->SetAllowSleeping(true);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Scene::~Scene()
///
/// \brief Destroys an instance of the Scene class.
///
/// \author Peter Bartosch
/// \date 2013-08-08
////////////////////////////////////////////////////////////////////////////////
Scene::~Scene()
{
}


////////////////////////////////////////////////////////////////////////////////
/// \fn void Scene::draw()
///
/// \brief Draws the scene
///
/// \author Peter Bartosch / Josh Douglas (UI)
/// \date 2013-08-08
/// 
/// \details This will go through each EntityMap for every drawable EntityType
///          and draw its members.  This will also draw the UI.
////////////////////////////////////////////////////////////////////////////////
void Scene::draw()
{
    if (_curCamera)
	    _curCamera->use();

#ifdef PBJ_EDITOR
	for(EntityMap::iterator it=_spawnPoints.begin();
		it!=_spawnPoints.end();
		it++)
		if(it->second->isDrawable())
			it->second->draw();
#endif

	for(EntityMap::iterator it=_terrain.begin();
		it!=_terrain.end();
		it++)
		if(it->second->isDrawable())
			it->second->draw();

	for(EntityMap::iterator it=_bullets.begin();
		it!=_bullets.end();
		it++)
		if(it->second->isDrawable())
			it->second->draw();

	for(EntityMap::iterator it=_players.begin();
		it!=_players.end();
		it++)
		if(it->second->isDrawable())
			it->second->draw();


    // Updates the UI to display the correct number of kills
	// deaths and health, Josh
    sw::ResourceId id;
    id.sandwich = Id(PBJ_ID_PBJBASE);
    id.resource = Id("std_font");
	
    I32 i=0;
    I32 j=20;
    I32 pad = 5;
    for(EntityMap::iterator it=_players.begin();
        it!=_players.end();
        ++it)
    {
        it->second->draw();
        
        PlayerComponent* p = it->second->getPlayerComponent();
        std::string kills = std::to_string(p->getKills());
        std::string deaths = std::to_string(p->getDeaths());

		//this is going to change when we change how players are id'd
		std::ostringstream osh;
		osh << p->getId().to_useful_string() << std::setprecision(3) << " Health: " <<  ((p->getHealth() / (F32)p->getMaxHealth()) * 100);
		frame_label_[i]->setText(osh.str());

		std::ostringstream oss;
		oss << p->getId().to_useful_string()
			<< " Kills: " << kills
			<< " Deaths: " << deaths;
		frame_kd_[i]->setText(oss.str());
 
		//p = nullptr;
        ++i;
        j+=30;
    }
    ui_.draw();
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
////////////////////////////////////////////////////////////////////////////////
void Scene::physUpdate(F32 dt, I32 velIter, I32 posIter)
{
    _physWorld->Step(dt, velIter, posIter);

    //after all other physics updates, clear forces
    _physWorld->ClearForces();

    //after we're done with the physics step, we need to disable anything queued
    //for it
    while(!_toDisable.empty())
    {
        _toDisable.front()->disable();
        _toDisable.pop();
    }
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
////////////////////////////////////////////////////////////////////////////////
void Scene::update(F32 dt)
{
    for(EntityMap::iterator it=_spawnPoints.begin();
        it!=_spawnPoints.end();
        it++)
        if(it->second->isEnabled())
            it->second->update(dt);

    for(EntityMap::iterator it=_terrain.begin();
        it!=_terrain.end();
        it++)
        if(it->second->isEnabled())
            

    for(EntityMap::iterator it=_players.begin();
    it!=_players.end();
    it++)
        if(it->second->isEnabled())
        {
            it->second->update(dt);
            vec2 pos = it->second->getTransform().getPosition();
            if(it->second->getAudioListener())
            {
                it->second->getAudioListener()->updatePosition();
                it->second->getAudioListener()->updateVelocity();
            }
            it->second->getAudioSource()->updatePosition();
            it->second->getAudioSource()->updateVelocity();
            ivec2 size = getEngine().getWindow()->getContextSize();
            F32 ratio = size.x/(F32)size.y;
            if(pos.y < -Game::grid_height*2 ||
                pos.x < -Game::grid_height*ratio*2 ||
                pos.x > Game::grid_height*ratio*2)
            {
                it->second->getPlayerComponent()->setTimeOfDeath(glfwGetTime());
                it->second->getPlayerComponent()->setDeaths(
                it->second->getPlayerComponent()->getDeaths()+1);
                Game::instance()->respawnPlayer(it->second.get());
            }
        }

    //Move current camera to local player's position
    Entity* player = getLocalPlayer();
    if (player && _curCamera)
    {
        _curCamera->setTargetPosition(player->getTransform().getPosition());
        _curCamera->setTargetVelocity(player->getRigidbody()->getVelocity());
    }

    //Do all camera updates
    for(EntityMap::iterator it=_cameras.begin();
        it!=_cameras.end();
        ++it)
            it->second->update(dt);

    for(EntityMap::iterator it=_bullets.begin();
        it!=_bullets.end();
        it++)
        if(it->second->isEnabled())
            it->second->update(dt);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Scene::setMapName(const std::string& name)
///
/// \brief  Sets map name.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  name    The name.
////////////////////////////////////////////////////////////////////////////////
void Scene::setMapName(const std::string& name) { _name = name; }

////////////////////////////////////////////////////////////////////////////////
/// \fn const std::string& Scene::getMapName() const
///
/// \brief  Gets map name.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \return The map name.
////////////////////////////////////////////////////////////////////////////////
const std::string& Scene::getMapName() const { return _name; }

////////////////////////////////////////////////////////////////////////////////
/// \fn U32 Scene::addEntity(unique_ptr<Entity>&& e)
///
/// \brief Adds an entity to the scene
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \param [in] e A unique pointer to the Entity to add.
////////////////////////////////////////////////////////////////////////////////
U32 Scene::addEntity(unique_ptr<Entity>&& e)
{
    U32 id = _nextEntityId;
    ++_nextEntityId;

    //Place the Entity in the appropriate map depending on its type
    switch(e->getType())
    {
    case Entity::EntityType::Terrain:
        _terrain[id] = std::move(e);
        _terrain[id]->setSceneId(id);
        break;
    case Entity::EntityType::Player:
        _players[id] = std::move(e);
        _players[id]->setSceneId(id);
        break;
    case Entity::EntityType::SpawnPoint:
        _spawnPoints[id] = std::move(e);
        _spawnPoints[id]->setSceneId(id);
        break;
    case Entity::EntityType::Bullet:
        _bullets[id] = std::move(e);
        _bullets[id]->setSceneId(id);
        break;
    case Entity::EntityType::Camera:
        _cameras[id] = std::move(e);
        _cameras[id]->setSceneId(id);
    default:
		_others[id] = std::move(e);
		_cameras[id]->setSceneId(id);
        break;
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
////////////////////////////////////////////////////////////////////////////////
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
/// \fn void Scene::setLocalPlayer(U32 newId)
///
/// \brief  Sets local player.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  newId   Identifier for the new local player.
////////////////////////////////////////////////////////////////////////////////
void Scene::setLocalPlayer(U32 newId)
{
    if(_players.count(newId) == 0)
    {
        PBJ_LOG(pbj::VError) << "Could not find referenced id in player map"
                                << PBJ_LOG_END;
        return;
    }
     
    if(_localPlayerId != U32(-1))
    {
        PBJ_LOG(pbj::VWarning) << "Overwriting already stored local player id"
                                << PBJ_LOG_END;
    }
    _localPlayerId = newId;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Scene::clearLocalPlayer()
///
/// \brief  Clears the local player.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void Scene::clearLocalPlayer()
{
     //not perfect, but I think it is unlikely that we will have this many
     //players in the game at a time.
     _localPlayerId = U32(-1);
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
/// \fn void Scene::initUI()
///
/// \brief  Initialises the user interface.
///
/// \author Josh Douglas
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void Scene::initUI()
{
    sw::ResourceId id;
    id.sandwich = Id(PBJ_ID_PBJBASE);

    UIElement* last_focusable = &ui_.panel;

    eInfo_ = new scene::UIPanel();
    ui_.panel.addElement(std::unique_ptr<UIElement>(eInfo_));

	id.resource = Id("std_font");
	const gfx::TextureFont* font = &engine_.getResourceManager().getTextureFont(id);

    // Draws the UI to the screen with seperate lines for the kills/deaths and health, Josh
    I32 i = 0;
    I32 j = 20;
	I32 k = 40;
    I32 pad = 5;
    for (EntityMap::iterator it=_players.begin(); it!=_players.end(); ++it)
    {
	    //Setup positioning and color
		frame_label_[i] = new UILabel();
		eInfo_->addElement(std::unique_ptr<UIElement>(frame_label_[i]));
		frame_label_[i]->setAlign(scene::UILabel::AlignLeft);
		frame_label_[i]->setFont(font);
		frame_label_[i]->setTextScale(vec2(2, 2));
		frame_label_[i]->setTextColor(it->second->getMaterial()->getColor());

		frame_label_[i]->setDimensions(vec2(200, 10));
		frame_label_[i]->setPosition(vec2(0+pad, 0 + j));

		//Setup positioning and color
		frame_kd_[i] = new UILabel();
		eInfo_->addElement(std::unique_ptr<UIElement>(frame_kd_[i]));
		frame_kd_[i]->setAlign(scene::UILabel::AlignLeft);
		frame_kd_[i]->setFont(font);
		frame_kd_[i]->setTextScale(vec2(2, 2));
		frame_kd_[i]->setTextColor(it->second->getMaterial()->getColor());

		frame_kd_[i]->setDimensions(vec2(200, 10));
		frame_kd_[i]->setPosition(vec2(0+pad, 0 + k));

        ++i;
        j += 50;
		k += 50;
    }
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
/// \fn void Scene::addToDisable(U32 id)
///
/// \brief  Adds an element to the queue for Entity disabling.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  id  The scene id of the Entity to add.
///  
/// \details For some reason I didn't want to pass a pointer, so now we have to
///          look through every EntityMap to find what we're disabling.  Rather
///          than change over to using a pointer, I set up the searching in the
///          order that is most likely for disabling.
////////////////////////////////////////////////////////////////////////////////
void Scene::addToDisable(U32 id)
{
	EntityMap::iterator res;
	res = _bullets.find(id);
	if(res != _bullets.end())
	{
		_toDisable.push(res->second.get());
		return;
	}

	res = _players.find(id);
	if(res != _players.end())
	{
		_toDisable.push(res->second.get());
		return;
	}
	
	res = _others.find(id);
	if(res != _others.end())
	{
		_toDisable.push(res->second.get());
		return;
	}

	res = _terrain.find(id);
	if(res != _terrain.end())
	{
		_toDisable.push(res->second.get());
		return;
	}

	res = _cameras.find(id);
	if(res != _cameras.end())
	{
		_toDisable.push(res->second.get());
		return;
	}

	res = _spawnPoints.find(id);
	if(res != _bullets.end())
	{
		_toDisable.push(res->second.get());
		return;
	}
	
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
                std::unique_ptr<Entity> e = loadEntity(sandwich, map_id, entity_id);
                if (e)
                     s->addEntity(std::move(e));
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
