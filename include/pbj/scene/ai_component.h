#ifndef AI_COMPONENT_H_
#define AI_COMPONENT_H_

#include <assert.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj
{
namespace scene
{
	class AIComponent : public b2RayCastCallback
	{
	public:
		AIComponent(void*);
		~AIComponent();
		
		void update(F32);

		float32 ReportFixture(b2Fixture*, const b2Vec2&, const b2Vec2&, float32);
	private:
		bool _seePlayer;
		vec2 _target;
		void* _owner;
		I32 _sweepCnt;
	};
} //namespace scene
} //namespace pbj
#endif