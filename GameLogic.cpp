#include "GameLogic.h"

void sig::GameLogic::Initialize()
{
	if (OnInitialize) {
		OnInitialize();
	}
}

void sig::GameLogic::Update(float dt)
{
	if (OnUpdate) {
		OnUpdate(dt);
	}
}

void sig::GameLogic::MessageReceived(const sig::Message &msg)
{
	if (OnMessageReceived) {
		OnMessageReceived(msg);
	}
}

void sig::GameLogic::CollisionEnter(const sig::Collision &col)
{
	if (OnCollisionEnter) {
		OnCollisionEnter(col);
	}
}

void sig::GameLogic::CollisionExit(const sig::Collision &col)
{
	if (OnCollisionExit) {
		OnCollisionExit(col);
	}
}
