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
using std::unique_ptr;

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

    Scene();
	~Scene();

    UIRoot ui;

	void draw();
	void update(F32);

	U32 addEntity(unique_ptr<Entity>&&);
	void removeEntity(U32, Entity::EntityType);
    
	Entity* getBullet(U32);
	Entity* getPlayer(U32);

	void setLocalPlayer(U32);
	void clearLocalPlayer();
	Entity* getLocalPlayer();
private:

	////////////////////////////////////////////////////////////////////////////
	/// \typedef unordered_map<U32,Entity> EntityMap
	///
	/// \brief An alias for an unordered map used for id->Entity
	////////////////////////////////////////////////////////////////////////////
	typedef unordered_map<U32,unique_ptr<Entity>> EntityMap;

	U32 _nextEntityId;
    U32 _localPlayerId;
	//as we get more Entity types this may have to expand/change entirely
	EntityMap _spawnPoints;
	EntityMap _terrain;
	EntityMap _players;
	EntityMap _bullets;
    Scene(const Scene&);
    void operator=(const Scene&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
