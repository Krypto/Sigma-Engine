#include "SoundSystem.h"
#include "SIG_Utilities.h"
#include "Node.h"

#include <OALWrapper/OAL_Sample.h>
#include <algorithm>

bool sig::AudioSource::OAL_Initialized = true;

sig::AudioSource::AudioSource()
{
}

sig::AudioSource::~AudioSource()
{
	if (OAL_Initialized) {
		OAL_Source_Stop(OAL_ALL);
		OAL_Close();
	}
}

void sig::AudioSource::Initialize()
{
	cOAL_Init_Params p;
	if (OAL_Init(p) == false) {
		OAL_Initialized = false;
		SIG_LOG_ERROR("Could not initialize OAL. Check your OpenAL installation and try again.");
	}

	if (OAL_Initialized) {
		SIG_FOREACH(it, m_library)
		{
			AudioSample *s = it->second;
			if (!s->m_loaded) {
				s->Reload();
			}
		}
	}
}

void sig::AudioSource::Update(float dt)
{
	SIG_FOREACH(it, m_playing)
	{
		AudioObject *ob = *it;
		ob->Update(GetOwner()->GetPosition());
		if (!ob->playing) {
			m_remove.push_back(ob);
		}
	}

	if (m_remove.size() > 0) {
		SIG_FOREACH(it, m_remove)
		{
			auto pos = std::find(m_playing.begin(), m_playing.end(), (*it));
			if (pos != m_playing.end()) {
				m_playing.erase(pos);
			}
		}
		m_remove.clear();
	}
}

void sig::AudioSource::AddSample(const string &name, sig::AudioSample *sample)
{
	if (!sample) { return; }
	if (!OAL_Initialized) { return; }

	AudioSampleList::iterator pos = m_library.find(name);
	if (pos == m_library.end()) {
		m_library.insert({name, sample});
	} else {
		SIG_LOG_ERROR("An AudioSample with the name (\"" << name << "\") already exists.");
	}
}

sig::AudioSample *sig::AudioSource::GetSample(const string &name)
{
	if (!OAL_Initialized) { return nullptr; }

	AudioSampleList::iterator pos = m_library.find(name);
	if (pos != m_library.end()) {
		return m_library.at(name);
	} else {
		SIG_LOG_ERROR("There's no AudioSample named \"" << name << "\"");
	}
	return nullptr;
}

sig::AudioObject *sig::AudioSource::Play(sig::AudioSample *sample)
{
	if (!sample) { return nullptr; }
	AudioObject *ob = new AudioObject();
	ob->volume = 1;
	ob->pitch = 1;
	ob->pan = 0;
	ob->is3D = false;
	ob->loop = false;
	ob->source = -1;

	ob->Play(sample);

	m_playing.push_back(ob);

	return ob;
}

sig::AudioObject *sig::AudioSource::Play(const string &name)
{
	if (!OAL_Initialized) { return nullptr; }

	return Play(GetSample(name));
}

sig::AudioSource *sig::AudioSource::GetInstance(sig::Node *owner)
{
	AudioSource *inst = new AudioSource();
	inst->m_owner = owner;
	SIG_FOREACH(it, m_library)
	{
		AudioSample *s = it->second;
		inst->m_library.insert({it->first, new AudioSample(s->GetData(), s->GetSize())});
	}
	return inst;
}

sig::AudioSample::AudioSample(void *data, u32 size)
{
	m_data = data;
	m_size = size;
	m_loaded = false;
}

sig::AudioSample::~AudioSample()
{
	if (m_sample) { OAL_Sample_Unload(m_sample); }
}

void sig::AudioSample::Reload()
{
	m_loaded = false;
	if (m_data) {
		m_sample = OAL_Sample_LoadFromBuffer(m_data, m_size);
		m_loaded = true;
	}
}

void sig::AudioObject::Play(AudioSample *sample)
{
	if (paused) {
		if (sample && sample->GetSample() != nullptr) {
			OAL_Source_SetPaused(source, false);
			paused = false;
		} else {
			paused = false;
			Play(sample);
		}
	} else {
		source = OAL_Sample_Play(OAL_FREE, sample->GetSample(), volume, false, 10);
		playing = true;
		paused = false;
	}
}

void sig::AudioObject::Pause()
{
	if (playing) {
		OAL_Source_SetPaused(source, true);
		paused = true;
	}
}

void sig::AudioObject::Stop()
{
	if (playing || paused) {
		OAL_Source_Stop(source);
		playing = false;
	}
}

void sig::AudioObject::Update(const Vector2 &pos)
{
	playing = OAL_Source_IsPlaying(source);

	if (playing) {
		OAL_Source_SetLoop(source, loop);
		OAL_Source_SetGain(source, volume);
		OAL_Source_SetPitch(source, pitch);
		if (!is3D) {
			OAL_Source_SetPosition(source, new float[3] { pan, 0, 0 });
		} else {
			OAL_Source_SetPosition(source, new float[3] { pos.X(), pos.Y(), 0 });
		}
	}
}

sig::AudioListener::AudioListener()
{
	m_masterVolume = 1;
}

void sig::AudioListener::Update(float dt)
{
	if (AudioSource::OAL_Initialized) {
		Vector2 pos = GetOwner()->GetPosition();
		OAL_Listener_SetAttributes(new float[3] { pos.X(), pos.Y(), 0 },
								   new float[3] { 0, 0, 0 },
								   new float[3] { 0, 1, 0 },
								   new float[3] { 0, 0, 1 });
		OAL_Listener_SetMasterVolume(m_masterVolume);
	}
}

sig::AudioListener *sig::AudioListener::GetInstance(sig::Node *owner)
{
	AudioListener *inst = new AudioListener();
	inst->m_owner = owner;
	inst->m_masterVolume = m_masterVolume;
	return inst;
}
