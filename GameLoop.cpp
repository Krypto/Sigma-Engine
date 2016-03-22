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

void sig::GameLoop::LogicStep(float fps, float time_scale)
{
	float newTime = Time::Instance()->GetTime();
	float frameTime = newTime - m_startTime;
	m_startTime = newTime;
	
	m_frameTime += frameTime;
	if (m_frameTime > 1.0f) {
		m_frameTime = 0;
		SIG_LOG(m_frames);
		m_frames = 0;
	}
	
	float cdt = time_scale / fps;
	while (frameTime > 0) {
		float deltaTime = std::min(frameTime, cdt);

		Input::Update();
		if (m_game != nullptr) {
			m_game->SIG_Update(deltaTime);
		}
		
		frameTime -= deltaTime;
	}
	
}

int sig::GameLoop::SIG_MainLoop(float fps)
{
	m_startTime = Time::Instance()->GetTime();

	m_window->Initialize();
	GPUShaders::Initialize();
	GFX::Initialize();

	if (m_game != nullptr) {
		m_game->SIG_Init(m_window);
	}

	SIG_LOG("Sigma Engine Started...");
	
	while (!m_window->IsClosing()) {
		float ts = m_game != nullptr ? m_game->GetTimeScale() : 1.0f;
		ts = SIG_CLAMPF_R(ts, 0.001f, 1.0f);
		LogicStep(fps, ts);

		Color bg = Color::BLACK;
		if (m_game && m_game->GetCurrentScene()) {
			bg = m_game->GetCurrentScene()->GetBackground();
		}
		m_window->Clear(bg, GL_COLOR_BUFFER_BIT);

		if (m_game != nullptr) {
			m_game->SIG_Render();
		}

		m_window->SwapBuffers();
		m_window->Update(0);

		if (m_game != nullptr) {
			m_game->SIG_Finalize();
		}
		m_frames++;
	}
	GPUShaders::Free();
	m_window->Close();

	SIG_LOG("Sigma Engine Finished...");

	return SIG_FINISHED;
}
