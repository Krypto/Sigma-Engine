#ifndef SIGMA_GAMELOOP
#define SIGMA_GAMELOOP

namespace sig
{

	class Window;
	class BaseGame;
	class GameLoop
	{
	public:
		GameLoop();
		GameLoop(Window* window);
		
		/**
		 * @brief Start the engine loop
		 * @param frameCap Target frame rate
		 * @return Exit code
		 */
		int Start(float frameCap = 60.0f);
		
		/**
		 * @brief Updates all the logic
		 * @param fps Frames per second
		 * @param time_scale Time scale
		 */
		void LogicStep(float fps, float time_scale=1.0f);
		
		BaseGame* GetGame() { return m_game; }
		GameLoop* SetGame(BaseGame* game) { this->m_game = game; return this; }
		
		int GetFPS() { return m_frames; }
	private:
		int SIG_MainLoop(float fps);
		
		Window *m_window;
		BaseGame *m_game;

		float m_startTime, m_frameTime;
		int m_frames;
	};

}

#endif // SIGMA_GAMELOOP
