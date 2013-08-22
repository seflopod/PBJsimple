////////////////////////////////////////////////////////////////////////////////
/// \file Z:\Documents\PBJsimple\include\pbj\scene\scene.h
///
/// \brief Declares the scene class.
////////////////////////////////////////////////////////////////////////////////
#ifndef PBJ_SCENE_SCENE_H_
#define PBJ_SCENE_SCENE_H_

#include <vector>
#include <map>
#include <random>
#include <queue>
#include <Box2D/Box2D.h>

#include "pbj/_pbj.h"
#include "pbj/_math.h"
#include "pbj/scene/ui_root.h"
#include "pbj/scene/ui_label.h"
#include "pbj/engine.h"
#include "pbj/scene/entity.h"
#include "pbj/sw/sandwich.h"
#include "be\id.h"

using std::queue;
using std::vector;
using std::unordered_map;
using std::unique_ptr;

namespace pbj {

class Editor;

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
    friend class Editor;
public:
    Scene();
	~Scene();

	void draw();
	void update(F32);
	void physUpdate(F32, I32, I32);
	void initUI();

    void setMapName(const std::string& name);
    const std::string& getMapName() const;

	U32 addEntity(unique_ptr<Entity>&&);
	void removeEntity(U32, Entity::EntityType);
    
	Entity* getBullet(U32);
	Entity* getPlayer(U32);
	Entity* getTerrain(U32);
	Entity* getSpawnPoint(U32);
	Entity* getCamera(U32);
	Entity* getRandomSpawnPoint();

	void setLocalPlayer(U32);
	void clearLocalPlayer();
	Entity* getLocalPlayer();

   void saveScene(const Id& sandwich_id, const Id& map_id);
    
	void setCurrentCamera(U32);
	CameraComponent* getCurrentCamera() const;

	b2World* getWorld() const;
	void addToDisable(U32);

private:

	////////////////////////////////////////////////////////////////////////////
	/// \typedef unordered_map<U32,Entity> EntityMap
	///
	/// \brief An alias for an unordered map used for id->Entity
	////////////////////////////////////////////////////////////////////////////
	typedef unordered_map<U32,unique_ptr<Entity>> EntityMap;

	U32 _nextEntityId;
   U32 _localPlayerId;
	U32 _nextBulletId;
	unique_ptr<b2World> _physWorld;

	queue<Entity*> _toDisable;

	//as we get more Entity types this may have to expand/change entirely
	EntityMap _spawnPoints;
	EntityMap _terrain;
	EntityMap _players;
	EntityMap _bullets;
	EntityMap _cameras;
	EntityMap _others;

	CameraComponent* _curCamera;

	std::ranlux24_base _rnd;  // ranlux? not mersenne twister?

    std::string _name;

    Scene(const Scene&);
    void operator=(const Scene&);

	// Hopeful UI stuffs
	UIRoot ui_;
	UIPanel* eInfo_;
	std::unordered_map<Id, UIElement*> ui_elements;
	UILabel* frame_label_[5];
	Engine& engine_;
};

std::unique_ptr<Scene> loadScene(sw::Sandwich& sandwich, const Id& map_id);


} // namespace pbj::scene
} // namespace pbj

#endif
