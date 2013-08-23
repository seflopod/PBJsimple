////////////////////////////////////////////////////////////////////////////////
/// \file   Z:\Documents\PBJsimple\include\pbj\audio\audio_source.h
///
/// \brief  Declares the audio source class.
////////////////////////////////////////////////////////////////////////////////
#ifndef PBJ_AUDIO_SOURCE_H_
#define PBJ_AUDIO_SOURCE_H_

#include <assert.h>
#include <map>
#include <string>
#include <memory>

#include "pbj/_pbj.h"
#include "pbj/_al.h"
#include "pbj/_math.h"
#include "pbj/audio/buffer.h"

namespace pbj {
namespace scene {

class Entity;

} // namespace pbj::scene
namespace audio {

////////////////////////////////////////////////////////////////////////////
/// \class  AudioSource
///
/// \brief  Audio source.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \details    AudioSource provide functions for wrapping around OpenAL's
///             alSource* functions.  I didn't provide all of the same
///             functionality (such as cone-directed sounds) because we did
///             not appear to need them.
////////////////////////////////////////////////////////////////////////////
class Source
{
public:
    Source(scene::Entity* owner);
    ~Source();

    void setPitch(F32);
    F32 getPitch() const;

    void setGain(F32);
    F32 getGain() const;

    void setMaxDistance(F32);
    F32 getMaxDistance() const;

    void setRolloff(F32);
    F32 getRolloff() const;

    void setReferenceDistance(F32);
    F32 getReferenceDistance() const;

    void setMinMaxGain(vec2);
    vec2 getMinMaxGain() const;

    //ignoring cone-related properties
    //but I feel like direction should be set to z=1 if we have issues

    void updatePosition();
    void updateVelocity();

    void addBuffer(const std::string&, Buffer*);
    Buffer* getBuffer(const std::string&);
    void play(const std::string&);
    void playAt(const std::string&, F32);
    void stop();
    void pause();
    void seek(F32);

    const std::string& getPlaying() const;

    scene::Entity* getOwner() const;
private:
    scene::Entity* _owner;
    ALuint _srcId;
    std::unordered_map<std::string, Buffer*> _buffers;
    std::string _curPlaying;

    Source(const Source&);
    void operator=(const Source&);
};

} //namespace audio
} //namesapce pbj

#endif
