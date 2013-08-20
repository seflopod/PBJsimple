///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/camera.h
/// \author Ben Crist

#ifndef PBJ_SCENE_CAMERA_H_
#define PBJ_SCENE_CAMERA_H_

#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj {
namespace scene {

class Camera
{
public:
    Camera();
    ~Camera();

    void setProjection(const mat4& projection);
    void setTargetPosition(const vec2& position);
    void setTargetVelocity(const vec2& velocity);

    const mat4& getProjection() const;
    const mat4& getView() const;

    void update(double delta_t);

    void use() const;

private:
    vec2 position_;
    vec2 velocity_;
    vec2 target_position_;
    vec2 target_velocity_;

    mat4 projection_;
    mat4 view_;

    Camera(const Camera&);
    void operator=(const Camera&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
