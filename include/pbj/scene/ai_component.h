////////////////////////////////////////////////////////////////////////////////
/// \file   Z:\Documents\PBJsimple\include\pbj\scene\ai_component.h
///
/// \brief  Declares the ai component class.
////////////////////////////////////////////////////////////////////////////////
#ifndef PBJ_SCENE_AI_COMPONENT_H_
#define PBJ_SCENE_AI_COMPONENT_H_

#include <assert.h>
#include <time.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj {
namespace scene {

class Entity;

////////////////////////////////////////////////////////////////////////////
/// \class  AIComponent
///
/// \brief  Defines methods and data for the game's AI.  Inherits from
///         b2RayCastCallback for raycasting purposes.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////
class AIComponent : public b2RayCastCallback
{
public:
    AIComponent(Entity*);
    ~AIComponent();

    void update(F32);

    float32 ReportFixture(b2Fixture*, const b2Vec2&, const b2Vec2&, float32);
private:
    bool _seePlayer;
    vec2 _target;
    Entity* _owner;
    I32 _sweepCnt;
    I32 _moveStart;
    I32 _dir;
};

} //namespace pbj::scene
} //namespace pbj

#endif
