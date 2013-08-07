///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::scene::Scene functions.

#include "pbj/scene/scene.h"

namespace pbj {
namespace scene {

Scene::Scene()
{
	_nextEntityId = 0;
	_entities = unordered_map<U32,Entity>();
}

Scene::~Scene()
{
}

void Scene::draw()
{
	for(unordered_map<U32,Entity>::iterator it = _entities.begin();
		it != _entities.end();
		++it)
	{
		it->second.draw();
	}

	//I assume the ui drawing goes like this.
	ui.draw();
}

//returns the id of the entity in the map
U32 Scene::addEntity(Entity& e)
{
	U32 ret = _nextEntityId;
	//this seems a little overdone.  Think this could be better.
	_entities[_nextEntityId].init();
	_entities[_nextEntityId].setTransform(*(e.getTransform()));
	_entities[_nextEntityId].setTextureId(e.getTextureId());
	_nextEntityId++;
	return ret;
}

void Scene::removeEntity(U32 id)
{
	_entities.erase(id);
}

} // namespace pbj::scene
} // namespace pbj
