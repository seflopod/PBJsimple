////////////////////////////////////////////////////////////////////////////////
/// \file   Z:\Documents\PBJsimple\include\pbj\physics\rigidbody.h
///
/// \brief  Declares the rigidbody class.
////////////////////////////////////////////////////////////////////////////////
#ifndef PBJ_PHYSICS_RIGIDBODY_H_
#define PBJ_PHYSICS_RIGIDBODY_H_

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include "pbj/_math.h"
#include "pbj/_pbj.h"

namespace pbj {
namespace physics {
////////////////////////////////////////////////////////////////////////////
/// \class  Rigidbody
///
/// \brief  Rigidbody class for wrapping around Box2D.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////
class Rigidbody
{
public:

    ////////////////////////////////////////////////////////////////////////
    /// \enum   CollisionGroup
    ///
    /// \brief  Values that represent CollisionGroup.  Used for shorthand
    ///         collision filtering.
    ////////////////////////////////////////////////////////////////////////
    enum CollisionGroup
    {
        Player = -1,
        Terrain = -2,
        SpawnPoint = -4,
        Pickup = 8,
        Other = 16
    };

    ////////////////////////////////////////////////////////////////////////
    /// \enum   ForceMode
    ///
    /// \brief  Values that represent how a force is applied.
    ////////////////////////////////////////////////////////////////////////
    enum ForceMode
    {
        Constant = 0x01,
        Impulse = 0x02
    };

    ////////////////////////////////////////////////////////////////////////
    /// \enum   BodyType
    ///
    /// \brief  Values that represent if the Rigidbody is static or dynamic.
    ////////////////////////////////////////////////////////////////////////
    enum BodyType
    {
        Static = 0x00,
        Dynamic = 0x02
    };

    Rigidbody(Rigidbody::BodyType, vec2, const b2Shape&, b2World*, F32, F32, F32, void*);
    virtual ~Rigidbody();

    b2Fixture* getFixtureList();
    const b2Fixture* getFixtureList() const;
    void destroyFixture(b2Fixture*);

    b2ContactEdge* getContactList();
    const b2ContactEdge* getContactList() const;

    void applyForce(const vec2&); ///< Applies to center of mass.  Assumes ForceMode::Constant
    void applyForce(const vec2&, Rigidbody::ForceMode);

    vec2 getVelocity() const;
    void setVelocity(const vec2&);

    F32 getAngularVelocity() const;
    void setAngularVelocity(F32);

    F32 getMass();
    void setMass(F32);

    void setFixedRotation(bool);
    void setTransform(b2Vec2, b2Vec2, float32);

    Rigidbody::BodyType getType() const;

    void setBullet(bool); ///< To turn on CCD with other dynamic bodies.
    bool isBullet();

    bool isAwake();
    bool isActive();
    void setActive(bool);
    CollisionGroup getCollisionGroup();
    void setCollisionGroup(CollisionGroup); ///< negative for no collision with group members

    void* getOwner();
    void updateOwnerTransform();

    void destroy();
private:
    b2Body* _body;
    void* _owner;
};

} //namespace physics
} //namespace pbj

#endif
