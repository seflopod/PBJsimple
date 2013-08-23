///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/audio/buffer.cpp
/// \author Josh Douglas
///
/// \brief  pbj::audio::Buffer class source.

#include "pbj/audio/buffer.h"

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load a audio buffer from a sandwich.
/// \param  1 The id of the texture.
#define PBJSQL_LOAD "SELECT data " \
            "FROM sw_sounds WHERE id = ?"

#ifdef BE_ID_NAMES_ENABLED
#define PBJSQLID_LOAD PBJSQL_LOAD
#else
// TODO: precalculate ids.
#define PBJSQLID_LOAD PBJSQL_LOAD
#endif

namespace pbj {
namespace audio {

////////////////////////////////////////////////////////////////////////////////
/// \fn AudioBuffer::AudioBuffer(const ALubyte* data, size_t size)
///
/// \brief  Constructor.
///
/// \author Josh Douglas
/// \date   2013-08-22
///
/// \param  data    The data.
/// \param  size    The size.
////////////////////////////////////////////////////////////////////////////////
Buffer::Buffer(const ALubyte* data, size_t size)
{
    buffer_id_ = alutCreateBufferFromFileImage(data, size);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn AudioBuffer::~AudioBuffer()
///
/// \brief  Destructor.
///
/// \author Josh Douglas
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
Buffer::~Buffer()
{
}

////////////////////////////////////////////////////////////////////////////////
/// \fn ALuint AudioBuffer::getBufferID()
///
/// \brief  Gets buffer identifier.
///
/// \author Josh Douglas
/// \date   2013-08-22
///
/// \return The buffer identifier.
////////////////////////////////////////////////////////////////////////////////
ALuint Buffer::getBufferID()
{
	return buffer_id_;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn std::unique_ptr<AudioBuffer> loadSound(sw::Sandwich& sandwich,
///     const Id& id)
///
/// \brief  Loads a sound.
///
/// \author Ben Crist
/// \date   2013-08-22
///
/// \exception  std::runtime_error  Thrown when a runtime error error
///                                 condition occurs.
///
/// \param [in,out] sandwich    The sandwich.
/// \param  id                  The identifier.
///
/// \return The sound.
////////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Buffer> loadSound(sw::Sandwich& sandwich, const Id& id)
{
    std::unique_ptr<Buffer> result;
   
    try
    {
        db::StmtCache& cache = sandwich.getStmtCache();
        db::CachedStmt stmt = cache.hold(Id(PBJSQLID_LOAD), PBJSQL_LOAD);

        stmt.bind(1, id.value());
        if (stmt.step())
        {
            const void* data;
            size_t data_length = stmt.getBlob(0, data);

            result.reset(new Buffer(static_cast<const ALubyte*>(data), data_length));
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
