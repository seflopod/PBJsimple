////////////////////////////////////////////////////////////////////////////////
/// \file   pbj\scene\scene.h
///
/// \brief  Declares the scene class.
#ifndef PBJ_SCENE_SCENE_H_
#define PBJ_SCENE_SCENE_H_

#include <vector>
#include <map>
#include <random>
#include <queue>
#include <Box2D/Box2D.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>

#include "pbj/_pbj.h"
#include "pbj/_math.h"
#include "pbj/scene/ui_root.h"
#include "pbj/scene/ui_label.h"
#include "pbj/engine.h"
#include "pbj/scene/entity.h"
#include "pbj/sw/sandwich.h"
#include "be\id.h"

namespace pbj {

class Editor;

namespace scene {

////////////////////////////////////////////////////////////////////////////////
/// \class  Scene
///
/// \brief  The Scene class is mostly an Entity container used in the game.
///
/// \author Peter Bartosch
/// \date   2013-08-08
///
/// \details This class contains muliple unordered maps call EntityMap that will
///         map an id number to a given Entity.  The maps are separated by
///         EntityType.  When new types are added, the new maps will have to be
///         added and handled.  Currently the UIRoot for the Scene is left as a
///         global variable, which could probably change but until we have the
///         user interface stuff down I'm leaving it as is.
class Scene : public b2ContactListener
{
    friend class Editor;
    friend void loadEntity(sw::Sandwich&, const Id&, const Id&, Scene&);
public:
    Scene();
    void makeHud();

    void draw();
    void update(F32 delta_t);
    void physUpdate(F32 delta_t);

    b2World* getWorld();

    void setName(const std::string& name);
    const std::string& getName() const;

    U32 addEntity(std::unique_ptr<Entity>&& entity);
    void removeEntity(U32 id, Entity::EntityType type);

    void initBulletRing();
    U32 makeBullet();
    U32 makePlayer(const std::string& name, const vec2& position, bool local_player);
    U32 makeTerrain(const vec2& position, const vec2& scale, F32 rotation, const gfx::Material* material);
    U32 makeSpawnPoint(const vec2& position);
    U32 makeCamera();

    Entity* getBullet(U32 id);
    Entity* getPlayer(U32 id);
    Entity* getLocalPlayer();
    Entity* getTerrain(U32 id);
    Entity* getSpawnPoint(U32 id);
    Entity* getRandomSpawnPoint();
    Entity* getCamera(U32 id);
    void setCurrentCamera(U32 id);
    Entity* getCurrentCamera();

    void spawnBullet(const vec2& position, const vec2& velocity, Entity* shooter);
    void disableBullet(U32 id);
    void respawnPlayer(U32 id);

    void saveScene(const Id& sandwich_id, const Id& map_id);

private:
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* manifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

    static const I32 _physVelocityIterations = 8;
    static const I32 _physPositionIterations = 3;
    static const size_t _maxBullets = 500;

    ////////////////////////////////////////////////////////////////////////////
    /// \typedef unordered_map<U32,Entity> EntityMap
    ///
    /// \brief An alias for an unordered map used for id->Entity
    typedef std::unordered_map<U32, std::unique_ptr<Entity> > EntityMap;

    Engine& _engine;
    Window& _window;

    sw::ResourceManager _resources;
    const gfx::Material* _bulletMaterial;
    const gfx::Material* _spawnPointMaterial;


    std::mt19937 _prng;
    b2World _physWorld;

    std::string _name;

    //as we get more Entity types this may have to expand/change entirely
    EntityMap _spawnPoints;
    EntityMap _terrain;
    EntityMap _players;
    EntityMap _bullets;
    EntityMap _cameras;
    EntityMap _others;

    U32 _nextEntityId;

    U32 _curCameraId;
    U32 _localPlayerId;
    U32 _nextBulletId;

    std::vector<Entity*> _toDisable;
    std::queue<Entity*> _toRespawn;

    U32 _bulletRing[_maxBullets];
    I32 _curRingIndex;

    UIRoot _ui;
    std::unordered_map<Id, UIElement*> _ui_elements;
    UILabel* _player_health_lbl[5];
    UILabel* _player_kd_lbl[5];

    // Disable copy/assignment
    Scene(const Scene&);
    void operator=(const Scene&);
};

std::unique_ptr<Scene> loadScene(sw::Sandwich& sandwich, const Id& map_id);
void loadEntity(sw::Sandwich& sandwich, const Id& map_id, const Id& entity_id, Scene& scene);
void saveEntity(const Id& sandwich_id, const Id& map_id, Entity* entity);

} // namespace pbj::scene
} // namespace pbj

#endif
