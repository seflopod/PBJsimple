///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/camera.cpp
/// \author Ben Crist

#include "pbj/scene/camera.h"

#include "pbj/_gl.h"
#include "pbj/_math.h"

namespace pbj {
namespace scene {

///////////////////////////////////////////////////////////////////////////////
Camera::Camera()
    : pos_k1_(20.0f),
      pos_k2_(0.5f),
      vel_k1_(5.0f),
      vel_k2_(0.33f)

{
}

///////////////////////////////////////////////////////////////////////////////
Camera::~Camera()
{
}

void Camera::setCoefficients(F32 position_k1, F32 position_k2, F32 velocity_k1, F32 velocity_k2)
{
    pos_k1_ = position_k1;
    pos_k2_ = position_k2;
    vel_k1_ = velocity_k1;
    vel_k2_ = velocity_k2;
}

///////////////////////////////////////////////////////////////////////////////
void Camera::setProjection(const mat4& projection)
{
    projection_ = projection;
}

///////////////////////////////////////////////////////////////////////////////
void Camera::setTargetPosition(const vec2& position)
{
    target_position_ = position;
}

///////////////////////////////////////////////////////////////////////////////
void Camera::setTargetVelocity(const vec2& velocity)
{
    target_velocity_ = velocity;
}

const vec2& Camera::getTargetPosition() const
{
    return target_position_;
}

const vec2& Camera::getTargetVelocity() const
{
    return target_velocity_;
}

///////////////////////////////////////////////////////////////////////////////
const mat4& Camera::getProjection() const
{
    return projection_;
}

///////////////////////////////////////////////////////////////////////////////
const mat4& Camera::getView() const
{
    return view_;
}

///////////////////////////////////////////////////////////////////////////////
void Camera::update(double delta_t)
{
    vec2 pos_delta = target_position_ - position_;
    vec2 vel_delta = target_velocity_ - velocity_;
    vec2 acceleration = pos_delta * pos_k1_;
    acceleration += pos_delta * glm::length(pos_delta) * pos_k2_;
    acceleration += vel_delta * vel_k1_;
    acceleration += vel_delta * glm::length(vel_delta) * vel_k2_;

    position_ += velocity_ * float(delta_t) +
                 acceleration * float(delta_t * delta_t * 0.5);

    velocity_ += acceleration * float(delta_t);

    view_ = glm::translate(mat4(), vec3(-position_, 0));
    vp_inv_ = glm::inverse(projection_ * view_);
}

void Camera::use() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection_));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view_));
}

vec2 Camera::getWorldPosition(const ivec2& screen_coords, const ivec2& context_size) const
{
    vec2 pos(screen_coords);
    pos += vec2(0.5f, 0.5f);
    pos /= context_size;
    pos -= vec2(0.5f, 0.5f);
    pos.x *= 2.0f;
    pos.y *= -2.0f;

    return vec2(vp_inv_ * vec4(pos, 0, 1));
}

} // namespace pbj::scene
} // namespace pbj
