////////////////////////////////////////////////////////////////////////////////
/// \file   Z:\Documents\PBJsimple\src\pbj\audio\audio_source.cpp
///
/// \brief  Implements the audio source class.
////////////////////////////////////////////////////////////////////////////////
#include "pbj/audio/audio_source.h"

#include "pbj/scene/entity.h"

using pbj::scene::Entity;

namespace pbj {
namespace audio {

////////////////////////////////////////////////////////////////////////////////
/// \fn AudioSource::AudioSource(void* owner)
///
/// \brief  Constructor.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param [in] owner   The owner of the AudioSource.  This MUST be an Entity.
////////////////////////////////////////////////////////////////////////////////
AudioSource::AudioSource(void* owner)
{
    assert((Entity*)owner);
    _owner = owner;
    alGenSources(1, &_srcId);
    _buffers.reset(new AudioBuffers());
}

////////////////////////////////////////////////////////////////////////////////
/// \fn AudioSource::~AudioSource()
///
/// \brief  Destructor.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
AudioSource::~AudioSource()
{}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioSource::setPitch(F32 val)
///
/// \brief  Sets the pitch.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  val The value.
////////////////////////////////////////////////////////////////////////////////
void AudioSource::setPitch(F32 val)
{
    alSourcef(_srcId, AL_PITCH, val);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn F32 AudioSource::getPitch() const
///
/// \brief  Gets the pitch.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The pitch.
////////////////////////////////////////////////////////////////////////////////
F32 AudioSource::getPitch() const
{
    ALfloat ret;
    alGetSourcef(_srcId, AL_PITCH, &ret);
    return (F32)ret;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioSource::setGain(F32 val)
///
/// \brief  Sets the gain.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  val The value.
////////////////////////////////////////////////////////////////////////////////
void AudioSource::setGain(F32 val)
{
    alSourcef(_srcId, AL_GAIN, val);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn F32 AudioSource::getGain() const
///
/// \brief  Gets the gain.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The gain.
////////////////////////////////////////////////////////////////////////////////
F32 AudioSource::getGain() const
{
    ALfloat ret;
    alGetSourcef(_srcId, AL_GAIN, &ret);
    return (F32)ret;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioSource::setMaxDistance(F32 val)
///
/// \brief  Sets maximum distance before the source begins to falloff.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  val The value.
////////////////////////////////////////////////////////////////////////////////
void AudioSource::setMaxDistance(F32 val)
{
    alSourcef(_srcId, AL_MAX_DISTANCE, val);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn F32 AudioSource::getMaxDistance() const
///
/// \brief  Gets maximum distance.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The maximum distance.
////////////////////////////////////////////////////////////////////////////////
F32 AudioSource::getMaxDistance() const
{
    ALfloat ret;
    alGetSourcef(_srcId, AL_MAX_DISTANCE, &ret);
    return (F32)ret;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioSource::setRolloff(F32 val)
///
/// \brief  Sets the rolloff.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  val The value.
////////////////////////////////////////////////////////////////////////////////
void AudioSource::setRolloff(F32 val)
{
    alSourcef(_srcId, AL_ROLLOFF_FACTOR, val);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn F32 AudioSource::getRolloff() const
///
/// \brief  Gets the rolloff.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The rolloff.
////////////////////////////////////////////////////////////////////////////////
F32 AudioSource::getRolloff() const
{
    ALfloat ret;
    alGetSourcef(_srcId, AL_ROLLOFF_FACTOR, &ret);
    return (F32)ret;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioSource::setReferenceDistance(F32 val)
///
/// \brief  Sets reference distance.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  val The value.
////////////////////////////////////////////////////////////////////////////////
void AudioSource::setReferenceDistance(F32 val)
{
    alSourcef(_srcId, AL_REFERENCE_DISTANCE, val);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn F32 AudioSource::getReferenceDistance() const
///
/// \brief  Gets reference distance.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The reference distance.
////////////////////////////////////////////////////////////////////////////////
F32 AudioSource::getReferenceDistance() const
{
    ALfloat ret;
    alGetSourcef(_srcId, AL_REFERENCE_DISTANCE, &ret);
    return (F32)ret;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioSource::setMinMaxGain(vec2 val)
///
/// \brief  Sets minimum and maximum gain.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  val The value.
////////////////////////////////////////////////////////////////////////////////
void AudioSource::setMinMaxGain(vec2 val)
{
    ALfloat min = val.x;
    ALfloat max = val.y;
    alSourcef(_srcId, AL_MIN_GAIN, min);
    alSourcef(_srcId, AL_MAX_GAIN, max);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn vec2 AudioSource::getMinMaxGain() const
///
/// \brief  Gets minimum and maximum gain.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The minimum maximum gain.
////////////////////////////////////////////////////////////////////////////////
vec2 AudioSource::getMinMaxGain() const
{
    ALfloat min, max;
    alGetSourcef(_srcId, AL_MIN_GAIN, &min);
    alGetSourcef(_srcId, AL_MAX_GAIN, &max);
    return vec2((F32)min, (F32)max);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioSource::updatePosition()
///
/// \brief  Updates the position using the owners Transform.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void AudioSource::updatePosition()
{
    //since we're in 2d, we'll assume that z=0
    vec2 xy = ((Entity*)_owner)->getTransform().getPosition();
    alSource3f(_srcId, AL_POSITION, (ALfloat)xy.x, (ALfloat)xy.y, 0.0f);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioSource::updateVelocity()
///
/// \brief  Updates the velocity using the owners Rigidbody.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void AudioSource::updateVelocity()
{
    //since we're in 2d, we'll assume that z=0
    Entity* e = (Entity*)_owner;
    vec2 xy;
    if(e->getType() == Entity::EntityType::Camera)
    {
        xy = e->getCamera()->getTargetVelocity();
    }
    else if(e->getRigidbody())
    {
        xy = e->getRigidbody()->getVelocity();
    }
    else
    {
        xy = vec2(0.0f, 0.0f);
    }

    alSource3f(_srcId, AL_VELOCITY, (ALfloat)xy.x, (ALfloat)xy.y, 0.0f);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioSource::addAudioBuffer(string name, AudioBuffer* buffer)
///
/// \brief  Adds an audio buffer to the source.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  name            The name.
/// \param [in] buffer      The buffer to add.
////////////////////////////////////////////////////////////////////////////////
void AudioSource::addAudioBuffer(string name, AudioBuffer* buffer)
{
    _buffers->insert(std::pair<string,AudioBuffer*>(name,buffer));
}

////////////////////////////////////////////////////////////////////////////////
/// \fn AudioBuffer* AudioSource::getAudioBuffer(string name) const
///
/// \brief  Gets audio buffer.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  name    The name.
///
/// \return null if it fails, else the audio buffer.
////////////////////////////////////////////////////////////////////////////////
AudioBuffer* AudioSource::getAudioBuffer(string name) const
{
    AudioBuffers::iterator it = _buffers->find(name);
    if(it != _buffers->end())
        return (AudioBuffer* const)(it->second);
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn AudioSource::AudioBuffers* AudioSource::getAudioBuffers() const
///
/// \brief  Gets all audio buffers.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the audio buffers.
////////////////////////////////////////////////////////////////////////////////
AudioSource::AudioBuffers* AudioSource::getAudioBuffers() const
{
    return _buffers.get();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioSource::play(string name)
///
/// \brief  Plays the named AudioBuffer.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  name    The name.
////////////////////////////////////////////////////////////////////////////////
void AudioSource::play(string name)
{
    alSourcei(_srcId, AL_BUFFER, _buffers->at(name)->getBufferID());
    alSourcePlay(_srcId);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioSource::playAt(string name, F32 pos)
///
/// \brief  Play the named AudioBuffer at a passed position.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  name    The name.
/// \param  pos     The position in seconds.
////////////////////////////////////////////////////////////////////////////////
void AudioSource::playAt(string name, F32 pos)
{
    alSourcei(_srcId, AL_BUFFER, _buffers->at(name)->getBufferID());
    alSourcef(_srcId, AL_SEC_OFFSET, pos);
    alSourcePlay(_srcId);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioSource::stop()
///
/// \brief  Stops playback.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void AudioSource::stop()
{
    alSourceStop(_srcId);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioSource::pause()
///
/// \brief  Pauses playback.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void AudioSource::pause()
{
    alSourcePause(_srcId);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioSource::seek(F32 pos)
///
/// \brief  Seeks the given position.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  pos The position in seconds.
////////////////////////////////////////////////////////////////////////////////
void AudioSource::seek(F32 pos)
{
    alSourcef(_srcId, AL_SEC_OFFSET, pos);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void* AudioSource::getOwner() const
///
/// \brief  Gets the owner of this item.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the owner.
////////////////////////////////////////////////////////////////////////////////
void* AudioSource::getOwner() const { return _owner; }

} //namespace audio
} //namespace pbj
