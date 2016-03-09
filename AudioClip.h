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
		
		void SetIs3D(bool is3D) {this->m_is3D = is3D;}
		void SetLoop(bool loop) {this->m_loop = loop;}
		void SetPan(float pan) {this->m_pan = pan;}
		void SetPitch(float pitch) {this->m_pitch = pitch;}
		void SetPosition(const Vector3& position) {this->m_position = position;}
		void SetVolume(float volume) {this->m_volume = volume;}
		bool Is3D() const {return m_is3D;}
		bool IsLoop() const {return m_loop;}
		const string& GetName() const {return m_name;}
		float GetPan() const {return m_pan;}
		bool IsPaused() const {return m_paused;}
		float GetPitch() const {return m_pitch;}
		bool IsPlaying() const {return m_playing;}
		Vector3& GetPosition() {return m_position;}
		float GetVolume() const {return m_volume;}
	
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
