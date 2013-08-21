#ifndef AUDIO_SOURCE_H_
#include "pbj/audio/audio_source.h"
#endif

#include "pbj/scene/entity.h"

using pbj::scene::Entity;

namespace pbj
{
namespace audio
{
AudioSource::AudioSource(void* owner)
{
	assert((Entity*)owner);
	_owner = owner;
	alGenSources(1, &_srcId);
	_buffers.reset(new AudioBuffers());
}

AudioSource::~AudioSource()
{}

void AudioSource::setPitch(F32 val)
{
	alSourcef(_srcId, AL_PITCH, val);
}

F32 AudioSource::getPitch() const
{
	ALfloat ret;
	alGetSourcef(_srcId, AL_PITCH, &ret);
	return (F32)ret;
}

void AudioSource::setGain(F32 val)
{
	alSourcef(_srcId, AL_GAIN, val);
}

F32 AudioSource::getGain() const
{
	ALfloat ret;
	alGetSourcef(_srcId, AL_GAIN, &ret);
	return (F32)ret;
}

void AudioSource::setMaxDistance(F32 val)
{
	alSourcef(_srcId, AL_MAX_DISTANCE, val);
}

F32 AudioSource::getMaxDistance() const
{
	ALfloat ret;
	alGetSourcef(_srcId, AL_MAX_DISTANCE, &ret);
	return (F32)ret;
}

void AudioSource::setRolloff(F32 val)
{
	alSourcef(_srcId, AL_ROLLOFF_FACTOR, val);
}

F32 AudioSource::getRolloff() const
{
	ALfloat ret;
	alGetSourcef(_srcId, AL_ROLLOFF_FACTOR, &ret);
	return (F32)ret;
}

void AudioSource::setReferenceDistance(F32 val)
{
	alSourcef(_srcId, AL_REFERENCE_DISTANCE, val);
}

F32 AudioSource::getReferenceDistance() const
{
	ALfloat ret;
	alGetSourcef(_srcId, AL_REFERENCE_DISTANCE, &ret);
	return (F32)ret;
}

void AudioSource::setMinMaxGain(vec2 val)
{
	ALfloat min = val.x;
	ALfloat max = val.y;
	alSourcef(_srcId, AL_MIN_GAIN, min);
	alSourcef(_srcId, AL_MAX_GAIN, max);
}

vec2 AudioSource::getMinMaxGain() const
{
	ALfloat min, max;
	alGetSourcef(_srcId, AL_MIN_GAIN, &min);
	alGetSourcef(_srcId, AL_MAX_GAIN, &max);
	return vec2((F32)min, (F32)max);
}

void AudioSource::updatePosition()
{
	//since we're in 2d, we'll assume that z=0
	vec2 xy = ((Entity*)_owner)->getTransform().getPosition();
	alSource3f(_srcId, AL_POSITION, (ALfloat)xy.x, (ALfloat)xy.y, 0.0f);
}

void AudioSource::updateVelocity()
{
	//since we're in 2d, we'll assume that z=0
	vec2 xy = ((Entity*)_owner)->getRigidbody()->getVelocity();
	alSource3f(_srcId, AL_VELOCITY, (ALfloat)xy.x, (ALfloat)xy.y, 0.0f);
}

void AudioSource::addAudioBuffer(string name, AudioBuffer* buffer)
{
	_buffers->at(name) = buffer;
}

AudioBuffer* AudioSource::getAudioBuffer(string name) const
{
	AudioBuffers::iterator it = _buffers->find(name);
	if(it != _buffers->end())
		return (AudioBuffer* const)(it->second);
	return nullptr;
}

AudioSource::AudioBuffers* AudioSource::getAudioBuffers() const
{
	return _buffers.get();
}

void AudioSource::play(string name)
{
	alSourcei(_srcId, AL_BUFFER, _buffers->at(name)->getBufferID());
	alSourcePlay(_srcId);
}

void AudioSource::playAt(string name, F32 pos)
{
	alSourcei(_srcId, AL_BUFFER, _buffers->at(name)->getBufferID());
	alSourcef(_srcId, AL_SEC_OFFSET, pos);
	alSourcePlay(_srcId);
}

void AudioSource::stop()
{
	alSourceStop(_srcId);
}

void AudioSource::pause()
{
	alSourcePause(_srcId);
}

void AudioSource::seek(F32 pos)
{
	alSourcef(_srcId, AL_SEC_OFFSET, pos);
}

void* AudioSource::getOwner() const { return _owner; }

} //namespace audio
} //namespace pbj