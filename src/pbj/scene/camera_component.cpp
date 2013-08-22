#ifndef CAMERA_COMPONENT_H_
#include "pbj/scene/camera_component.h"
#endif

#include <assert.h>
#include "pbj/scene/entity.h"

namespace pbj {
namespace scene {

///////////////////////////////////////////////////////////////////////////////
CameraComponent::CameraComponent(Entity* owner)
    : _owner(owner),
      _pos_k1(20.0f),
      _pos_k2(0.5f),
      _vel_k1(5.0f),
      _vel_k2(0.33f)
{
}

///////////////////////////////////////////////////////////////////////////////
CameraComponent::~CameraComponent()
{
}

///////////////////////////////////////////////////////////////////////////////
void CameraComponent::setCoefficients(F32 position_k1, F32 position_k2, F32 velocity_k1, F32 velocity_k2)
{
    _pos_k1 = position_k1;
    _pos_k2 = position_k2;
    _vel_k1 = velocity_k1;
    _vel_k2 = velocity_k2;
}

///////////////////////////////////////////////////////////////////////////////
void CameraComponent::setProjection(const mat4& projection)
{
    _projection = projection;
}

///////////////////////////////////////////////////////////////////////////////
void CameraComponent::setTargetPosition(const vec2& position)
{
    _target_position = position;
}

///////////////////////////////////////////////////////////////////////////////
void CameraComponent::setTargetVelocity(const vec2& velocity)
{
    _target_velocity = velocity;
}

///////////////////////////////////////////////////////////////////////////////
const vec2& CameraComponent::getTargetPosition() const
{
    return _target_position;
}

///////////////////////////////////////////////////////////////////////////////
const vec2& CameraComponent::getTargetVelocity() const
{
    return _target_velocity;
}

///////////////////////////////////////////////////////////////////////////////
const mat4& CameraComponent::getProjection() const
{
    return _projection;
}

///////////////////////////////////////////////////////////////////////////////
const mat4& CameraComponent::getView() const
{
    return _view;
}

///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
void CameraComponent::use() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(_projection));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(_view));
}

///////////////////////////////////////////////////////////////////////////////
vec2 CameraComponent::getWorldPosition(const ivec2& screen_coords, const ivec2& context_size) const
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
vec2 CameraComponent::getScreenPosition(const vec2& world_coords, const ivec2& context_size) const
{
    vec2 pos(_projection * _view * vec4(world_coords, 0, 1));
    pos.x *= 0.5f;
    pos.y *= -0.5f;
    pos += vec2(0.5f, 0.5f);
    pos *= context_size;

    return pos;
}

///////////////////////////////////////////////////////////////////////////////
Entity* CameraComponent::getOwner() const
{
    return _owner;
}

} // namespace scene
} // namespace pbj