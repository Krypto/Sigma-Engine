#include "GameLoop.h"
#include "Color.h"
#include "Window.h"
#include "Time.h"
#include "SIG_Utilities.h"
#include "SIG_OpenGL.h"
#include "BaseGame.h"
#include "Node.h"
#include "Input.h"
#include "sig_gpu_builtin.h"
#include "GUIGFX.h"

#include "imgui_sdl.h"

#include <cmath>

sig::GameLoop::GameLoop()
	:	m_window(nullptr)
{
	m_frames = 0;
	m_frameTime = 0;
}

sig::GameLoop::GameLoop(Window* window)
	:	m_window(window)
{
}

int sig::GameLoop::Start(float frameCap)
{
	if (m_window == nullptr) {
		return SIG_ERROR_NULL_WINDOW;
	}

	if (m_window->IsClosing()) {
		m_window->Open();
	}

	return SIG_MainLoop(frameCap);
}

void sig::GameLoop::LogicStep(double dt)
{
	double newTime = Time::Instance()->GetTime();
	double frameTime = newTime - m_startTime;
	m_startTime = newTime;
	
	m_frameTime += frameTime;
	if (m_frameTime > 1.0f) {
		m_frameTime = 0;
		SIG_LOG(m_frames);
		m_frames = 0;
	}
	
	while (frameTime > 0) {
		float deltaTime = std::min(frameTime, dt);

		Input::Update();
		if (m_game != nullptr) {
			m_game->SIG_Update(deltaTime);
		}
		m_window->Update(deltaTime);
		
		frameTime -= deltaTime;
	}
	
}

int sig::GameLoop::SIG_MainLoop(float fps)
{
	const double dt = 1.0 / fps;

	m_startTime = Time::Instance()->GetTime();

	m_window->Initialize();
	GPUShaders::Initialize();
	GFX::Initialize();
	
	if (m_game != nullptr) {
		m_game->SIG_Init(m_window);
	}
	
	ImGui_ImplSdl_Init(m_game->GetWindow()->GetSDLWindow());
		
	SIG_LOG("Sigma-Project Engine Started...");
	
	while (!m_window->IsClosing()) {
		LogicStep(dt);
		ImGui_ImplSdl_NewFrame(m_game->GetWindow()->GetSDLWindow());

		m_window->Clear(Color::BLACK, GL_COLOR_BUFFER_BIT);

		if (m_game != nullptr) {
			m_game->SIG_Render();
			m_game->OnGUI();
		}
		
		ImGui::Render();
		
		m_window->SwapBuffers();
		
		if (m_game != nullptr) {
			m_game->SIG_Finalize();
		}
		m_frames++;
	}
	GPUShaders::Free();

	SIG_LOG("Sigma-Project Engine Finished...");

	return SIG_FINISHED;
}
