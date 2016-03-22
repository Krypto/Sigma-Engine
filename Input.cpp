#include "Input.h"
#include "Node.h"
#include "Camera2D.h"
#include "Matrix4.h"

namespace sig
{
	bool Input::m_inputKeys[MAX_KEYS];
	int Input::m_inputMod = 0;
	bool Input::m_downKeys[MAX_KEYS];
	bool Input::m_upKeys[MAX_KEYS];

	bool Input::m_inputMouse[MAX_MOUSE];
	bool Input::m_downMouse[MAX_MOUSE];
	bool Input::m_upMouse[MAX_MOUSE];

	Vector2 Input::m_mousePos;

	SDL_Event Input::evt;
	SDL_Window* Input::win = 0;
	
	bool Input::CLOSE_REQUESTED = false;
}

void sig::Input::Update()
{
	for (int i = 0; i < MAX_MOUSE; i++) {
		m_downMouse[i] = false;
		m_upMouse[i] = false;
	}

	for (int i = 0; i < MAX_KEYS; i++) {
		m_downKeys[i] = false;
		m_upKeys[i] = false;
	}

	while (SDL_PollEvent(&evt)) {
		if (evt.type == SDL_QUIT) {
			CLOSE_REQUESTED = true;
		}
		
		if (evt.type == SDL_MOUSEMOTION) {
			m_mousePos.SetX(evt.motion.x);
			m_mousePos.SetY(evt.motion.y);
		}

		if (evt.type == SDL_KEYDOWN) {
			int value = evt.key.keysym.scancode;

			m_inputMod = evt.key.keysym.mod;
			m_inputKeys[value] = true;
			m_downKeys[value] = true;
		}
		
		if (evt.type == SDL_KEYUP) {
			int value = evt.key.keysym.scancode;
			
			m_inputMod = 0;
			m_inputKeys[value] = false;
			m_upKeys[value] = true;
		}
		
		if (evt.type == SDL_MOUSEBUTTONDOWN) {
			int value = evt.button.button;

			m_inputMouse[value] = true;
			m_downMouse[value] = true;
		}
		
		if (evt.type == SDL_MOUSEBUTTONUP) {
			int value = evt.button.button;

			m_inputMouse[value] = false;
			m_upMouse[value] = true;
		}
	}
}

bool sig::Input::GetKey(int keycode)
{
	return m_inputKeys[keycode];
}

bool sig::Input::GetKeyDown(int keycode)
{
	return m_downKeys[keycode];
}

bool sig::Input::GetKeyUp(int keycode)
{
	return m_upKeys[keycode];
}

bool sig::Input::GetMouse(int btncode)
{
	return m_inputMouse[btncode];
}

bool sig::Input::GetMouseButtonDown(int btncode)
{
	return m_downMouse[btncode];
}

bool sig::Input::GetMouseButtonUp(int btncode)
{
	return m_upMouse[btncode];
}

sig::math::Vector2 sig::Input::GetMousePosition()
{
	return m_mousePos;
}

void sig::Input::SetMousePosition(float x, float y)
{
	m_mousePos = Vector2(x, y);
	SDL_WarpMouseInWindow(win, int(x), int(y));
}

void sig::Input::SetCursorVisibility(bool cur)
{
	SDL_ShowCursor(cur ? 1 : 0);
}

void sig::Input::SetCursorType(sig::CursorType type)
{
	SDL_SetCursor(SDL_CreateSystemCursor((SDL_SystemCursor)type));
}

bool sig::Input::GetModifier(int mod)
{
	return m_inputMod & mod;
}
