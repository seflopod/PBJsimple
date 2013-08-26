///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/audio/buffer.h
/// \author Josh Douglas
///
/// \brief  pbj::audio::Buffer class header.

#ifndef PBJ_AUDIO_BUFFER_H_
#define PBJ_AUDIO_BUFFER_H_

#include "pbj/_al.h"
#include "pbj/_pbj.h"

#include "pbj/sw/sandwich.h"


namespace pbj {
namespace audio {

////////////////////////////////////////////////////////////////////////////////
/// \class  Buffer
///
/// \brief  Audio element.
///
/// \author Josh Douglas
/// \date   2013-08-22
///
/// \details Wraps OpenAL audio buffers.
class Buffer
{
public:
    Buffer(const ALubyte* data, size_t size);
    ~Buffer();

    ALuint getBufferID();

private:
    ALuint buffer_id_;

    Buffer(const Buffer&);
    void operator=(const Buffer&);
};

std::unique_ptr<Buffer> loadSound(sw::Sandwich& sandwich, const Id& id);

} // namespace pbj::audio
} // namespace pbj

#endif
