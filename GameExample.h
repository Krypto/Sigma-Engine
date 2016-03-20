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
		NewResource("particle", "particle.png", ResourceType::RESOURCE_TEXTURE);
	}
	
	void Initialize() {
		GetCurrentScene()->SetBackground(Color(HEX(0x8A6132), 1));
		GUI::DEFAULT_FONT = GetResourceData(Texture2D*, "font");

		p = GetCurrentScene()->CreateNode("p");
		e = new ParticleEmitter();
		e->SetTexture(GetResourceData(Texture2D*, "particle"));
		e->SetParticleBlend(BlendMode::ADD);
		p->AddComponent(e);

		g = new GUI();
		GetCurrentScene()->SetGUI(g);

		g->BeginBox(Rect(10, 10, 150, 100));
		g->GetCurrentBox()->SetDockDirection(DockDirection::DOCK_LEFT);

		g->AddLabel("Particle Emitter");
		g->AddSeparator();

		g->AddParam("Start Color", &e->GetStartColor());
		g->AddParam("End Color", &e->GetEndColor());
		g->AddParam("Acceleration", &e->GetParticleAcceleration());
		g->AddParam("Spread", &e->GetParticleSpread(), 0.0f, PI);
		g->AddParam("Life (s)", &e->GetParticleLife());
		g->AddParam("E. Rate (s)", &e->GetEmitRate());
		g->AddParam("Speed", &e->GetParticleSpeed());
		g->AddParam("Start Size", &e->GetParticleStartSize());
		g->AddParam("End Size", &e->GetParticleEndSize());
		g->AddParam("Enabled", &e->IsEnabled());

		g->EndBox();

		g->BeginBox(Rect(0, 0, 120, 200));
		g->GetCurrentBox()->SetDockDirection(DockDirection::DOCK_RIGHT);

		g->AddLabel("Scene");
		g->AddSeparator();

		g->AddParam("Background Color", &GetCurrentScene()->GetBackground());

		g->EndBox();

		GetCurrentScene()->AddChild(p);
	}

	GUI *g;
	Node *p;
	ParticleEmitter *e;
	int selected_texture;
	ListView *tex_list;
};

#endif // GAME_EXAMPLE
