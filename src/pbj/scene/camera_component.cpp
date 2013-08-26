////////////////////////////////////////////////////////////////////////////////
/// \file   pbj\scene\camera_component.cpp
///
/// \brief  Implements the camera component class.
#include "pbj/scene/camera_component.h"


#include "pbj/scene/entity.h"
#include <cassert>

namespace pbj {
namespace scene {

////////////////////////////////////////////////////////////////////////////////
/// \fn CameraComponent::CameraComponent(void* owner)
///
/// \brief  Constructor.
///
/// \author Ben Crist
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param [in] owner   If non-null, the owner.  This must be an Entity.
CameraComponent::CameraComponent(Entity* owner)
    : _owner(owner),
      _pos_k1(20.0f),
      _pos_k2(0.5f),
      _vel_k1(5.0f),
      _vel_k2(0.33f)
{
}

////////////////////////////////////////////////////////////////////////////////
/// \fn CameraComponent::~CameraComponent()
///
/// \brief  Destructor.
///
/// \author Ben Crist
/// \author Peter Bartosch
/// \date   2013-08-22
CameraComponent::~CameraComponent()
{
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void CameraComponent::setCoefficients(F32 a,
///     F32 b, F32 c, F32 d)
///
/// \brief  Sets the coefficients that determine the camera's acceleration.
///
/// \details The acceleration is defined by the eqation:
///
///          camera_acceleration  = a * position_delta +
///                                 b * position_delta * ||position_delta|| +
///                                 c * velocity_delta +
///                                 d * velocity_delta * ||velocity_delta||
///
///         where position_delta = target_position - camera_position
///         and   velocity_delta = target_velocity - camera_velocity
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  a The first-order delta-position coefficient.
/// \param  b The second-order delta-position coefficient.
/// \param  c The first-order delta-velocity coefficient.
/// \param  d The second-order delta-velocity coefficient.
void CameraComponent::setCoefficients(F32 a, F32 b, F32 c, F32 d)
{
    _pos_k1 = a;
    _pos_k2 = b;
    _vel_k1 = c;
    _vel_k2 = d;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void CameraComponent::setProjection(const mat4& projection)
///
/// \brief  Sets a projection matrix.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  projection  The projection.
void CameraComponent::setProjection(const mat4& projection)
{
    _projection = projection;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void CameraComponent::setTargetPosition(const vec2& position)
///
/// \brief  Sets target position.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  position    The position.
void CameraComponent::setTargetPosition(const vec2& position)
{
    _target_position = position;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void CameraComponent::setTargetVelocity(const vec2& velocity)
///
/// \brief  Sets target velocity.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  velocity    The velocity.
void CameraComponent::setTargetVelocity(const vec2& velocity)
{
    _target_velocity = velocity;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void CameraComponent::setPosition(const vec2& position)
///
/// \brief  Sets camera's current position.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  velocity    The velocity.
void CameraComponent::setPosition(const vec2& position)
{
	_position = position;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void CameraComponent::setVelocity(const vec2& velocity)
///
/// \brief  Sets camera's velocity.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  velocity    The velocity.
void CameraComponent::setVelocity(const vec2& velocity)
{
	_velocity = velocity;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn const vec2& CameraComponent::getTargetPosition() const
///
/// \brief  Gets target position.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \return The target position.
const vec2& CameraComponent::getTargetPosition() const
{
    return _target_position;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn const vec2& CameraComponent::getTargetVelocity() const
///
/// \brief  Gets target velocity.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \return The target velocity.
const vec2& CameraComponent::getTargetVelocity() const
{
    return _target_velocity;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn const mat4& CameraComponent::getProjection() const
///
/// \brief  Gets the projection matrix.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \return The projection.
const mat4& CameraComponent::getProjection() const
{
    return _projection;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn const mat4& CameraComponent::getView() const
///
/// \brief  Gets the view matrix.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \return The view.
const mat4& CameraComponent::getView() const
{
    return _view;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void CameraComponent::update(F32 dt)
///
/// \brief  Updates this CameraComponent.
///
/// \author Ben Crist
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  dt  The delta time.
///
/// \details    This will set move the position and increase the velocity for
///             the CameraComponent. The velocity is tracked locally since we
///             assume that the Entity with a camera will not have a Rigidbody.
///             Once position is updated here it is passed back to the owner's
///             Transform.
void CameraComponent::update(F32 dt)
{
    if (_owner)
    {
        _position = _owner->getTransform().getPosition();
    }

    vec2 pos_delta = _target_position - _position;
    vec2 vel_delta = _target_velocity - _velocity;
    vec2 acceleration = pos_delta * _pos_k1;
    acceleration += pos_delta * glm::length(pos_delta) * _pos_k2;
    acceleration += vel_delta * _vel_k1;
    acceleration += vel_delta * glm::length(vel_delta) * _vel_k2;

    // //originally velocity was updated after position, I want to try it like this
    //_velocity += acceleration * dt;
    // // ben's note: velocity should be updated after position because the (1/2)at^2 term takes into account the change in velocity over the time period.
    _position += _velocity * dt + acceleration * dt * dt * 0.5f;
    _velocity += acceleration * dt;
    if (_owner)
    {
        _owner->getTransform().setPosition(_position);
    }

    _view = glm::translate(mat4(), vec3(-_position, 0));
    _vp_inv = glm::inverse(_projection * _view);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void CameraComponent::use() const
///
/// \brief  Overwrites the GL_PROJECTION and GL_MODELVIEW matrices so that
///         the viewport represent's this camera's view.
///
/// \author Ben Crist
/// \date   2013-08-22
void CameraComponent::use() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(_projection));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(_view));
}

////////////////////////////////////////////////////////////////////////////////
/// \fn vec2 CameraComponent::getWorldPosition(const ivec2& screen_coords,
///     const ivec2& context_size) const
///
/// \brief  Unprojects a position in pixel coordinates into world coordinates.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  screen_coords   The screen coords.
/// \param  context_size    Size of the context.
///
/// \return The world position.
vec2 CameraComponent::getWorldPosition(const ivec2& screen_coords,
                                       const ivec2& context_size) const
{
    vec2 pos(screen_coords);
    pos += vec2(0.5f, 0.5f);
    pos /= context_size;
    pos -= vec2(0.5f, 0.5f);
    pos.x *= 2.0f;
    pos.y *= -2.0f;

    return vec2(_vp_inv * vec4(pos, 0, 1));
}


///////////////////////////////////////////////////////////////////////////////
/// \fn vec2 CameraComponent::getScreenPosition(const vec2& world_coords,
///     const ivec2& context_size) const
///
/// \brief  Projects a position in world coordinates onto the screen, returning
///         pixel coordinates.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  screen_coords   The screen coords.
/// \param  context_size    Size of the context.
///
/// \return The world position.
vec2 CameraComponent::getScreenPosition(const vec2& world_coords, const ivec2& context_size) const
{
    vec2 pos(_projection * _view * vec4(world_coords, 0, 1));
    pos.x *= 0.5f;
    pos.y *= -0.5f;
    pos += vec2(0.5f, 0.5f);
    pos *= context_size;

    return pos;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Entity* CameraComponent::getOwner() const
///
/// \brief  Gets the owner of this item.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the owner.
Entity* CameraComponent::getOwner() const
{
    return _owner;
}

} // namespace scene
} // namespace pbj