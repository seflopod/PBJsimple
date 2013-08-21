#ifndef AUDIO_LISTENER_H_
#define AUDIO_LISTENER_H_

#include <assert.h>
#include "pbj/_al.h"
#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj
{
namespace audio
{
	
	class AudioListener
	{
	public:
		AudioListener(void*);
		~AudioListener();
		
		void updatePosition();
		void updateVelocity();
		
		void changeOrientation(vec3);
		vec3 getOrientation() const;

		void setGain(F32);
		F32 getGain() const;
		
		void* getOwner() const;

	private:
		void* _owner;
		vec3 _up;
		F32 _gain;
	};

} //namespace audio
} //namespace pbj
#endif