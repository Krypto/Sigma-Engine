#ifndef GAME_EXAMPLE
#define GAME_EXAMPLE

#include "Sigma.h"
using namespace sig;
using namespace math;

class GameExample : public BaseGame
{
public:
	GameExample() : BaseGame() {
		NewResource("font", "font2.png", ResourceType::RESOURCE_TEXTURE);
	}
	
	void Initialize() {
		str = "Test";
		g = new GUI();
		GUI::DEFAULT_FONT = GetResourceData(Texture2D*, "font");

		GetCurrentScene()->SetGUI(g);
		GetCurrentScene()->SetBackground(Color(HEX(0x4DD5FA), 1.0f));

		g->BeginBox(Rect(10, 10, 120, 100));
		g->AddLabel("Label");
		g->AddSeparator();
		g->AddParam("Integer", &num, 0, 10);
		g->AddParam("Float", &rot, 0.0f, PI);
		g->AddParam("String", &str);
		g->AddParam("Boolean", &bol);
		g->AddParam("Float Entry", &num2);
		g->AddParam("Vector", &pos);
		g->AddSeparator();
		g->AddButton("Click Me!")->SetCallback([]() {
			SIG_LOG("You clicked me!");
		});
		g->EndBox();
	}

	GUI *g;
	float rot, num2;
	int num;
	bool bol;
	Vector2 pos;
	string str;
};

#endif // GAME_EXAMPLE
