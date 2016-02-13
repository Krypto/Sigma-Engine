#ifndef SIGMA_SOUND_SYSTEM
#define SIGMA_SOUND_SYSTEM

#include "AudioClip.h"

#include <OALWrapper/OAL_Funcs.h>
#include <vector>
using namespace std;

namespace sig
{
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
		
		void SetMasterVolume(float masterVolume) {this->m_masterVolume = masterVolume;}
		float GetMasterVolume() const {return m_masterVolume;}
		
	private:
		float m_masterVolume;
		vector<AudioClip*> m_playing;
		vector<AudioClip*> m_remove;
	};
}

#endif // SIGMA_SOUND_SYSTEM
