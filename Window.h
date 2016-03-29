#ifndef SIGMA_WINDOW
#define SIGMA_WINDOW

#include "IUR.h"
#include "Color.h"
#include "Matrix4.h"

#include <SDL2/SDL.h>

#include <string>
using namespace std;

namespace sig
{
	using namespace math;
	class Window : public IUR
	{
		friend class Input;
	public:
		/**
		 * @brief Empty constructor
		 */
		Window();

		/**
		 * @brief Create a new Window
		 * @param title Window title
		 * @param width Window width
		 * @param height Window height
		 */
		Window(string title, int width, int height);
		
		~Window();
		
		/**
		 * @brief Open this window
		 * @param fullscreen Full screen switch
		 */
		void Open(bool fullscreen = false);

		/**
		 * @brief Close this window
		 */
		void Close();

		/**
		 * @brief Refresh screen
		 */
		void SwapBuffers();

		/**
		 * @brief Clear the window with a color
		 * @param backColor The target color
		 * @param flags glClear flags
		 */
		void Clear(Color backColor, int flags);

		/**
		 * @brief Resize the window
		 * @param newW New Width
		 * @param newH New Height
		 */
		void Resize(int newW, int newH);

		/**
		 * @brief Checks if the window is closing. Use this for the game loop
		 */
		bool IsClosing() const { return m_closing; }
		
		int GetHeight() const { return m_height; }
		int GetWidth() const { return m_width; }

		void Initialize();
		void Update(float dt);
		
		SDL_Window* GetSDLWindow() {return m_sdl_window;}

		Matrix4 GetProjectionMatrix();
	protected:
		SDL_Window *m_sdl_window;
		SDL_GLContext m_context;
		
		bool m_closing;
		int m_width, m_height;
		
		string m_title;
	};
}

#endif // SIGMA_WINDOW
