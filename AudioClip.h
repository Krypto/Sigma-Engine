#ifndef SIGMA_AUDIO_CLIP
#define SIGMA_AUDIO_CLIP

#include "Vector.h"

#include <OALWrapper/OAL_Funcs.h>
#include <OALWrapper/OAL_Sample.h>

#include <string>
#include <vector>
using namespace std;

namespace sig
{
	using namespace math;
	
	class SoundSystem;
	class AudioClip
	{
		friend class SoundSystem;
	public:
		AudioClip();
		AudioClip(const string &fileName);
		~AudioClip();
		
		/**
		 * @brief Play/Continue the Audio Clip
		 */
		void Play();
		
		/**
		 * @brief Pause the Audio Clip
		 */
		void Pause();
		
		/**
		 * @brief Stop the Audio Clip
		 */
		void Stop();
		
		void Update(SoundSystem *ss);
		
		bool Is3D() const { return m_is3D; }
		void SetIs3D(bool is3D) { this->m_is3D = is3D; }

		bool IsLoop() const { return m_loop; }
		void SetLoop(bool loop) { this->m_loop = loop; }

		float GetPan() const { return m_pan; }
		void SetPan(float pan) { this->m_pan = pan; }

		float GetPitch() const { return m_pitch; }
		void SetPitch(float pitch) { this->m_pitch = pitch; }

		Vector3& GetPosition() { return m_position; }
		void SetPosition(const Vector3& position) { this->m_position = position; }

		float GetVolume() const { return m_volume; }
		void SetVolume(float volume) { this->m_volume = volume; }

		const string& GetName() const { return m_name; }
		bool IsPaused() const { return m_paused; }
		bool IsPlaying() const { return m_playing; }
	
	protected:
		float m_volume, m_pitch, m_pan;
		string m_name;
		bool m_loop, m_paused, m_is3D, m_playing;
		cOAL_Sample *m_sample;
		Vector3 m_position;
		int m_source;
	};

}

#endif // SIGMA_AUDIO_CLIP
