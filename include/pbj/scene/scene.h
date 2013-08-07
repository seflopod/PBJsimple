///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene.h
/// \author Benjamin Crist
///
/// \brief  pbj::scene::Scene class header.

#ifndef PBJ_SCENE_SCENE_H_
#define PBJ_SCENE_SCENE_H_

#include <vector>
#include <map>
#include "pbj/_pbj.h"
#include "pbj/scene/ui_root.h"
#include "pbj/scene/entity.h"
#include "pbj/gfx/batcher.h"

using std::vector;
using std::unordered_map;


namespace pbj {
namespace scene {

class Scene
{
public:
    Scene();
	~Scene();

    UIRoot ui;

	void draw();

	U32 addEntity(Entity);
	void removeEntity(U32);

private:
	U32 _nextEntityId;

	unordered_map<U32,Entity> _entities;

    Scene(const Scene&);
    void operator=(const Scene&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
