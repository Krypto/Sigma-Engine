#ifndef GAME_EXAMPLE
#define GAME_EXAMPLE

#include "Sigma.h"
using namespace sig;
using namespace math;

class GameExample : public BaseGame
{
public:
	GameExample() : BaseGame() {
		NewResource("car", "car_animated.png", ResourceType::RESOURCE_TEXTURE);
		NewResource("particle", "fire.png", ResourceType::RESOURCE_TEXTURE);
		NewResource("track", "map.png", ResourceType::RESOURCE_TEXTURE);
		NewResource("car_logic", "CarISO.lua", ResourceType::RESOURCE_TEXT);
		NewResource("map", "tg_track.json", ResourceType::RESOURCE_TEXT);
	}
	
	void Initialize() {
		GetCurrentScene()->GetCamera()->SetZoom(0.5f);

		car = GetCurrentScene()->CreateNode("Car");
		car->SetOrigin(Vector2(0.5f));

		AnimatedSprite *spr = new AnimatedSprite(GetResourceData(Texture2D*, "car"), 4, 10);
		spr->SetDrawOrder(5);
		spr->AddAnimation("run", 0.07, true, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
		spr->AddAnimation("jump", 0.08, false, { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 });
		spr->AddAnimation("turn_left", 0.01, false, { 20 });
		spr->AddAnimation("turn_right", 0.01, false, { 22 });
		spr->AddAnimation("damage", 0.07, true, { 26, 27, 28, 29 });
		spr->AddAnimation("flip", 0.08, true, { 30, 31, 32, 33, 34, 35, 36, 37 });

		Script *scr = GetLua()->RunStringModule("CarISO", GetResourceText("car_logic"));

		car->AddComponent(spr);
		car->AddComponent(scr);

		track = GetCurrentScene()->CreateNode("track");

		TileMap *tml = new TileMap(GetResourceData(Texture2D*, "track"));
		tml->SetProjection(TileMap::MAP_ISOMETRIC);
		tml->Configure(1, 3);
		tml->LoadFromJSONString(GetResourceText("map"));

		track->AddComponent(tml);

		pt = GetCurrentScene()->CreateNode("psys");
		pt->SetPosition(Vector2(-200, 0));

		ParticleEmitter *emt = new ParticleEmitter();
		emt->SetTexture(GetResourceData(Texture2D*, "particle"));
		emt->SetParticleSpeed(60);
		emt->SetParticleSpread(1.6f);
		emt->SetStartColor(Color::DARK_GRAY);
		emt->SetEndColor(Color::BLACK);
		emt->SetParticleBlend(BlendMode::ADD);
		emt->SetParticleAcceleration(Vector2(0, -2));
		emt->SetParticleAngularVelocity(0.6f);
		emt->SetParticleStartSize(0.5f);
		emt->SetEmitRate(0.01f);

		pt->AddComponent(emt);

		GetCurrentScene()->AddChild(pt);
		GetCurrentScene()->AddChild(car);
		GetCurrentScene()->AddChild(track);
	}

	Node *car, *track, *pt;
};

#endif // GAME_EXAMPLE
