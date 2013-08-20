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
{
}

///////////////////////////////////////////////////////////////////////////////
Camera::~Camera()
{
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
    const F32 pos_coeff = 1.9f;
    const F32 vel_coeff = 1.5f;

    vec2 acceleration = (target_position_ - position_) * pos_coeff;
    acceleration += (target_velocity_ - velocity_) * vel_coeff;

    position_ += velocity_ * float(delta_t) +
                 acceleration * float(delta_t * delta_t * 0.5);

    velocity_ += acceleration * float(delta_t);

    view_ = glm::translate(mat4(), vec3(-position_, 0));
}

void Camera::use() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection_));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(view_));
}

} // namespace pbj::scene
} // namespace pbj
