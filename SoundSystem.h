#ifndef SIGMA_SOUND_SYSTEM
#define SIGMA_SOUND_SYSTEM

#include "AudioClip.h"

#include <OALWrapper/OAL_Funcs.h>

#include <map>
#include <vector>
#include <string>
using namespace std;

namespace sig
{
	typedef map<string, AudioClip*> AudioClipMap;

	class SoundSystem
	{
	public:
		SoundSystem();
		~SoundSystem();
		
		void Initialize();
		void Update();
		
		/**
		 * @brief Play an AudioClip object
		 * @param ac non-null Audio Clip
		 */
		void Play(AudioClip *ac);

		/**
		 * @brief Play an AudioClip object from the library
		 * @param name Clip name, unique
		 */
		void Play(const string &name);

		/**
		 * @brief Add an audio clip to the audio library
		 * @param name Clip name, unique
		 * @param audio_clip Audio clip object. Not null
		 */
		void AddToLibrary(const string &name, AudioClip* audio_clip);
		
		/**
		 * @brief Get an AudioClip object from the library
		 * @param name Clip name, unique
		 * @return Null if the AudioClip doesn't exist, AudioClip* otherwise
		 */
		AudioClip *GetAudioClipFromLibrary(const string &name);

		void SetMasterVolume(float masterVolume) {this->m_masterVolume = masterVolume;}
		float GetMasterVolume() const {return m_masterVolume;}
		
	private:
		float m_masterVolume;
		bool m_valid;
		vector<AudioClip*> m_playing, m_remove;
		AudioClipMap m_library;
	};
}

#endif // SIGMA_SOUND_SYSTEM
