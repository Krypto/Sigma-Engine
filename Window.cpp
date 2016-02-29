#include "Window.h"
#include "SIG_OpenGL.h"
#include "Input.h"
#include "SIG_Utilities.h"

sig::Window::Window()
	: 	m_closing(true),
		m_width(1),
		m_height(1),
		m_title("Sigma")		
{ }

sig::Window::Window(string title, int width, int height)
{
	m_width = width;
	m_height = height;
	m_title = title;
	m_closing = true;
}

sig::Window::~Window()
{
}

void sig::Window::Open(bool fullscreen)
{
	m_closing = false;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SIG_LOG_ERROR("Could NOT initialize SDL. " << SDL_GetError());
		SIG_DIE(SIG_ERR_SDL_NOT_INITIALIZED);
	}
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 		8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 		8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 		8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,		8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 		16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 	1);
	
	m_sdl_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED, m_width, m_height, 
									SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (m_sdl_window == nullptr) {
		SIG_LOG_ERROR("Could NOT create window." << SDL_GetError());
		SIG_DIE(SIG_ERROR_NULL_WINDOW);
	}
	
	m_context = SDL_GL_CreateContext(m_sdl_window);
	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SIG_LOG_ERROR("Could NOT initialize GLEW.");
		SIG_DIE(SIG_ERR_UNKNOWN);
	}
}

void sig::Window::Close()
{
	if (!m_closing) {
		m_closing = true;
		
		SDL_GL_DeleteContext(m_context);
		SDL_DestroyWindow(m_sdl_window);
		ImGui_ImplSdl_Shutdown();
		SDL_Quit();
	}
}

void sig::Window::SwapBuffers()
{
	if (!m_closing) {
		SDL_GL_SwapWindow(m_sdl_window);
	}
}

void sig::Window::Update(float dt)
{
	if (Input::CLOSE_REQUESTED) {
		m_closing = true;
	}
}

void sig::Window::Clear(Color backColor, int flags)
{
	glViewport(0, 0, m_width, m_height);
	glClearColor(backColor.r, backColor.g, backColor.b, backColor.a);
	glClear(flags);
}

void sig::Window::Initialize()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, m_width, m_height);
	
	Input::win = m_sdl_window;
	
//	SDL_EnableUNICODE(SDL_ENABLE);
}

void sig::Window::Resize(int newW, int newH)
{
	m_width = newW;
	m_height = newH;
	
	SDL_SetWindowSize(m_sdl_window, m_width, m_height);
	
	glViewport(0, 0, m_width, m_height);
}
