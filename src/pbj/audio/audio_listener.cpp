////////////////////////////////////////////////////////////////////////////////
/// \file   Z:\Documents\PBJsimple\src\pbj\audio\audio_listener.cpp
///
/// \brief  Implements the audio listener class.
////////////////////////////////////////////////////////////////////////////////
#include "pbj/audio/audio_listener.h"

#include "pbj/scene/entity.h"

using pbj::scene::Entity;

namespace pbj {
namespace audio {

////////////////////////////////////////////////////////////////////////////////
/// \fn AudioListener::AudioListener(void* owner)
///
/// \brief  Constructor.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param [in] owner   If non-null, the owner.  This MUST be an Entity.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn AudioListener::~AudioListener()
///
/// \brief  Destructor.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
AudioListener::~AudioListener()
{}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioListener::updatePosition()
///
/// \brief  Updates the position using the owner's Transform.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void AudioListener::updatePosition()
{
    //since we're in 2d, we'll assume that z=0
    vec2 xy = ((Entity*)_owner)->getTransform().getPosition();
    alListener3f(AL_POSITION, (ALfloat)xy.x, (ALfloat)xy.y, 0.0f);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioListener::updateVelocity()
///
/// \brief  Updates the velocity using data from our owner.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void AudioListener::updateVelocity()
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
    alListener3f(AL_VELOCITY, (ALfloat)xy.x, (ALfloat)xy.y, 0.0f);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioListener::changeOrientation(vec3 up)
///
/// \brief  Change orientation.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  up  The up vector.
////////////////////////////////////////////////////////////////////////////////
void AudioListener::changeOrientation(vec3 up)
{
    //have to use intermediate array because AL_ORIENTATION only takes arrays
    _up = up;
    ALfloat dir[3] = { _up.x, _up.y, _up.z };
    alListenerfv(AL_ORIENTATION, dir);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn vec3 AudioListener::getOrientation() const
///
/// \brief  Gets the orientation.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The orientation.
////////////////////////////////////////////////////////////////////////////////
vec3 AudioListener::getOrientation() const { return _up; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void AudioListener::setGain(F32 gain)
///
/// \brief  Sets the gain.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  gain    The gain.
////////////////////////////////////////////////////////////////////////////////
void AudioListener::setGain(F32 gain)
{
    if(gain < 0)
        return;

    _gain = gain;
    alListenerf(AL_GAIN, _gain);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn F32 AudioListener::getGain() const
///
/// \brief  Gets the gain.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The gain.
////////////////////////////////////////////////////////////////////////////////
F32 AudioListener::getGain() const { return _gain; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void* AudioListener::getOwner() const
///
/// \brief  Gets the owner of this item.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the owner.
////////////////////////////////////////////////////////////////////////////////
void* AudioListener::getOwner() const { return _owner; }

} //namespace audio
} //namespace pbj
