////////////////////////////////////////////////////////////////////////////////
/// \file Z:\Documents\PBJsimple\src\pbj\scene\scene.cpp
///
/// \brief Implements the scene class.
////////////////////////////////////////////////////////////////////////////////

#include "pbj/scene/scene.h"

#include <iostream>

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
#define PBJ_SCENE_SCENE_SQLID_GET_NAME       PBJ_SCENE_SCENE_SQL_GET_NAME
#define PBJ_SCENE_SCENE_SQLID_GET_ENTITIES   PBJ_SCENE_SCENE_SQL_GET_ENTITIES
#define PBJ_SCENE_SCENE_SQLID_SET_NAME       PBJ_SCENE_SCENE_SQL_SET_NAME
#define PBJ_SCENE_SCENE_SQLID_CLEAR_ENTITIES PBJ_SCENE_SCENE_SQL_CLEAR_ENTITIES
#else
// TODO: precalculate ids.
#define PBJ_SCENE_SCENE_SQLID_GET_NAME       PBJ_SCENE_SCENE_SQL_GET_NAME
#define PBJ_SCENE_SCENE_SQLID_GET_ENTITIES   PBJ_SCENE_SCENE_SQL_GET_ENTITIES
#define PBJ_SCENE_SCENE_SQLID_SET_NAME       PBJ_SCENE_SCENE_SQL_SET_NAME
#define PBJ_SCENE_SCENE_SQLID_CLEAR_ENTITIES PBJ_SCENE_SCENE_SQL_CLEAR_ENTITIES
#endif

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
{
	_nextEntityId = 1;
    _localPlayerId = U32(-1);
	std::random_device rd;
	_rnd = std::ranlux24_base(rd());
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
/// \author Peter Bartosch
/// \date 2013-08-08
/// 
/// \details This will go through each EntityMap for every drawable EntityType
/// 		 and draw its members.  In a more general setting this would not be
/// 		 handled here, I don't think.
////////////////////////////////////////////////////////////////////////////////
void Scene::draw()
{

	//drawing for debug purposes
	for(EntityMap::iterator it=_spawnPoints.begin();
		it!=_spawnPoints.end();
		it++)
		if(it->second->isDrawable())
			it->second->draw();

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

	//I assume the ui drawing goes like this.
	ui.draw();
}

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
			it->second->update(dt);

	for(EntityMap::iterator it=_players.begin();
		it!=_players.end();
		it++)
		if(it->second->isEnabled())
			it->second->update(dt);

	for(EntityMap::iterator it=_bullets.begin();
		it!=_bullets.end();
		it++)
		if(it->second->isEnabled())
			it->second->update(dt);
}

////////////////////////////////////////////////////////////////////////////////
void Scene::setMapName(const std::string& name)
{
    _name = name;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& Scene::getMapName() const
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
////////////////////////////////////////////////////////////////////////////////
U32 Scene::addEntity(unique_ptr<Entity>&& e)
{
	U32 id = _nextEntityId;
	++_nextEntityId;

	//this seems a little overdone.  Think this could be better.
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
	default:
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
/// 		 Scene::addEntity.
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

void Scene::clearLocalPlayer()
{
    //not perfect, but I think it is unlikely that we will have this many
    //players in the game at a time.
    _localPlayerId = U32(-1);
}

Entity* Scene::getLocalPlayer()
{
    return _players[_localPlayerId].get();
}

Entity* Scene::getBullet(U32 id)
{
	EntityMap::iterator it = _bullets.find(id);
	if(it != _bullets.end())
		return _bullets[id].get();

	return nullptr;
}

Entity* Scene::getPlayer(U32 id)
{
	EntityMap::iterator it = _players.find(id);
	if(it != _players.end())
		return _players[id].get();

	return nullptr;
}

Entity* Scene::getTerrain(U32 id)
{
	EntityMap::iterator it = _terrain.find(id);
	if(it != _terrain.end())
		return _terrain[id].get();

	return nullptr;
}

Entity* Scene::getSpawnPoint(U32 id)
{
	EntityMap::iterator it = _spawnPoints.find(id);
	if(it != _spawnPoints.end())
		return _spawnPoints[id].get();

	return nullptr;
}

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
        s.reset();
        PBJ_LOG(VWarning) << "Database error while loading scene!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "     Map ID: " << map_id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_NL
                          << "        SQL: " << err.sql() << PBJ_LOG_END;
   }
   catch (const std::exception& err)
   {
       s.reset();
       PBJ_LOG(VWarning) << "Exception while loading scene!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "     Map ID: " << map_id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_END;
   }

    return s;
}

void saveScene(const Id& sandwich_id, const Id& map_id)
{
    PBJ_LOG(VWarning) << "Not yet fully implemented!" << PBJ_LOG_END;
}

} // namespace pbj::scene
} // namespace pbj
