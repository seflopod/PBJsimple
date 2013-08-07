///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene.h
/// \author Benjamin Crist
///
/// \brief  pbj::scene::Scene class header.

#ifndef PBJ_SCENE_SCENE_H_
#define PBJ_SCENE_SCENE_H_

#include <vector>
#include "pbj/_pbj.h"
#include "pbj/scene/ui_root.h"
#include "pbj/scene/entity.h"
#include "pbj/gfx/batcher.h"

using std::vector;

namespace pbj {
namespace scene {

class Scene
{
public:
    Scene();
	~Scene()
	{}

    UIRoot ui;
	vector<Entity> entities;

	void setupRender(gfx::Batcher* const b)
	{
		for(U32 i=0;i<entities.size();++i)
		{
			entities[i].generateBatcherTask();
			b->submit(*(entities[i].getBatcherTask()));
		}
	}
private:

    Scene(const Scene&);
    void operator=(const Scene&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
