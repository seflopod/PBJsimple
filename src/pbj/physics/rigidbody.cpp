#ifndef RIGIDBODY_H_
#include "pbj/physics/rigidbody.h"
#endif

using namespace pbj;
using namespace pbj::physics;

/*Rigidbody::Rigidbody(Rigidbody::BodyType bodyType, const b2Shape* shape, b2World* physWorld, Entity& owner)
{
	b2BodyDef bd;
	bd.type = (b2BodyType)bodyType;
	bd.position.SetZero();
	bd.angle = 0.0f;
	bd.linearDamping = 0.0f;
	bd.allowSleep = true;
	bd.awake = true;
	bd.bullet = false;
	bd.active = true;
	_body = physWorld->CreateBody(&bd);
	_body->CreateFixture(shape, 1.0f);

	_owner = owner;
	_body->SetUserData(_owner);
}*/

Rigidbody::Rigidbody(Rigidbody::BodyType bodyType, const b2Shape* shape, b2World* physWorld)
{
	b2BodyDef bd;
	bd.type = (b2BodyType)bodyType;
	bd.position.SetZero();
	bd.angle = 0.0f;
	bd.linearDamping = 0.0f;
	bd.allowSleep = true;
	bd.awake = true;
	bd.bullet = false;
	bd.active = true;
	_body = physWorld->CreateBody(&bd);
	_body->CreateFixture(shape, 1.0f);
}

Rigidbody::~Rigidbody()
{
	if(_body != 0)// || _owner !=0)
		destroy();
}


b2Fixture* Rigidbody::getFixtureList()
{
	return _body->GetFixtureList();
}

const b2Fixture* Rigidbody::getFixtureList() const
{
	return _body->GetFixtureList();
}

void Rigidbody::destroyFixture(b2Fixture* fixture)
{
	_body->DestroyFixture(fixture);
}


b2ContactEdge* Rigidbody::getContactList()
{
	return _body->GetContactList();
}

const b2ContactEdge* Rigidbody::getContactList() const
{
	return _body->GetContactList();
}


void Rigidbody::applyForce(const vec2& force)
{
	_body->ApplyForceToCenter(b2Vec2(force.x, force.y));
}

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


vec2 Rigidbody::getVelocity() const
{
	b2Vec2 v = _body->GetLinearVelocity();
	return vec2(v.x, v.y);
}

void Rigidbody::setVelocity(const vec2& v)
{
	_body->SetLinearVelocity(b2Vec2(v.x, v.y));
}


F32 Rigidbody::getMass()
{
	return _body->GetMass();
}

void Rigidbody::setMass(F32 m)
{
	b2MassData md;
	md.mass = m;
	_body->SetMassData(&md);
}


Rigidbody::BodyType Rigidbody::getType() const
{
	return (Rigidbody::BodyType)(_body->GetType());
}


void Rigidbody::setBullet(bool bullet)
{
	_body->SetBullet(bullet);
}

bool Rigidbody::isBullet()
{
	return _body->IsBullet();
}


bool Rigidbody::isAwake()
{
	return _body->IsAwake();
}

bool Rigidbody::isActive()
{
	return _body->IsActive();
}

/*Entity* Rigidbody::getOwner()
{
	return _owner;
}*/

/*void Rigidbody::moveOwner()
{
	vec2 bPos = _body->GetPosition();
	vec3 curPos = vec3(bPos.x, bPos.y, _body->GetUserData()->getPosition().z);
	_body->GetUserData()->setPosition(curPos);
}*/

/// \details Since the rigidbody is being destroyed, I assume the b2body should
///          be destroyed as well.  But we cannot assume the owner Entity needs
///          to be destroyed, so I'll just NULL the pointer.
void Rigidbody::destroy()
{
	//hope this works.  otherwise need a temp b2World*
	_body->GetWorld()->DestroyBody(_body);
	_body = 0;
	//_owner = 0;
}