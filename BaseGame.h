#ifndef SIGMA_BASE_GAME
#define SIGMA_BASE_GAME

#include "IUR.h"
#include "Scene.h"
#include "Vector.h"
#include "SpriteBatch.h"

#include <vector>
using namespace std;

#include <Box2D/Box2D.h>

namespace sig
{
	class GameLoop;
	class Lua;
	class Window;
	class BaseGame : public IUR
	{
		friend class GameLoop;
	public:
		BaseGame();
		~BaseGame();

		virtual void Initialize();
		virtual void Update(float dt);
		virtual void Render();
		virtual void OnNodeHover(Node *node, float mx, float my) {}

		Window* GetWindow() { return m_window; }

		SpriteBatch* GetSpriteBatch() { return m_spriteBatch; }
		
		Scene* GetCurrentScene() {
			if (m_currentScene == nullptr) {
				m_currentScene = new Scene(this);
			}
			return m_currentScene;
		}
		void SetCurrentScene(Scene* currentScene) {
			this->m_currentScene = currentScene;
			if (this->m_currentScene != nullptr) {
				this->m_currentScene->m_game = this;
				this->m_currentScene->Initialize();
			}
		}

		float& GetTimeScale() { return m_timeScale; }
		void SetTimeScale(float ts) {
			m_timeScale = SIG_CLAMPF_R(ts, 0.001f, 1.0f);
		}

		Node *PickNode();

		void Restart();
		void Pause();
		void Resume();
		
		Lua* GetLua() { return m_lua; }
	private:
		bool m_paused;

		void SIG_CleanUp();
		void SIG_Render();
		void SIG_Update(float dt);
		void SIG_Init(Window *w);
		void SIG_Finalize();

		SpriteBatch *m_spriteBatch;
		Window *m_window;
		Lua *m_lua;

		Scene *m_currentScene;

		float m_timeScale;
	};

}

#endif // SIGMA_BASE_GAME
