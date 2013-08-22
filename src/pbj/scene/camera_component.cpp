////////////////////////////////////////////////////////////////////////////////
/// \file   Z:\Documents\PBJsimple\src\pbj\scene\camera_component.cpp
///
/// \brief  Implements the camera component class.
////////////////////////////////////////////////////////////////////////////////
#ifndef CAMERA_COMPONENT_H_
#include "pbj/scene/camera_component.h"
#endif

#include <assert.h>
#include "pbj/scene/entity.h"

namespace pbj {
namespace scene {

////////////////////////////////////////////////////////////////////////////////
/// \fn CameraComponent::CameraComponent(void* owner)
///
/// \brief  Constructor.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param [in] owner   If non-null, the owner.  This must be an Entity.
////////////////////////////////////////////////////////////////////////////////
CameraComponent::CameraComponent(void* owner)
    : _pos_k1(20.0f),
      _pos_k2(0.5f),
      _vel_k1(5.0f),
      _vel_k2(0.33f)

{
	assert((Entity*)owner);
	_owner = owner;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn CameraComponent::~CameraComponent()
///
/// \brief  Destructor.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
CameraComponent::~CameraComponent()
{}

////////////////////////////////////////////////////////////////////////////////
/// \fn void CameraComponent::setCoefficients(F32 _positionk1,
///     F32 _positionk2, F32 _velocityk1, F32 _velocityk2)
///
/// \brief  Sets the coefficients.  Straight copy from Camera.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  _positionk1 The first positionk.
/// \param  _positionk2 The second positionk.
/// \param  _velocityk1 The first velocityk.
/// \param  _velocityk2 The second velocityk.
////////////////////////////////////////////////////////////////////////////////
void CameraComponent::setCoefficients(F32 _positionk1, F32 _positionk2,
                                      F32 _velocityk1, F32 _velocityk2)
{
    _pos_k1 = _positionk1;
    _pos_k2 = _positionk2;
    _vel_k1 = _velocityk1;
    _vel_k2 = _velocityk2;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void CameraComponent::setProjection(const mat4& projection)
///
/// \brief  Sets a projection.  Straight copy from Camera.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  projection  The projection.
////////////////////////////////////////////////////////////////////////////////
void CameraComponent::setProjection(const mat4& projection)
{
    _projection = projection;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void CameraComponent::setTargetPosition(const vec2& position)
///
/// \brief  Sets target position.  Straight copy from Camera
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  position    The position.
////////////////////////////////////////////////////////////////////////////////
void CameraComponent::setTargetPosition(const vec2& position)
{
    _target_position = position;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void CameraComponent::setTargetVelocity(const vec2& velocity)
///
/// \brief  Sets target velocity.  Straight copy from Camera
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  velocity    The velocity.
////////////////////////////////////////////////////////////////////////////////
void CameraComponent::setTargetVelocity(const vec2& velocity)
{
    _target_velocity = velocity;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn const vec2& CameraComponent::getTargetPosition() const
///
/// \brief  Gets target position.  Straight copy from Camera.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \return The target position.
////////////////////////////////////////////////////////////////////////////////
const vec2& CameraComponent::getTargetPosition() const
{
    return _target_position;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn const vec2& CameraComponent::getTargetVelocity() const
///
/// \brief  Gets target velocity.  Straight copy from Camera.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \return The target velocity.
////////////////////////////////////////////////////////////////////////////////
const vec2& CameraComponent::getTargetVelocity() const
{
    return _target_velocity;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn const mat4& CameraComponent::getProjection() const
///
/// \brief  Gets the projection.  Straight copy from Camera.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \return The projection.
////////////////////////////////////////////////////////////////////////////////
const mat4& CameraComponent::getProjection() const
{
    return _projection;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn const mat4& CameraComponent::getView() const
///
/// \brief  Gets the view.  Straight copy from Camera
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \return The view.
////////////////////////////////////////////////////////////////////////////////
const mat4& CameraComponent::getView() const
{
    return _view;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void CameraComponent::update(F32 dt)
///
/// \brief  Updates this CameraComponent.
///
/// \author Ben Crist / Peter Bartosch
/// \date   2013-08-22
///
/// \param  dt  The delta time.
/// 
/// \details    This will set move the position and increase the velocity for
///             the CameraComponent. The velocity is tracked locally since we
///             assume that the Entity with a camera will not have a Rigidbody.
///             Once position is updated here it is passed back to the owner's
///             Transform.
////////////////////////////////////////////////////////////////////////////////
void CameraComponent::update(F32 dt)
{
	Entity* e = (Entity*)_owner;
	vec2 pos = e->getTransform().getPosition();
    vec2 pos_delta = _target_position - pos;
    vec2 vel_delta = _target_velocity - _velocity;
    vec2 acceleration = pos_delta * _pos_k1;
    acceleration += pos_delta * glm::length(pos_delta) * _pos_k2;
    acceleration += vel_delta * _vel_k1;
    acceleration += vel_delta * glm::length(vel_delta) * _vel_k2;
	
	//originally velocity was updated after position, I want to try it like this
	_velocity += acceleration * dt;
	pos += _velocity * dt + acceleration * dt * dt * 0.5f;
	e->getTransform().setPosition(pos);
    
	_view = glm::translate(mat4(), vec3(-pos, 0));
    _vp_inv = glm::inverse(_projection * _view);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void CameraComponent::use() const
///
/// \brief  Uses this CameraComponent. Straight copy from Camera
///
/// \author Ben Crist
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
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
/// \brief  Gets world position.  Straight copy from Camera
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \param  screen_coords   The screen coords.
/// \param  context_size    Size of the context.
///
/// \return The world position.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn void* CameraComponent::getOwner() const
///
/// \brief  Gets the owner of this item.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the owner.
////////////////////////////////////////////////////////////////////////////////
void* CameraComponent::getOwner() const { return _owner; }

} // namespace scene
} // namespace pbj