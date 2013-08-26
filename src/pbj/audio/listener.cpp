////////////////////////////////////////////////////////////////////////////////
/// \file   pbj\audio\audio_listener.cpp
///
/// \brief  Implements the audio listener class.
#include "pbj/audio/listener.h"

#include "pbj/scene/entity.h"

namespace pbj {
namespace audio {

////////////////////////////////////////////////////////////////////////////////
/// \fn Listener::Listener(Entity* owner)
///
/// \brief  Constructor.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param [in] owner   If non-null, the owner.  This MUST be an Entity.
Listener::Listener(scene::Entity* owner)
    : _owner(owner)
{
    //just assume that y=1 is up
    changeOrientation(vec3(0.0f, 1.0f, 0.0f));

    //guessing at a default value for gain.
    //info I found says that it is an amplitude multiplier, so I assume that
    //means that 1.0f should multiply by 1.
    setGain(1.0f);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Listener::~Listener()
///
/// \brief  Destructor.
///
/// \author Peter Bartosch
/// \date   2013-08-22
Listener::~Listener()
{}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Listener::updatePosition()
///
/// \brief  Updates the position using the owner's Transform.
///
/// \author Peter Bartosch
/// \date   2013-08-22
void Listener::updatePosition()
{
    //since we're in 2d, we'll assume that z=0
    vec2 xy = _owner->getTransform().getPosition();
    alListener3f(AL_POSITION, (ALfloat)xy.x, (ALfloat)xy.y, 0.0f);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Listener::updateVelocity()
///
/// \brief  Updates the velocity using data from our owner.
///
/// \author Peter Bartosch
/// \date   2013-08-22
void Listener::updateVelocity()
{
    //since we're in 2d, we'll assume that z=0
    vec2 xy;
    if(_owner->getType() == scene::Entity::Camera)
    {
        xy = _owner->getCamera()->getTargetVelocity();
    }
    else if(_owner->getRigidbody())
    {
        xy = _owner->getRigidbody()->getVelocity();
    }
    else
    {
        xy = vec2(0.0f, 0.0f);
    }
    alListener3f(AL_VELOCITY, (ALfloat)xy.x, (ALfloat)xy.y, 0.0f);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Listener::changeOrientation(vec3 up)
///
/// \brief  Change orientation.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  up  The up vector.
void Listener::changeOrientation(vec3 up)
{
    //have to use intermediate array because AL_ORIENTATION only takes arrays
    _up = up;
    ALfloat dir[3] = { _up.x, _up.y, _up.z };
    alListenerfv(AL_ORIENTATION, dir);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn vec3 Listener::getOrientation() const
///
/// \brief  Gets the orientation.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The orientation.
vec3 Listener::getOrientation() const { return _up; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Listener::setGain(F32 gain)
///
/// \brief  Sets the gain.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  gain    The gain.
void Listener::setGain(F32 gain)
{
    if(gain < 0)
        return;

    _gain = gain;
    alListenerf(AL_GAIN, _gain);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn F32 Listener::getGain() const
///
/// \brief  Gets the gain.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The gain.
F32 Listener::getGain() const { return _gain; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void* Listener::getOwner() const
///
/// \brief  Gets the owner of this item.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the owner.
scene::Entity* Listener::getOwner() const { return _owner; }

} //namespace audio
} //namespace pbj
