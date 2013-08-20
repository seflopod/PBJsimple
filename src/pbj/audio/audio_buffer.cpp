///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/audio/audio_buffer.cpp
/// \author Josh Douglas
///
/// \brief  pbj::audio::AudioBuffer class source.

#include "pbj/audio/audio_buffer.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load a audio buffer from a sandwich.
/// \param  1 The id of the texture.
#define PBJ_AUDIO_AUDIO_BUFFER_SQL_LOAD "SELECT data, " \
            "FROM sw_sounds WHERE id = ?"

#ifdef BE_ID_NAMES_ENABLED
#define PBJ_AUDIO_AUDIO_BUFFER_SQLID_LOAD PBJ_AUDIO_AUDIO_BUFFER_SQL_LOAD
#else
// TODO: precalculate ids.
#define PBJ_AUDIO_AUDIO_BUFFER_SQLID_LOAD PBJ_AUDIO_AUDIO_BUFFER_SQL_LOAD
#endif

namespace pbj {
namespace audio {

AudioBuffer::AudioBuffer(const ALubyte* data, size_t size)
{
    buffer_id_ = alutCreateBufferFromFileImage(data, size);
}

AudioBuffer::~AudioBuffer()
{
}

ALuint AudioBuffer::getBufferID()
{
	return buffer_id_;
}

std::unique_ptr<AudioBuffer> loadSound(sw::Sandwich& sandwich, const Id& id)
{
    std::unique_ptr<AudioBuffer> result;
   
    try
    {
        db::StmtCache& cache = sandwich.getStmtCache();
        db::CachedStmt stmt = cache.hold(Id(PBJ_AUDIO_AUDIO_BUFFER_SQLID_LOAD), PBJ_AUDIO_AUDIO_BUFFER_SQL_LOAD);

        stmt.bind(1, id.value());
        if (stmt.step())
        {
            const void* data;
            size_t data_length = stmt.getBlob(0, data);

            result.reset(new AudioBuffer(static_cast<const ALubyte*>(data), data_length));
        }
        else
            throw std::runtime_error("Sound not found!");
    }
    catch (const db::Db::error& err)
    {
        PBJ_LOG(VWarning) << "Database error while loading sound!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "   Sound ID: " << id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_NL
                          << "        SQL: " << err.sql() << PBJ_LOG_END;
   }
   catch (const std::exception& err)
   {
      PBJ_LOG(VWarning) << "Exception while loading sound!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "   Sound ID: " << id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_END;
   }

   return result;
}

} // namespace pbj::audio
} // namespace pbj
