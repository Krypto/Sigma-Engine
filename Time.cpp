#include "Time.h"
#include <SDL2/SDL.h>

sig::Time* sig::Time::ms_instance = nullptr;

sig::Time::Time()
{
}

sig::Time::~Time()
{}

sig::Time* sig::Time::Instance()
{
	if (ms_instance == nullptr) {
		ms_instance = new sig::Time();
	}
	return ms_instance;
}

void sig::Time::Release()
{
	if (ms_instance) {
		delete ms_instance;
	}
	ms_instance = nullptr;
}

double sig::Time::GetTime()
{
	return SDL_GetTicks() / 1000.0;
}