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
		NewResource("fire", "fire.png", ResourceType::RESOURCE_TEXTURE);
		NewResource("tree", "tree.png", ResourceType::RESOURCE_TEXTURE);
	}
	
	void Initialize() {
		GUI::DEFAULT_FONT = GetResourceData(Texture2D*, "font");

		p = GetCurrentScene()->CreateNode("p");
		e = new ParticleEmitter();
		e->SetTexture(GetResourceData(Texture2D*, "particle"));
		e->SetParticleBlend(BlendMode::ADD);
		p->AddComponent(e);

		g = new GUI();
		GetCurrentScene()->SetGUI(g);

		g->BeginBox(Rect(10, 10, 120, 100));

		g->AddLabel("Particle Emitter");
		g->AddSeparator();

		ItemList items;
		for (auto it = GetResources().begin();
			it != GetResources().end(); ++it)
		{
			Resource* res = it->second;
			if (res->GetType() == ResourceType::RESOURCE_TEXTURE) {
				items.push_back(new Texture2DItem(static_cast<Texture2D*>(res->GetData())));
			}
		}

		tex_list = g->AddParam("Textures", &selected_texture);
		tex_list->SetItems(items);

		g->AddButton("Set Texture")->SetCallback([this]() {
			ListViewItem *sel = tex_list->GetItem(selected_texture);
			if (sel != nullptr) {
				Texture2DItem *tex_item = dynamic_cast<Texture2DItem*>(sel);
				e->SetTexture(tex_item->GetData());
			}
		});

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
