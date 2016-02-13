#include "SoundSystem.h"
#include "SIG_Utilities.h"

#include <OALWrapper/OAL_Sample.h>
#include <algorithm>

sig::SoundSystem::SoundSystem()
	:	m_masterVolume(1)
{
}

sig::SoundSystem::~SoundSystem()
{
	OAL_Source_Stop(OAL_ALL);
	OAL_Close();
}

void sig::SoundSystem::Initialize()
{
	OAL_SetupLogging(true, eOAL_LogOutput_File, eOAL_LogVerbose_High);
	
	cOAL_Init_Params params;
	
	if (OAL_Init(params) == false) {
		SIG_LOG_ERROR("Could not initialize OAL. Check your OpenAL installation and try again.");
		return;
	}
}

void sig::SoundSystem::Update()
{
	for (auto it = m_playing.begin(); it != m_playing.end(); ++it) {
		auto ac = (*it);
		ac->Update(this);
		
		if (!ac->IsPlaying()) {
			m_remove.push_back(ac);
		}
	}
	
	if (m_remove.size() > 0) {
		for (auto it = m_remove.begin(); it != m_remove.end(); ++it) {
			auto pos = std::find(m_playing.begin(), m_playing.end(), (*it));
			if (pos != m_playing.end()) {
				m_playing.erase(pos);
			}
		}
		m_remove.clear();
	}
}

void sig::SoundSystem::Play(AudioClip* ac)
{
	AudioClip *cp = new AudioClip();
	cp->m_name = ac->m_name;
	cp->m_sample = ac->m_sample;
	cp->m_volume = ac->m_volume;
	cp->m_pitch = ac->m_pitch;
	cp->m_pan = ac->m_pan;
	cp->m_loop = ac->m_loop;
	cp->m_paused = ac->m_paused;
	cp->m_is3D = ac->m_is3D;
	cp->m_playing = ac->m_playing;
	cp->m_position = Vector3(ac->m_position);
	
	cp->Play();
	
	m_playing.push_back(cp);
}
