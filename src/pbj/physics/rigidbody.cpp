////////////////////////////////////////////////////////////////////////////////
/// \file Z:\Documents\PBJsimple\src\pbj\physics\rigidbody.cpp
///
/// \brief Implements the rigidbody class.
////////////////////////////////////////////////////////////////////////////////
#ifndef RIGIDBODY_H_
#include "pbj/physics/rigidbody.h"
#endif

#include "pbj/scene/entity.h"

using namespace pbj;
using namespace pbj::physics;
using pbj::scene::Entity;

////////////////////////////////////////////////////////////////////////////////
/// \fn Rigidbody::Rigidbody(Rigidbody::BodyType bodyType,
/// 	const b2Shape* shape, b2World* physWorld, void* owner)
///
/// \brief Initializes a new instance of the Rigidbody class.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \param bodyType			  Type of the body.
/// \param shape			  The shape.
/// \param [in,out] physWorld If non-null, the physical world.
/// \param [in,out] owner	  If non-null, the owner.
////////////////////////////////////////////////////////////////////////////////
Rigidbody::Rigidbody(Rigidbody::BodyType bodyType, const b2Shape* shape,
					 b2World* physWorld, void* owner) :
					_owner(owner)
{
	b2FixtureDef fd;
	fd.shape = shape;
	fd.density = 1.0f;

	b2BodyDef bd;
	///< An enum constant representing the bd.type option
	bd.type = (b2BodyType)bodyType;
	bd.position.SetZero();
	bd.angle = 0.0f;
	bd.linearDamping = 0.0f;
	bd.allowSleep = true;
	bd.awake = true;
	bd.bullet = false;
	bd.active = true;

	_body = physWorld->CreateBody(&bd);
	_body->CreateFixture(&fd);

	if(_owner)
		_body->SetUserData(_owner);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Rigidbody::~Rigidbody()
///
/// \brief Finalizes an instance of the Rigidbody class.
///
/// \author Peter Bartosch
/// \date 2013-08-08
////////////////////////////////////////////////////////////////////////////////
Rigidbody::~Rigidbody()
{
	if(_body != 0 || _owner !=0)
		destroy();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn b2Fixture* Rigidbody::getFixtureList()
///
/// \brief Gets fixture list.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return null if it fails, else the fixture list.
////////////////////////////////////////////////////////////////////////////////
b2Fixture* Rigidbody::getFixtureList()
{
	return _body->GetFixtureList();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn const b2Fixture* Rigidbody::getFixtureList() const
///
/// \brief Gets fixture list.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return null if it fails, else the fixture list.
////////////////////////////////////////////////////////////////////////////////
const b2Fixture* Rigidbody::getFixtureList() const
{
	return _body->GetFixtureList();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Rigidbody::destroyFixture(b2Fixture* fixture)
///
/// \brief Destroys the fixture described by fixture.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \param [in,out] fixture If non-null, the fixture.
////////////////////////////////////////////////////////////////////////////////
void Rigidbody::destroyFixture(b2Fixture* fixture)
{
	_body->DestroyFixture(fixture);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn b2ContactEdge* Rigidbody::getContactList()
///
/// \brief Gets contact list.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return null if it fails, else the contact list.
////////////////////////////////////////////////////////////////////////////////
b2ContactEdge* Rigidbody::getContactList()
{
	return _body->GetContactList();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn const b2ContactEdge* Rigidbody::getContactList() const
///
/// \brief Gets contact list.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return null if it fails, else the contact list.
////////////////////////////////////////////////////////////////////////////////
const b2ContactEdge* Rigidbody::getContactList() const
{
	return _body->GetContactList();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Rigidbody::applyForce(const vec2& force)
///
/// \brief Applies the force described by force.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \param force The force.
////////////////////////////////////////////////////////////////////////////////
void Rigidbody::applyForce(const vec2& force)
{
	_body->ApplyForceToCenter(b2Vec2(force.x, force.y));
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Rigidbody::applyForce(const vec2& force,
/// 	Rigidbody::ForceMode mode)
///
/// \brief Applies the force.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \param force The force.
/// \param mode  The mode.
////////////////////////////////////////////////////////////////////////////////
void Rigidbody::applyForce(const vec2& force, Rigidbody::ForceMode mode)
{
	switch(mode)
	{
	case Rigidbody::ForceMode::Constant:
		_body->ApplyForceToCenter(b2Vec2(force.x, force.y));
		break;
	case Rigidbody::ForceMode::Impulse:
		_body->ApplyLinearImpulse(b2Vec2(force.x,force.y), _body->GetPosition());
		break;
	default:
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \fn vec2 Rigidbody::getVelocity() const
///
/// \brief Gets the velocity.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return The velocity.
////////////////////////////////////////////////////////////////////////////////
vec2 Rigidbody::getVelocity() const
{
	b2Vec2 v = _body->GetLinearVelocity();
	return vec2(v.x, v.y);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Rigidbody::setVelocity(const vec2& v)
///
/// \brief Sets a velocity.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \param v The const vec2&amp; to process.
////////////////////////////////////////////////////////////////////////////////
void Rigidbody::setVelocity(const vec2& v)
{
	_body->SetLinearVelocity(b2Vec2(v.x, v.y));
}

////////////////////////////////////////////////////////////////////////////////
/// \fn F32 Rigidbody::getMass()
///
/// \brief Gets the mass.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return The mass.
////////////////////////////////////////////////////////////////////////////////
F32 Rigidbody::getMass()
{
	return _body->GetMass();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Rigidbody::setMass(F32 m)
///
/// \brief Sets the mass.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \param m The F32 to process.
////////////////////////////////////////////////////////////////////////////////
void Rigidbody::setMass(F32 m)
{
	b2MassData md;
	md.mass = m;
	_body->SetMassData(&md);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Rigidbody::BodyType Rigidbody::getType() const
///
/// \brief Gets the type.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return The type.
////////////////////////////////////////////////////////////////////////////////
Rigidbody::BodyType Rigidbody::getType() const
{
	return (Rigidbody::BodyType)(_body->GetType());
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Rigidbody::setBullet(bool bullet)
///
/// \brief Sets a bullet.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \param bullet true to bullet.
////////////////////////////////////////////////////////////////////////////////
void Rigidbody::setBullet(bool bullet)
{
	_body->SetBullet(bullet);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn bool Rigidbody::isBullet()
///
/// \brief Query if this object is bullet.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return true if bullet, false if not.
////////////////////////////////////////////////////////////////////////////////
bool Rigidbody::isBullet()
{
	return _body->IsBullet();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn bool Rigidbody::isAwake()
///
/// \brief Query if this object is awake.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return true if awake, false if not.
////////////////////////////////////////////////////////////////////////////////
bool Rigidbody::isAwake()
{
	return _body->IsAwake();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn bool Rigidbody::isActive()
///
/// \brief Query if this object is active.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return true if active, false if not.
////////////////////////////////////////////////////////////////////////////////
bool Rigidbody::isActive()
{
	return _body->IsActive();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void* Rigidbody::getOwner()
///
/// \brief Gets the owner of this item.
///
/// \author Peter Bartosch
/// \date 2013-08-08
///
/// \return null if it fails, else the owner.
////////////////////////////////////////////////////////////////////////////////
void* Rigidbody::getOwner()
{
	return _owner;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Rigidbody::updateOwnerTransform()
///
/// \brief Updates the owner transform.
///
/// \author Peter Bartosch
/// \date 2013-08-08
////////////////////////////////////////////////////////////////////////////////
void Rigidbody::updateOwnerTransform()
{
	b2Vec2 bPos = _body->GetPosition();
	F32 bRot = _body->GetAngle() * RADTODEG;
	if((Entity*)(_body->GetUserData()))
	{
		((Entity*)(_body->GetUserData()))->getTransform()->setPosition(vec2(bPos.x,bPos.y));
		((Entity*)(_body->GetUserData()))->getTransform()->setRotation(bRot);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Rigidbody::destroy()
///
/// \brief Destroys this object.
///
/// \author Peter Bartosch
/// \date 2013-08-08
/// \details Since the rigidbody is being destroyed, I assume the b2body should
///          be destroyed as well.  But we cannot assume the owner Entity needs
///          to be destroyed, so I'll just NULL the pointer.
////////////////////////////////////////////////////////////////////////////////
void Rigidbody::destroy()
{
	//hope this works.  otherwise need a temp b2World*
	_body->GetWorld()->DestroyBody(_body);
	_body = 0;
	_owner = 0;
}

Rigidbody::CollisionGroup Rigidbody::getCollisionGroup()
{
	return Other;
}

void Rigidbody::setCollisionGroup(CollisionGroup cg)
{
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Rigidbody::setTransform(b2Vec2 pos, b2Vec2 scale, float32 rot)
///
/// \brief Sets the transform on the b2Body that this object wraps.
/// 
/// \author Peter Bartosch
/// \date   2013-08-10
/// \details Because the scale paraemeter does nothing right now, this funciton
///          just wraps the SetTransform method of b2Body.  Scaling would
///          involve destroying the shape and the making a new one of the proper
///          size.  Not too difficult, but difficult (and unnecessary) enough
///          for me to ignore it.
void Rigidbody::setTransform(b2Vec2 pos, b2Vec2 scale, float32 rot)
{
	_body->SetTransform(pos, rot);
}