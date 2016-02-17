#ifndef SIGMA_GAMELOGIC
#define SIGMA_GAMELOGIC

#include "Component.h"

#include <functional>

namespace sig {

	class GameLogic : public Component
	{
	public:
		GameLogic() : Component() {}

		void Initialize();
		void Update(float dt);
		void MessageReceived(const Message &msg);
		void CollisionEnter(const Collision &col);
		void CollisionExit(const Collision &col);

		std::function<void()> OnInitialize;
		std::function<void(float)> OnUpdate;
		std::function<void(const Message&)> OnMessageReceived;
		std::function<void(const Collision&)> OnCollisionEnter;
		std::function<void(const Collision&)> OnCollisionExit;
	};

}

#endif // SIGMA_GAMELOGIC
