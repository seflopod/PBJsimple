////////////////////////////////////////////////////////////////////////////////
/// \file   Z:\Documents\PBJsimple\include\pbj\scene\camera_component.h
///
/// \brief  Declares the camera component class.
////////////////////////////////////////////////////////////////////////////////
#ifndef PBJ_SCENE_CAMERA_COMPONENT_H_
#define PBJ_SCENE_CAMERA_COMPONENT_H_

#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj {
namespace scene {

class Entity;

class CameraComponent
{
public:
    CameraComponent(Entity*);
    ~CameraComponent();

void setCoefficients(F32 a, F32 b, F32 c, F32 d);

    void setProjection(const mat4& projection);
    void setTargetPosition(const vec2& position);
    void setTargetVelocity(const vec2& velocity);

    const vec2& getTargetPosition() const;
    const vec2& getTargetVelocity() const;

    const mat4& getProjection() const;
    const mat4& getView() const;

    void update(F32 dt);

    void use() const;

    vec2 getWorldPosition(const ivec2& screen_coords, const ivec2& context_size) const;
    vec2 getScreenPosition(const vec2& world_coords, const ivec2& context_size) const;

    Entity* getOwner() const;

private:
    Entity* _owner;

    vec2 _position;
    vec2 _velocity;
    vec2 _target_position;
    vec2 _target_velocity;

    mat4 _projection;
    mat4 _view;
    mat4 _vp_inv;

    F32 _pos_k1;
    F32 _pos_k2;
    F32 _vel_k1;
    F32 _vel_k2;

    CameraComponent(const CameraComponent&);
    void operator=(const CameraComponent&);
};

} //namespace scene
} //namespace pbj

#endif
