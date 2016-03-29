#include "BaseGame.h"
#include "SIG_Utilities.h"
#include "GameLoop.h"
#include "MessageNetwork.h"
#include "ResourceManager.h"
#include "Window.h"
#include "SigmaTypeReg.h"
#include "Input.h"

sig::BaseGame::BaseGame()
{
	m_paused = false;
	m_currentScene = nullptr;
	m_spriteBatch = nullptr;
	m_window = nullptr;
	m_lua = nullptr;
	m_timeScale = 1;
}

sig::BaseGame::~BaseGame()
{
	SIG_CleanUp();
	SIG_FREE(m_lua);
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
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	GetCurrentScene()->Render();
}

void sig::BaseGame::SIG_Update(float dt)
{
	if (!m_paused) {
		GetCurrentScene()->Update(dt);
		MessageNetwork::Instance()->Update(dt);
		Update(dt);
	}

	if (GetCurrentScene()->GetGUI() != nullptr) {
		GetCurrentScene()->GetGUI()->Update(dt);
	}
}

void sig::BaseGame::SIG_Init(Window *w)
{
	m_window = w;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w->GetWidth(), w->GetHeight(), 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);

	ResourceManager::Instance()->LoadAll();
	MessageNetwork::Instance()->SetGame(this);
	
	m_spriteBatch = new SpriteBatch();
	m_spriteBatch->Initialize();
	
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

Node *BaseGame::PickNode()
{
	Node *picked = nullptr;
	Vector2 mp = Input::GetMousePosition();
	u32 index;

	glReadPixels(mp.X(), m_window->GetHeight() - mp.Y(), 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

	vector<Node*> all = GetCurrentScene()->GetAllNodes();
	for (auto it = all.begin(); it != all.end(); ++it) {
		Node *n = *it;
		if (n->GetPickID() == index) {
			picked = n;
		}
	}
	glDisable(GL_STENCIL_TEST);
	return picked;
}
