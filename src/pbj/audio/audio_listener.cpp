#ifndef AUDIO_LISTENER_H_
#include "pbj/audio/audio_listener.h"
#endif

#include "pbj/scene/entity.h"

using pbj::scene::Entity;

namespace pbj
{
namespace audio
{
AudioListener::AudioListener(void* owner)
{
	assert((Entity*)owner);
	_owner = owner;
	
	//just assume that y=1 is up
	changeOrientation(vec3(0.0f, 1.0f, 0.0f));

	//guessing at a default value for gain.
	//info I found says that it is an amplitude multiplier, so I assume that
	//means that 1.0f should multiply by 1.
	setGain(1.0f);
}

AudioListener::~AudioListener()
{}

void AudioListener::updatePosition()
{
	//since we're in 2d, we'll assume that z=0
	vec2 xy = ((Entity*)_owner)->getTransform().getPosition();
	alListener3f(AL_POSITION, (ALfloat)xy.x, (ALfloat)xy.y, 0.0f);
}

void AudioListener::updateVelocity()
{
	//since we're in 2d, we'll assume that z=0
	vec2 xy = ((Entity*)_owner)->getRigidbody()->getVelocity();
	alListener3f(AL_VELOCITY, (ALfloat)xy.x, (ALfloat)xy.y, 0.0f);
}

void AudioListener::changeOrientation(vec3 up)
{
	//have to use intermediate array because AL_ORIENTATION only takes arrays
	_up = up;
	ALfloat dir[3] = { _up.x, _up.y, _up.z };
	alListenerfv(AL_ORIENTATION, dir);
}

vec3 AudioListener::getOrientation() const { return _up; }

void AudioListener::setGain(F32 gain)
{
	if(gain < 0)
		return;

	_gain = gain;
	alListenerf(AL_GAIN, _gain);
}

F32 AudioListener::getGain() const { return _gain; }

void* getOwner() const { return _owner; }
} //namespace audio
} //namespace pbj