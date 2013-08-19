#ifndef AI_COMPONENT_H_
#define AI_COMPONENT_H_

#include <assert.h>
#include <time.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj {
namespace scene {

class Entity;

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
