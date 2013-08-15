#ifndef AI_COMPONENT_H_
#include "pbj/scene/ai_component.h"
#endif

#include <iostream>
#include "pbj/scene/entity.h"
#include "pbj/engine.h"

//I just hate seeing std:: all the damn time
using std::sin;
using std::cos;

namespace pbj
{
namespace scene
{

AIComponent::AIComponent(void* owner) :
				_seePlayer(false),
				_sweepCnt(0),
				_moveStart(0),
				_dir(0)
{
	assert((Entity*)owner);
	_owner = owner;
	srand(time(nullptr));
}

AIComponent::~AIComponent()
{
	_owner = nullptr;
}

void AIComponent::update(F32 dt)
{
	F32 castDistance = 100.0f;
	Entity* e = (Entity*)_owner;
	
	vec2 center = e->getTransform()->getPosition();
	vec2 scale = e->getTransform()->getScale();

	F32 r1 = (scale.x>scale.y) ? scale.x : scale.y;
	F32 r2 = r1 + castDistance;
	F32 angle = DEGTORAD * (6 * _sweepCnt);

	//make the starting point on a circle off of the body, this way it isn't
	//counted in the raycast.
	getEngine().getWorld()->RayCast(this,
									b2Vec2(r1*cos(angle),r1*sin(angle)), //start
									b2Vec2(r2*cos(angle),r2*sin(angle))); //end
	if(_seePlayer)
	{
		e->getPlayerComponent()->fire(_target.x, _target.y);
		e->getPlayerComponent()->fire(_target.x, _target.y);
		e->getPlayerComponent()->fire(_target.x, _target.y);
		_seePlayer = false;
	}
	if(_moveStart == 0 || _moveStart == _sweepCnt)
	{
		_dir = rand()%2;
		_moveStart = rand()%60;
	}

	if(_dir)
		e->getPlayerComponent()->moveLeft();
	else
		e->getPlayerComponent()->moveRight();

	I32 jumpThrust = rand()%3;
	switch(jumpThrust)
	{
	case 0: e->getPlayerComponent()->jump(); break;
	case 1: e->getPlayerComponent()->doThrust(); break;
	default: break;
	}

	if(++_sweepCnt == 60)
		_sweepCnt = 0;
}

float32 AIComponent::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
	Entity* e = (Entity*)(fixture->GetBody()->GetUserData());
	if(e->getType()==Entity::EntityType::Player)
	{
		_target = e->getTransform()->getPosition();
		_seePlayer = (e->getType() == Entity::EntityType::Player);
	}
	return fraction; //because we only want the closest fixture;
}

}
}