#ifndef SIGMA_SOUND_SYSTEM
#define SIGMA_SOUND_SYSTEM

#include "SIG_Utilities.h"

#include "Component.h"

#include <OALWrapper/OAL_Funcs.h>
#include <OALWrapper/OAL_Sample.h>

#include <map>
#include <vector>
#include <string>
using namespace std;

namespace sig
{
	class AudioSample;
	typedef struct AudioObject {
		float volume, pitch, pan;
		bool paused, playing, is3D, loop;
		int source;

		void Play(AudioSample* sample);
		void Pause();
		void Stop();

		bool IsPlaying() const { return playing; }

		void Update(const Vector2& pos);
	} AudioObject;

	class AudioSample
	{
		friend class AudioSource;
	public:
		AudioSample(void *data, u32 size);
		~AudioSample();

		cOAL_Sample *GetSample() { return m_sample; }
		void *GetData() { return m_data; }
		u32 GetSize() { return m_size; }

		void Reload();
	protected:
		bool m_loaded;
		cOAL_Sample *m_sample;
		void *m_data;
		u32 m_size;
	};

	typedef map<string, AudioSample*> AudioSampleList;

	class AudioSource : public Component
	{
	public:
		AudioSource();
		~AudioSource();

		void Initialize();
		void Update(float dt);

		void AddSample(const string& name, AudioSample *sample);
		AudioSample *GetSample(const string& name);

		AudioObject *Play(AudioSample *sample);
		AudioObject *Play(const string& name);

		AudioSource *GetInstance(Node *owner) override;

		static bool OAL_Initialized;

		COMPONENT_NAME("AudioSource");
	private:
		AudioSampleList m_library;
		vector<AudioObject*> m_playing, m_remove;
	};

	class AudioListener : public Component
	{
	public:
		AudioListener();

		void Update(float dt);

		float GetMasterVolume() const { return m_masterVolume; }
		void SetMasterVolume(float v) { m_masterVolume = v; }

		AudioListener *GetInstance(Node *owner) override;

		COMPONENT_NAME("AudioListener");
	private:
		float m_masterVolume;
	};
}

#endif // SIGMA_SOUND_SYSTEM
