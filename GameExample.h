#ifndef GAME_EXAMPLE
#define GAME_EXAMPLE

#include "Sigma.h"
using namespace sig;
using namespace math;

class BlendModeListViewItem : public ListViewItem
{
public:
	BlendModeListViewItem(BlendMode mode)
		:	ListViewItem(),
			bmode(mode)
	{}

	virtual string ToString() {
		switch (bmode) {
			case BlendMode::ADD:
				return "Add";
			default:
			case BlendMode::NORMAL:
				return "Normal";
			case BlendMode::MULTIPLY:
				return "Multiply";
			case BlendMode::SCREEN:
				return "Screen";
		}
	}

	BlendMode GetBlendMode() const { return bmode; }
private:
	BlendMode bmode;
};

class GameExample : public BaseGame
{
public:
	GameExample() : BaseGame() {
		NewResource("font", "font2.png", ResourceType::RESOURCE_TEXTURE);
		NewResource("particle", "particle.png", ResourceType::RESOURCE_TEXTURE);
		NewResource("smoke", "smoke.png", ResourceType::RESOURCE_TEXTURE);
	}
	
	void Initialize() {
		GetCurrentScene()->SetBackground(Color(HEX(0x8A6132), 1));
		GUI::DEFAULT_FONT = GetResourceData(Texture2D*, "font");

		p = GetCurrentScene()->CreateNode("p");
		e = new ParticleEmitter();
		e->SetTexture(GetResourceData(Texture2D*, "smoke"));
		p->AddComponent(e);

		ItemList blendModes;
		blendModes.push_back(new BlendModeListViewItem(BlendMode::NORMAL));
		blendModes.push_back(new BlendModeListViewItem(BlendMode::ADD));
		blendModes.push_back(new BlendModeListViewItem(BlendMode::MULTIPLY));
		blendModes.push_back(new BlendModeListViewItem(BlendMode::SCREEN));

		g = new GUI();
		GetCurrentScene()->SetGUI(g);

		g->BeginBox(Rect(10, 10, 150, 100));
		g->GetCurrentBox()->SetDockDirection(DockDirection::DOCK_LEFT);

		g->AddLabel("Particle Emitter");
		g->AddSeparator();

		g->AddParam("Start Color", &e->GetStartColor());
		g->AddParam("End Color", &e->GetEndColor());

		ListView *bview = g->AddParam("Blend Mode", &selected_bmode);
		bview->SetItems(blendModes);

		g->AddButton("Update Blend Mode")->SetCallback([this, bview]() {
			BlendModeListViewItem *sel = static_cast<BlendModeListViewItem*>(bview->GetItem(selected_bmode));
			e->SetParticleBlend(sel->GetBlendMode());
		});

		g->AddParam("Acceleration", &e->GetParticleAcceleration());
		g->AddParam("Spread", &e->GetParticleSpread(), 0.0f, PI);
		g->AddParam("Life (s)", &e->GetParticleLife());
		g->AddParam("E. Rate (s)", &e->GetEmitRate());
		g->AddParam("Speed", &e->GetParticleSpeed());
		g->AddParam("Ang. Speed", &e->GetParticleAngularVelocity());
		g->AddParam("Start Size", &e->GetParticleStartSize());
		g->AddParam("End Size", &e->GetParticleEndSize());
		g->AddParam("Enabled", &e->IsEnabled());

		g->EndBox();

		g->BeginBox(Rect(0, 0, 150, 200));
		g->GetCurrentBox()->SetDockDirection(DockDirection::DOCK_RIGHT);

		g->AddLabel("Scene");
		g->AddSeparator();

		g->AddParam("Background Color", &GetCurrentScene()->GetBackground());

		g->AddLabel("Current Camera");
		g->AddSeparator();
		g->AddParam("Position", &GetCurrentScene()->GetCamera()->GetPosition());
		g->AddParam("Rotation", &GetCurrentScene()->GetCamera()->GetRotation(), 0.0f, PI);
		g->AddParam("Zoom", &GetCurrentScene()->GetCamera()->GetZoom(), 0.01f, 4.0f);

		g->AddLabel("Game");
		g->AddSeparator();
		g->AddParam("Time Scale", &GetTimeScale());

		g->EndBox();

		GetCurrentScene()->AddNode(p);
	}

	GUI *g;
	Node *p;
	ParticleEmitter *e;
	int selected_bmode;
};

#endif // GAME_EXAMPLE
