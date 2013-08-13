////////////////////////////////////////////////////////////////////////////////
/// \file Z:\Documents\PBJsimple\src\pbj\scene\scene.cpp
///
/// \brief Implements the scene class.
////////////////////////////////////////////////////////////////////////////////

#include "pbj/scene/scene.h"

#include <iostream>

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
	_nextEntityId = 0;
    _localPlayerId = U32(-1);
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

	for(EntityMap::iterator it=_terrain.begin();
		it!=_terrain.end();
		it++)
		it->second->draw();

	for(EntityMap::iterator it=_players.begin();
		it!=_players.end();
		it++)
		it->second->draw();

	//I assume the ui drawing goes like this.
	ui.draw();
}

void Scene::update()
{
	for(EntityMap::iterator it=_spawnPoints.begin();
		it!=_spawnPoints.end();
		it++)
		if(it->second->getRigidbody())
			it->second->getRigidbody()->updateOwnerTransform();

	for(EntityMap::iterator it=_terrain.begin();
		it!=_terrain.end();
		it++)
		if(it->second->getRigidbody())
			it->second->getRigidbody()->updateOwnerTransform();

	for(EntityMap::iterator it=_players.begin();
		it!=_players.end();
		it++)
		if(it->second->getRigidbody())
			it->second->getRigidbody()->updateOwnerTransform();
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
void Scene::addEntity(unique_ptr<Entity>&& e)
{
	U32 ret = _nextEntityId;

	//this seems a little overdone.  Think this could be better.
	switch(e->getType())
	{
	case Entity::EntityType::Terrain:
		_terrain[_nextEntityId] = std::move(e);
		_terrain[_nextEntityId]->setSceneId(_nextEntityId);
		_nextEntityId++;
		break;
	case Entity::EntityType::Player:
		_players[_nextEntityId] = std::move(e);
		_players[_nextEntityId]->setSceneId(_nextEntityId);
		_nextEntityId++;
		break;
	case Entity::EntityType::SpawnPoint:
		_spawnPoints[_nextEntityId] = std::move(e);
		_players[_nextEntityId]->setSceneId(_nextEntityId);
		_nextEntityId++;
		break;
	default:
		break;
	}
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

} // namespace pbj::scene
} // namespace pbj
