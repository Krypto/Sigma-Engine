#include "AudioClip.h"
#include "SIG_Utilities.h"
#include "SoundSystem.h"

sig::AudioClip::AudioClip()
	:	m_volume(1),
		m_pitch(1),
		m_pan(0),
		m_name(""),
		m_loop(false),
		m_paused(false),
		m_is3D(false),
		m_playing(false),
		m_sample(0),
		m_position(Vector3::Zero)
{
}

sig::AudioClip::AudioClip(const string& fileName)
	:	sig::AudioClip()
{
	m_sample = OAL_Sample_Load(fileName);
}

sig::AudioClip::~AudioClip()
{
	if (m_sample) { OAL_Sample_Unload(m_sample); }
}


void sig::AudioClip::Play()
{
	if (m_paused) {
		if (m_sample != nullptr) {
			OAL_Source_SetPaused(m_source, false);
			m_paused = false;
		} else {
			m_paused = false;
			Play();
		}
	} else {
		m_source = OAL_Sample_Play(OAL_FREE, m_sample, m_volume, false, 10);
		m_playing = true;
		m_paused = false;
	}
}

void sig::AudioClip::Pause()
{
	if (m_playing) {
		OAL_Source_SetPaused(m_source, true);
		m_paused = true;
	}
}

void sig::AudioClip::Stop()
{
	if (m_playing || m_paused) {
		OAL_Source_Stop(m_source);
		m_playing = false;
	}
}

void sig::AudioClip::Update(SoundSystem* ss)
{
	m_playing = OAL_Source_IsPlaying(m_source);
	
	if (m_playing) {
		OAL_Source_SetLoop(m_source, m_loop);
		OAL_Source_SetGain(m_source, m_volume * ss->GetMasterVolume());
		OAL_Source_SetPitch(m_source, m_pitch);
		if (!m_is3D) {
			OAL_Source_SetPosition(m_source, new float[3] { m_pan, 0, 0 });
		} else {
			OAL_Source_SetPosition(m_source, new float[3] { m_position.X(), m_position.Y(), m_position.Z() });
		}
	}
}
