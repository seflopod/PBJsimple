
#ifndef RIGIDBODY_H_
#define RIGIDBODY_H_

#include <Box2D/Box2D.h>
#include <glm/glm.hpp>
#include "pbj/_math.h"
#include "pbj/_pbj.h"

namespace pbj
{
namespace physics
{

class Rigidbody
{
public:
	enum CollisionGroup
	{
		Player = -1,
		Terrain = -2,
	};

	enum ForceMode
	{
		Constant = 0x01,
		Impulse = 0x02
	};

	enum BodyType
	{
		Static = 0x00,
		Dynamic = 0x02
	};

	//Rigidbody(Rigidbody::BodyType, const b2Shape*, b2World*, Entity&);
	Rigidbody(Rigidbody::BodyType, const b2Shape*, b2World*);
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
	
	F32 getMass();
	void setMass(F32);

	Rigidbody::BodyType getType() const;

	void setBullet(bool); ///< To turn on CCD with other dynamic bodies.
	bool isBullet();

	bool isAwake();
	bool isActive();

	int getCollisionGroup();
	void setCollisionGroup(CollisionGroup); ///< negative for no collision with group members
	
	//class Entity* getOwner();
	//void moveOwner();

	void destroy();
private:
	b2Body* _body;
	//Entity* const _owner;
};

} //namespace physics
} //namespace pbj
#endif