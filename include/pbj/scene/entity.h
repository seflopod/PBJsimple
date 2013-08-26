////////////////////////////////////////////////////////////////////////////////
/// \file    pbj\scene\entity.h
///
/// \brief    Declares the entity class.
#ifndef PBJ_SCENE_ENTITY_H_
#define PBJ_SCENE_ENTITY_H_

#include <Box2D/Box2D.h>

#include "pbj/_pbj.h"
#include "pbj/gfx/texture.h"
#include "pbj/gfx/shape_square.h"
#include "pbj/gfx/shape_triangle.h"
#include "pbj/gfx/material.h"
#include "pbj/physics/rigidbody.h"
#include "pbj/scene/transform.h"
#include "pbj/scene/player_component.h"
#include "pbj/scene/ai_component.h"
#include "pbj/scene/bullet_component.h"
#include "pbj/scene/camera_component.h"
#include "pbj/audio/source.h"
#include "pbj/audio/listener.h"
#include "pbj/sw/sandwich.h"
#include "be/id.h"

#include <memory>

namespace pbj {
namespace scene {

class Scene;

////////////////////////////////////////////////////////////////////////////
/// \class  Entity
///
/// \brief  Entity keeps track of all data relating to an object within a
///         scene.
///
/// \author Peter Bartosch
/// \date   2013-08-05
class Entity
{
public:
    ////////////////////////////////////////////////////////////////////////
    /// \enum EntityType
    ///
    /// \brief Values that represent EntityType.
    enum EntityType
    {
        Terrain = 0x01,
        Player = 0x02,
        SpawnPoint = 0x04,
        Bullet = 0x08,
        Camera = 0xF0
    };

    Entity();
    ~Entity();

    void update(F32);
    void draw();

    //accessors, these will expand as the class gains more component
    //possiblities
    Transform& getTransform();
    void setTransform(const Transform&);

    gfx::Shape* getShape() const;
    void setShape(gfx::Shape*);

    const gfx::Material* getMaterial();
    void setMaterial(const gfx::Material*);

    GLuint getTextureId() const;
    void setTextureId(const GLuint);

    void addRigidbody(physics::Rigidbody::BodyType, b2World*);
    physics::Rigidbody* getRigidbody() const;

    void addPlayerComponent(const std::string& name);
    PlayerComponent* getPlayerComponent() const;

    void addAIComponent();
    AIComponent* getAIComponent() const;
    U32 getSceneId() const;
    void setSceneId(U32);

    void addBulletComponent();
    BulletComponent* getBulletComponent() const;

    void addAudioListener();
    audio::Listener* getAudioListener() const;

    void addAudioSource();
    audio::Source* getAudioSource() const;

    void addCamera();
    CameraComponent* getCamera() const;

    EntityType getType() const;
    void setType(EntityType);


    bool isDrawable() const;
    void enableDraw();
    void disableDraw();

    bool isEnabled() const;
    void enable();
    void disable();

private:
    bool _drawable;
    bool _enabled;

    U32 _transformCallbackId;
    U32 _sceneId;
    EntityType _type;

    //components
    Transform _transform;
    std::unique_ptr<gfx::Shape> _shape;
    const gfx::Material* _material;    ///< Not a unique ptr because the entity does not own the material object (ResourceManager does)
    std::unique_ptr<physics::Rigidbody> _rigidbody;
    std::unique_ptr<PlayerComponent> _player;
    std::unique_ptr<AIComponent> _ai;
    std::unique_ptr<BulletComponent> _bullet;
    std::unique_ptr<audio::Source> _source;
    std::unique_ptr<audio::Listener> _listener;
    std::unique_ptr<CameraComponent> _camera;

    // disallow copy/assignment
    Entity(const Entity&);
    void operator=(const Entity&);
};

} //namespace pbj::scene
} //namespace pbj

#endif
