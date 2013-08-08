////////////////////////////////////////////////////////////////////////////////
/// \file Z:\Documents\PBJsimple\src\pbj\scene\scene.cpp
///
/// \brief Implements the scene class.
////////////////////////////////////////////////////////////////////////////////
#include "pbj/scene/scene.h"

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
	_spawnPoints = EntityMap();
	_terrain = EntityMap();
	_players = EntityMap();
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
		it->second.draw();

	for(EntityMap::iterator it=_players.begin();
		it!=_players.end();
		it++)
		it->second.draw();

	//I assume the ui drawing goes like this.
	ui.draw();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn U32 Scene::addEntity(const Entity& e, EntityType et)
///
/// \brief Adds an Entity to the EntityMap for its EntityType
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \param e  A reference to the Entity to add to the scene.
/// \param et The type of Entity.
///
/// \return The id of the Entity in the EntityMap for its type.
/// 
/// \sa Entity, Scene::EntityType
////////////////////////////////////////////////////////////////////////////////
U32 Scene::addEntity(const Entity& e, EntityType et)
{
	U32 ret = _nextEntityId;

	//this seems a little overdone.  Think this could be better.
	switch(et)
	{
	case Terrain:
		_terrain[_nextEntityId].init();
		_terrain[_nextEntityId].setTransform(*(e.getTransform()));
		_terrain[_nextEntityId].setTextureId(e.getTextureId());
		_nextEntityId++;
		break;
	case Player:
		_players[_nextEntityId].init();
		_players[_nextEntityId].setTransform(*(e.getTransform()));
		_players[_nextEntityId].setTextureId(e.getTextureId());
		_nextEntityId++;
		break;
	case SpawnPoint:
		_spawnPoints[_nextEntityId].init();
		_spawnPoints[_nextEntityId].setTransform(*(e.getTransform()));
		_spawnPoints[_nextEntityId].setTextureId(e.getTextureId());
		_nextEntityId++;
		break;
	default:
		break;
	}

	return ret;
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
void Scene::removeEntity(U32 id, EntityType et)
{
	switch(et)
	{
	case Terrain:
		_terrain.erase(id);
		break;
	case Player:
		_players.erase(id);
		break;
	case SpawnPoint:
		_spawnPoints.erase(id);
		break;
	default:
		break;
	}
}

} // namespace pbj::scene
} // namespace pbj
