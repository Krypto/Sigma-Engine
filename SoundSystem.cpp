#include "SoundSystem.h"
#include "SIG_Utilities.h"

#include <OALWrapper/OAL_Sample.h>
#include <algorithm>

sig::SoundSystem::SoundSystem()
	:	m_masterVolume(1),
		m_valid(false)
{
}

sig::SoundSystem::~SoundSystem()
{
	OAL_Source_Stop(OAL_ALL);
	OAL_Close();
}

void sig::SoundSystem::Initialize()
{
	// OAL_SetupLogging(true, eOAL_LogOutput_File, eOAL_LogVerbose_High);
	
	cOAL_Init_Params params;
	
	if (OAL_Init(params) == false) {
		SIG_LOG_ERROR("Could not initialize OAL. Check your OpenAL installation and try again.");
		m_valid = false;
	} else {
		m_valid = true;
	}
}

void sig::SoundSystem::Update()
{
	if (!m_valid) { return; }

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
	if (!m_valid) { return; }

	if (ac == nullptr) {
		SIG_LOG_ERROR("The AudioClip object must not be null.");
	} else {
		ac->Play();
		m_playing.push_back(ac);
	}
}

void sig::SoundSystem::Play(const string &name)
{
	if (!m_valid) { return; }

	AudioClipMap::iterator pos = m_library.find(name);
	if (pos != m_library.end()) {
		AudioClip *clip = m_library.at(name);
		Play(clip);
	} else {
		SIG_LOG_ERROR("There's no AudioClip named \"" << name << "\"");
	}
}

void sig::SoundSystem::AddToLibrary(const string &name, sig::AudioClip *audio_clip)
{
	if (!m_valid) { return; }

	AudioClipMap::iterator pos = m_library.find(name);
	if (pos == m_library.end()) {
		m_library.insert({name, audio_clip});
	} else {
		SIG_LOG_ERROR("An AudioClip with this name (\"" << name << "\") already exists.");
	}
}

sig::AudioClip *sig::SoundSystem::GetAudioClipFromLibrary(const string &name)
{
	if (!m_valid) { return nullptr; }

	AudioClipMap::iterator pos = m_library.find(name);
	if (pos != m_library.end()) {
		return m_library.at(name);
	}
	return nullptr;
}
