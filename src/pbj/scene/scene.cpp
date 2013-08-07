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
	_entities = map<U32,Entity>();
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
		it->second->draw();
	}
}

} // namespace pbj::scene
} // namespace pbj
