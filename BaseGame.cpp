#include "BaseGame.h"
#include "SIG_Utilities.h"
#include "GameLoop.h"
#include "MessageNetwork.h"
#include "ResourceManager.h"
#include "Window.h"
#include "SigmaTypeReg.h"

sig::BaseGame::BaseGame()
{
	m_paused = false;
	m_soundSystem = nullptr;
	m_currentScene = nullptr;
	m_spriteBatch = nullptr;
	m_window = nullptr;
	m_timeScale = 1;
}

sig::BaseGame::~BaseGame()
{
	SIG_CleanUp();
}

void sig::BaseGame::Initialize()
{

}

void sig::BaseGame::Update(float dt)
{
}

void sig::BaseGame::Render()
{
	
}

void sig::BaseGame::SIG_CleanUp()
{
	SIG_FREE(m_soundSystem);
	SIG_FREE(m_currentScene);
	SIG_FREE(m_spriteBatch);
	ResourceManager::Release();
}

void sig::BaseGame::Restart()
{
	SIG_CleanUp();
	SIG_Init(m_window);
}

void sig::BaseGame::Pause()
{
	m_paused = true;
}

void sig::BaseGame::Resume()
{
	if (m_paused) {
		m_paused = false;
	}
}

void sig::BaseGame::SIG_Render()
{
	GetCurrentScene()->Render();
}

void sig::BaseGame::SIG_Update(float dt)
{
	if (!m_paused) {
		GetCurrentScene()->Update(dt);
		MessageNetwork::Instance()->Update(dt);
		Update(dt);
	}

	GetSoundSystem()->Update();
}

void sig::BaseGame::SIG_Init(Window *w)
{
	m_window = w;

	ResourceManager::Instance()->LoadAll();
	MessageNetwork::Instance()->SetGame(this);
	
	m_spriteBatch = new SpriteBatch();
	m_spriteBatch->Initialize();
	
	GetSoundSystem()->Initialize();
	GetCurrentScene()->Initialize();

	// Register Types
	m_lua = new Lua();
	reg::RegisterTypes(m_lua, this);

	Initialize();
}

void sig::BaseGame::SIG_Finalize()
{
	GetCurrentScene()->Finalize();
}

SoundSystem* sig::BaseGame::GetSoundSystem()
{
	if (m_soundSystem == nullptr) {
		m_soundSystem = new SoundSystem();
	}
	return m_soundSystem;
}
