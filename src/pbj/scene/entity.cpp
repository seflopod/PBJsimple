////////////////////////////////////////////////////////////////////////////////
/// \file   Z:\Documents\PBJsimple\src\pbj\scene\entity.cpp
///
/// \brief  Implements the entity class.
////////////////////////////////////////////////////////////////////////////////
#ifndef ENTITY_H_
#include "pbj/scene/entity.h"
#endif

#include "be/bed/transaction.h"
#include "pbj/sw/sandwich_open.h"

#include <iostream>
#include "pbj/game.h"

namespace pbj {
namespace scene {

////////////////////////////////////////////////////////////////////////////////
/// \fn Entity::Entity()
///
/// \brief  Default constructor.
///
/// \author Peter Bartosch
/// \date   2013-08-05
Entity::Entity()
    : _drawable(false),
      _enabled(true),
      _transformCallbackId(U32(-1)),
      _sceneId(U32(-1)),
      _transform(this),
      _material(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Entity::~Entity()
///
/// \brief  Destructor.
///
/// \author Peter Bartosch
/// \date   2013-08-05
Entity::~Entity()
{
    _shape.reset();
    _rigidbody.reset();
    _player.reset();
    _ai.reset();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::update(F32 dt)
///
/// \brief  Updates using the given difference in time.
///
/// \author Peter Bartosch
/// \date   2013-08-13
///
/// \param  dt  The difference in time.
///
/// \details This mostly checks firing states for the PlayerComponent.  It also
///          uses the Rigidbody to make sure the Transform used for drawing is
///          up-to-date.
void Entity::update(F32 dt)
{
    if (!_enabled)
        return;

    if(_rigidbody)
        _rigidbody->updateOwnerTransform();

    if(_ai.get())
        _ai->update(dt);

    if(_player)
    {
        if(!_player->isThrusting())
            _player->regenFuel();

        if(_player->reloading())
            _player->stepReloadTimer(dt);

        if(_player->fireOnCooldown())
            _player->stepFireTimer(dt);

        if (_transform.getPosition().y < -50)
        {
            _player->setTimeOfDeath(glfwGetTime());
            _player->setDeaths(_player->getDeaths()+1);
            Game::instance()->getScene()->respawnPlayer(getSceneId());
        }
    }

    if (_source)
    {
        _source->updatePosition();
        _source->updateVelocity();
    }

    if (_listener)
    {
        _listener->updatePosition();
        _listener->updateVelocity();
    }

    //If a bullet isn't moving fast enough, make it disappear
    if(_type == Bullet && _rigidbody && glm::length2(_rigidbody->getVelocity()) < 16.0f)
        Game::instance()->getScene()->disableBullet(getSceneId());

    //Make sure the camera is doing as it needs to do.
    if(_camera)
        _camera->update(dt);
}
////////////////////////////////////////////////////////////////////////////////
/// \brief  draws this object.
///
/// \author Peter Bartosch / Josh Douglas / Ben Crist
/// \date   2013-08-08
////////////////////////////////////////////////////////////////////////////////
void Entity::draw()
{
    if (!_drawable)
        return;

    vec2 glmPos = _transform.getPosition();
    F32 glmRot = _transform.getRotation();
    vec2 glmSca = _transform.getScale();

    if (_material)
        _material->use();
    else
        gfx::Texture::disable();

    glPushMatrix();

    glTranslatef(glmPos.x, glmPos.y, 0.0f);
    glRotatef(glmRot, 0, 0, 1);
    glScalef(glmSca.x, glmSca.y, 1.0f);
    _shape->draw((_material->getTexture() != nullptr));

    glPopMatrix();
}

#pragma region components
////////////////////////////////////////////////////////////////////////////////
/// \fn Transform* Entity::getTransform() const
///
/// \brief  Gets the transform.
///
/// \author Peter Bartosch
/// \date   2013-08-05
///
/// \return the transform.
////////////////////////////////////////////////////////////////////////////////
Transform& Entity::getTransform() { return _transform; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::setTransform(Transform transform)
///
/// \brief  Sets a transform.
///
/// \author Peter Bartosch
/// \date   2013-08-05
///
/// \param  transform   The transform.
////////////////////////////////////////////////////////////////////////////////
void Entity::setTransform(const Transform& transform) { _transform = transform; }

////////////////////////////////////////////////////////////////////////////////
/// \fn Shape* Entity::getShape() const
///
/// \brief  Gets the shape used by the Entity.
///
/// \author Peter Bartosch
/// \date   2013-08-13
///
/// \return null if the shape does not exist; a pointer to the Shape otherwise.
////////////////////////////////////////////////////////////////////////////////
gfx::Shape* Entity::getShape() const { return _shape.get(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::setShape(Shape* shape)
///
/// \brief  Sets the shape of the entity.
///
/// \author Peter Bartosch / Ben Crist
/// \date   2013-08-13
///
/// \param [in] shape   A pointer to the Shape to use.
/// \details Any class the uses the Shape interface (ShapeSquare, ShapeTriangle)
///          is acceptable.  This is what will be drawn.
////////////////////////////////////////////////////////////////////////////////
void Entity::setShape(gfx::Shape* shape) { _shape.reset(shape); }

////////////////////////////////////////////////////////////////////////////////
/// \brief  Gets the entity's material.
///
/// \author Peter Bartosch / Ben Crist
/// \date   2013-08-13
///
/// \return A pointer to the Material.
////////////////////////////////////////////////////////////////////////////////
const gfx::Material* Entity::getMaterial() { return _material; }

////////////////////////////////////////////////////////////////////////////////
/// \brief  Adds a material.
///
/// \author Peter Bartosch / Ben Crist
/// \date   2013-08-13
///
/// \param  material A pointer to a material object.  The object's lifetime
///         must be at least as long as this entity's.  In the normal usage,
///         a ResourceManager will manage the lifetime of the material object.
////////////////////////////////////////////////////////////////////////////////
void Entity::setMaterial(const gfx::Material* material) { _material = material; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::addRigidbody(Rigidbody::BodyType bodyType,
///     b2World* world)
///
/// \brief  Adds a rigidbody to the Entity.
///
/// \author Peter Bartosch / Ben Crist
/// \date   2013-08-13
///
/// \param  bodyType        Type of the body.
/// \param [in] world   The Box2D world that holds the Rigidbody.
/// \details This will create a Rigidbody based on the type of Entity that is
///          using it.  Things like shape, weight and restitution (bounciness)
///          are all passed to the Rigidbody class to create a new one for this
///          particular Entity.  This is rather simple and therefore does not
///          use multiple fixtures for making complex shapes.  Simple boxes,
///          polygons and circles are used instead.
////////////////////////////////////////////////////////////////////////////////
void Entity::addRigidbody(physics::Rigidbody::BodyType bodyType, b2World* world)
{
    if(!_rigidbody)
    {
        vec2 scale = _transform.getScale();
        vec2 pos = _transform.getPosition();
        b2PolygonShape shape;

        physics::Rigidbody* rigidbody = nullptr;

        switch(_type)
        {
        case Player:
        {
            shape.SetAsBox(scale.x/2, scale.y/2, b2Vec2_zero,
            _transform.getRotation());
            _rigidbody.reset(new physics::Rigidbody(bodyType, pos, shape, world, 100.0f/(scale.x*scale.y), 0.0f, 0.5f,this));
            _rigidbody->setCollisionGroup(physics::Rigidbody::Player);
            break;
        }
        case Terrain:
        {
            shape.SetAsBox(scale.x/2, scale.y/2, b2Vec2_zero,
            _transform.getRotation());
            _rigidbody.reset(new physics::Rigidbody(bodyType, pos, shape, world, 100.0f/(scale.x*scale.y), 0.0f, 0.5f,this));
            _rigidbody->setCollisionGroup(physics::Rigidbody::Terrain);
            break;
        }
        case SpawnPoint:
        {
            shape.SetAsBox(scale.x/2, scale.y/2, b2Vec2_zero,
            _transform.getRotation());
            _rigidbody.reset(new physics::Rigidbody(bodyType, pos, shape, world, 1.0f/(scale.x*scale.y), 0.0f, 1.0f, this));
            _rigidbody->setCollisionGroup(physics::Rigidbody::SpawnPoint);
            break;
        }
        case Bullet:
        {
            b2Vec2 verts[3];
            verts[0] = b2Vec2(-0.5f*scale.x, -0.433f*scale.y);
            verts[1] = b2Vec2(0.5f*scale.x, -0.433f*scale.y);
            verts[2] = b2Vec2(0.0f*scale.x, 0.433f*scale.y);
            shape.Set(verts,3);
            _rigidbody.reset(new physics::Rigidbody(bodyType, pos, shape, world, 0.01f/(scale.x*scale.y), 0.5f, 0.1f,this));
        }
        default:
        {
            shape.SetAsBox(scale.x/2, scale.y/2, b2Vec2_zero,
            _transform.getRotation());
            _rigidbody.reset(new physics::Rigidbody(bodyType, pos, shape, world, 1.0f/(scale.x*scale.y), 0.0f, 1.0f, this));
            _rigidbody->setCollisionGroup(physics::Rigidbody::Other);
        break;
        }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Rigidbody* Entity::getRigidbody() const
///
/// \brief  Gets a pointer to the Rigidbody.
///
/// \author Peter Bartosch / Ben Crist
/// \date   2013-08-13
///
/// \return null if it none exists, else the rigidbody.
////////////////////////////////////////////////////////////////////////////////
physics::Rigidbody* Entity::getRigidbody() const { return _rigidbody.get(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::addPlayerComponent()
///
/// \brief  Adds player component.
///
/// \author Peter Bartosch
/// \date   2013-08-13
////////////////////////////////////////////////////////////////////////////////
void Entity::addPlayerComponent(const std::string& name)
{
    if (!_player)
        _player.reset(new PlayerComponent(name, PlayerStats(), this));
}

////////////////////////////////////////////////////////////////////////////////
/// \fn PlayerComponent* Entity::getPlayerComponent() const
///
/// \brief  Gets PlayerComponent.
///
/// \author Peter Bartosch
/// \date   2013-08-13
///
/// \return null if none exists, else the player component.
////////////////////////////////////////////////////////////////////////////////
PlayerComponent* Entity::getPlayerComponent() const { return _player.get(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::addAIComponent()
///
/// \brief  Adds an AI component.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void Entity::addAIComponent() { _ai.reset(new AIComponent(this)); }

////////////////////////////////////////////////////////////////////////////////
/// \fn AIComponent* Entity::getAIComponent() const
///
/// \brief  Gets an AI component.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the AI component.
////////////////////////////////////////////////////////////////////////////////
AIComponent* Entity::getAIComponent() const { return _ai.get(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::addBulletComponent()
///
/// \brief  Adds bullet component.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void Entity::addBulletComponent() { _bullet.reset(new BulletComponent(this)); }

////////////////////////////////////////////////////////////////////////////////
/// \fn BulletComponent* Entity::getBulletComponent() const
///
/// \brief  Gets bullet component.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the bullet component.
////////////////////////////////////////////////////////////////////////////////
BulletComponent* Entity::getBulletComponent() const { return _bullet.get(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::addAudioListener()
///
/// \brief  Adds audio listener.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void Entity::addAudioListener() { _listener.reset(new audio::Listener(this)); }

////////////////////////////////////////////////////////////////////////////////
/// \fn AudioListener* Entity::getAudioListener() const
///
/// \brief  Gets audio listener.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the audio listener.
////////////////////////////////////////////////////////////////////////////////
audio::Listener* Entity::getAudioListener() const { return _listener.get(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::addAudioSource()
///
/// \brief  Adds audio source.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void Entity::addAudioSource() { _source.reset(new audio::Source(this)); }

////////////////////////////////////////////////////////////////////////////////
/// \fn AudioSource* Entity::getAudioSource() const
///
/// \brief  Gets audio source.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the audio source.
////////////////////////////////////////////////////////////////////////////////
audio::Source* Entity::getAudioSource() const { return _source.get(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::addCamera()
///
/// \brief  Adds camera.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void Entity::addCamera() { _camera.reset(new CameraComponent(this)); }

////////////////////////////////////////////////////////////////////////////////
/// \fn CameraComponent* Entity::getCamera() const
///
/// \brief  Gets the camera.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the camera.
////////////////////////////////////////////////////////////////////////////////
CameraComponent* Entity::getCamera() const { return _camera.get(); }

#pragma endregion
////////////////////////////////////////////////////////////////////////////////
/// \fn U32 Entity::getSceneId() const
///
/// \brief  Gets scene identifier.
///
/// \author Peter Bartosch
/// \date   2013-08-13
///
/// \return The scene identifier.
U32 Entity::getSceneId() const
{
    return _sceneId;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::setSceneId(U32 id)
///
/// \brief  Sets scene identifier.
///
/// \author Peter Bartosch
/// \date   2013-08-13
///
/// \param  id  The identifier.
void Entity::setSceneId(U32 id)
{
    _sceneId = id;
}

Entity::EntityType Entity::getType() const
{
    return _type;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::setType(EntityType et)
///
/// \brief  Sets the EntityType and adds the Rigidbody to the right collision
///         group.
///
/// \author Peter Bartosch
/// \date   2013-08-13
///
/// \param  et  The EntityType to use.
////////////////////////////////////////////////////////////////////////////////
void Entity::setType(EntityType et)
{
    _type = et;
    if(_rigidbody != nullptr)
    {
        switch(_type)
        {
        case Player:
            _rigidbody->setCollisionGroup(physics::Rigidbody::Player);
            break;
        case Terrain:
            _rigidbody->setCollisionGroup(physics::Rigidbody::Terrain);
            break;
        case SpawnPoint:
            _rigidbody->setCollisionGroup(physics::Rigidbody::SpawnPoint);
            break;
        default:
            _rigidbody->setCollisionGroup(physics::Rigidbody::Other);
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::enableDraw()
///
/// \brief  Enables drawing.
///
/// \author Peter Bartosch
/// \date   2013-08-13
////////////////////////////////////////////////////////////////////////////////
void Entity::enableDraw() { _drawable = true; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::disableDraw()
///
/// \brief  Disables drawing.
///
/// \author Peter Bartosch
/// \date   2013-08-13
void Entity::disableDraw() { _drawable = false; }

////////////////////////////////////////////////////////////////////////////////
/// \fn bool Entity::isDrawable() const
///
/// \brief  Query if this object is drawable.
///
/// \author Peter Bartosch
/// \date   2013-08-13
///
/// \return true if drawable, false if not.
////////////////////////////////////////////////////////////////////////////////
bool Entity::isDrawable() const { return _drawable; }

////////////////////////////////////////////////////////////////////////////////
/// \fn bool Entity::isEnabled() const
///
/// \brief  Query if this Entity is enabled.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return true if enabled, false if not.
////////////////////////////////////////////////////////////////////////////////
bool Entity::isEnabled() const { return _enabled; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::enable()
///
/// \brief  Enables this Entity by turning on drawing and its Rigidbody.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void Entity::enable()
{
    if(_shape.get())
        _drawable = true;
    if(_rigidbody)
        _rigidbody->setActive(true);

    _enabled = true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::disable()
///
/// \brief  Disables this Entity by turning off drawing and its Rigidybody.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void Entity::disable()
{
    _drawable = false;
    if(_rigidbody)
        _rigidbody->setActive(false);

    _enabled = false;
}

} // namespace pbj::scene
} // namespace pbj
