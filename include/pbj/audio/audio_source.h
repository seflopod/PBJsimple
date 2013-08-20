#ifndef AUDIO_SOURCE_H_
#define AUDIO_SOURCE_H_

#include <assert.h>
#include <map>
#include <string>
#include <memory>

#include "pbj/_pbj.h"
#include "pbj/_al.h"
#include "pbj/_math.h"
#include "pbj/audio/audio_buffer.h"

using std::unordered_map;
using std::string;

namespace pbj
{
namespace audio
{
	class AudioSource
	{
	public:
		typedef unordered_map<string, AudioBuffer*> AudioBuffers;
		
		AudioSource(void*);
		~AudioSource();
		
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
		
		void addAudioBuffer(string, AudioBuffer*);
		AudioBuffer* getAudioBuffer(string) const;
		AudioBuffers* getAudioBuffers() const;
		void play(string);
		void playAt(string, F32);
		void stop();
		void pause();
		void seek(F32);
		
		string getPlaying() const;
	private:
		void* _owner;
		ALuint _srcId;
		std::unique_ptr<AudioBuffers> _buffers;
		string _curPlaying;
	};
} //namespace audio
} //namesapce pbj
#endif