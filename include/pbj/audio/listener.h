////////////////////////////////////////////////////////////////////////////////
/// \file   pbj\audio\listener.h
///
/// \brief  Declares the audio source class.

#ifndef PBJ_AUDIO_LISTENER_H_
#define PBJ_AUDIO_LISTENER_H_

#include <assert.h>
#include "pbj/_al.h"
#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj {
namespace scene {

class Entity;

} // namespace pbj::scene
namespace audio {

////////////////////////////////////////////////////////////////////////////
/// \class  Listener
///
/// \brief  Audio listener.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \details    Wraps OpenAL for alListener* functions.  To make this work
///             within our architecture I had to make the position and
///             velocity updates more automatic, but they work.  I left in
///             the ability to change the orientation of the AudioListener,
///             but I do not think we will need to use it.
class Listener
{
public:
    Listener(scene::Entity*);
    ~Listener();

    void updatePosition();
    void updateVelocity();

    void changeOrientation(vec3);
    vec3 getOrientation() const;

    void setGain(F32);
    F32 getGain() const;

    scene::Entity* getOwner() const;

private:
    scene::Entity* _owner;
    vec3 _up;
    F32 _gain;

    Listener(const Listener&);
    void operator=(const Listener&);
};

} //namespace audio
} //namespace pbj

#endif
