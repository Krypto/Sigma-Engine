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
		 * @param dt Delta time 1.0/fps
		 */
		void LogicStep(double dt);
		
		GameLoop* SetGame(BaseGame* game) { this->m_game = game; return this; }
		BaseGame* GetGame() { return m_game; }
		
		int GetFPS() { return m_frames; }
	private:
		int SIG_MainLoop(float fps);
		
		Window *m_window;
		BaseGame *m_game;
		
		double m_startTime, m_frameTime;
		int m_frames;
	};

}

#endif // SIGMA_GAMELOOP
