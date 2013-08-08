////////////////////////////////////////////////////////////////////////////////
/// \file Z:\Documents\PBJsimple\include\pbj\scene\scene.h
///
/// \brief Declares the scene class.
////////////////////////////////////////////////////////////////////////////////
#ifndef PBJ_SCENE_SCENE_H_
#define PBJ_SCENE_SCENE_H_

#include <vector>
#include <map>
#include "pbj/_pbj.h"
#include "pbj/scene/ui_root.h"
#include "pbj/scene/entity.h"
#include "pbj/scene/camera.h"

using std::vector;
using std::unordered_map;


namespace pbj {
namespace scene {

////////////////////////////////////////////////////////////////////////////////
/// \class Scene
///
/// \brief The Scene class is mostly an Entity container used in the game.
///
/// \author Peter Bartosch
/// \date 2013-08-08
/// 
/// \details This class contains muliple unordered maps call EntityMap that will
/// 		 map an id number to a given Entity.  The maps are separated by
/// 		 EntityType.  When new types are added, the new maps will have to be
/// 		 added and handled.  Currently the UIRoot for the Scene is left as a
/// 		 global variable, which could probably change but until we have the
/// 		 user interface stuff down I'm leaving it as is.
////////////////////////////////////////////////////////////////////////////////
class Scene
{
public:

	////////////////////////////////////////////////////////////////////////////
	/// \enum EntityType
	///
	/// \brief Values that represent EntityType.
	////////////////////////////////////////////////////////////////////////////
	enum EntityType
	{
		Terrain = 0x01,
		Player = 0x02,
		SpawnPoint = 0x04
	};

    Scene();
	~Scene();

    UIRoot ui;

	void draw();

	U32 addEntity(const Entity&, EntityType);
	void removeEntity(U32, EntityType);

private:

	////////////////////////////////////////////////////////////////////////////
	/// \typedef unordered_map<U32,Entity> EntityMap
	///
	/// \brief An alias for an unordered map used for id->Entity
	////////////////////////////////////////////////////////////////////////////
	typedef unordered_map<U32,Entity> EntityMap;

	U32 _nextEntityId;

	//as we get more Entity types this may have to expand/change entirely
	EntityMap _spawnPoints;
	EntityMap _terrain;
	EntityMap _players;

    Scene(const Scene&);
    void operator=(const Scene&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
