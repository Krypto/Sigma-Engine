#ifndef GAME_EXAMPLE
#define GAME_EXAMPLE

#include "Sigma.h"
using namespace sig;
using namespace math;

class Parallax : public Component
{
public:
	Parallax(Sprite *spr, float speed = 1.0f, int dir = 1) : Component() {
		this->speed = speed;
		this->direction = dir;
		sprite = spr;
		on = true;
	}

	void Update(float dt) {
		if (GetOwner() == nullptr) {
			return;
		}
		if (sprite == nullptr) {
			return;
		}
		if (!on) {
			return;
		}

		float spd = (speed * direction) * dt;

		Rect cr = sprite->GetUVRectangle();
		cr.x += spd;
		if (cr.x >= 1.0f * direction) {
			cr.x = 0.0f;
		}
		sprite->SetUVRectangle(cr);
	}

	void MessageReceived(Message* msg) {
		if (msg->GetBody() == "game_over") {
			on = false;
		}
	}

	Sprite *sprite;
	float speed;
	int direction;
	bool on;
};

class GameExample : public BaseGame
{
public:
	GameExample() : BaseGame() {
		NewResource("bg_sky", "data/fbg.png", ResourceType::RESOURCE_TEXTURE);
		NewResource("bg_floor", "data/fbg2.png", ResourceType::RESOURCE_TEXTURE);
		NewResource("bird", "data/bird.png", ResourceType::RESOURCE_TEXTURE);
		NewResource("pipe", "data/pipe.png", ResourceType::RESOURCE_TEXTURE);
		NewResource("bird_logic", "data/bird.lua", ResourceType::RESOURCE_TEXT);
		NewResource("pipe_spawner", "data/pipe_spawner.lua", ResourceType::RESOURCE_TEXT);
		NewResource("pipe_move", "data/pipe_move.lua", ResourceType::RESOURCE_TEXT);

		// Sounds
		NewResource("flap", "data/sfx_wing.wav", ResourceType::RESOURCE_AUDIO);
		NewResource("hit", "data/sfx_hit.wav", ResourceType::RESOURCE_AUDIO);
	}
	
	void Initialize() {
		src = new AudioSource();

		src->AddSample("flap", GetResourceData(AudioSample*, "flap"));
		src->AddSample("hit", GetResourceData(AudioSample*, "hit"));

		// Sky background
		bg1 = GetCurrentScene()->CreateNode("bg1");
		bg1->SetOrigin(Vector2(0.5f));

		Sprite *fbg = new Sprite(GetResourceData(Texture2D*, "bg_sky"));
		bg1->AddComponent(fbg);
		bg1->AddComponent(new Parallax(fbg, 0.08f, 1));

		// Floor
		bg2 = GetCurrentScene()->CreateNode("bg2");
		bg2->SetOrigin(Vector2(0.5f, 0.0f));
		bg2->SetPosition(Vector2(0, 150));
		bg2->SetTag("floor");

		Sprite *fbg2 = new Sprite(GetResourceData(Texture2D*, "bg_floor"));
		bg2->AddComponent(fbg2);
		bg2->AddComponent(new Parallax(fbg2, 0.22f, 1));
		bg2->AddComponent(new BoxShape(fbg2->GetTexture()->GetWidth()/2,
									   fbg2->GetTexture()->GetHeight()/2,
									   Vector2(0, fbg2->GetTexture()->GetHeight()/2)));
		bg2->AddComponent(new RigidBody2D(GetCurrentScene()->GetPhysicsWorld()));
		fbg2->SetDrawOrder(22);

		// Bird
		bird = GetCurrentScene()->CreateNode("bird");
		bird->SetOrigin(Vector2(0.5f, 0.5f));

		AnimatedSprite *birdspr = new AnimatedSprite(GetResourceData(Texture2D*, "bird"), 1, 3);
		birdspr->AddAnimation("flap", 0.04f, true, {0, 1, 2, 1});

		bird->AddComponent(src);
		bird->AddComponent(new CircleShape(13.0f));
		bird->AddComponent(new RigidBody2D(GetCurrentScene()->GetPhysicsWorld()));
		bird->AddComponent(birdspr);
		bird->AddComponent(GetLua()->RunStringModule(GetResourceText("bird_logic")));

		// Pipe 1
		pipe = GetCurrentScene()->CreateNode("pipe");
		pipe->SetTag("pipe");
		pipe->SetOrigin(Vector2(0.5f, 0));
		pipe->SetPosition(Vector2(-400));
		pipe->SetRotation(0);

		Sprite *pipespr = new Sprite(GetResourceData(Texture2D*, "pipe"));
		pipe->AddComponent(pipespr);
		pipe->AddComponent(new BoxShape(pipespr->GetTexture()->GetWidth()/2,
										pipespr->GetTexture()->GetHeight()/2,
										Vector2(0, pipespr->GetTexture()->GetHeight()/2)));
		pipe->AddComponent(new RigidBody2D(GetCurrentScene()->GetPhysicsWorld()));
		pipe->AddComponent(GetLua()->RunStringModule(GetResourceText("pipe_move")));

		// Pipe 2
		pipe2 = GetCurrentScene()->CreateNode("pipe2");
		pipe2->SetTag("pipe");
		pipe2->SetOrigin(Vector2(0.5f, 0));
		pipe2->SetPosition(Vector2(-400));
		pipe2->SetScale(Vector2(1, -1));
		pipe2->SetRotation(0);

		pipe2->AddComponent(pipespr);
		pipe2->AddComponent(new BoxShape(pipespr->GetTexture()->GetWidth()/2,
										pipespr->GetTexture()->GetHeight()/2,
										Vector2(0, -pipespr->GetTexture()->GetHeight()/2)));
		pipe2->AddComponent(new RigidBody2D(GetCurrentScene()->GetPhysicsWorld()));
		pipe2->AddComponent(GetLua()->RunStringModule(GetResourceText("pipe_move")));

		// Spawner
		pipe_spawner = GetCurrentScene()->CreateNode("spawn");
		pipe_spawner->AddComponent(GetLua()->RunStringModule(GetResourceText("pipe_spawner")));

		GetCurrentScene()->AddNode(bg1);
		GetCurrentScene()->AddNode(bg2);
		GetCurrentScene()->AddNode(bird);
		GetCurrentScene()->AddNode(pipe_spawner);

		GetCurrentScene()->AddInactiveNode(pipe);
		GetCurrentScene()->AddInactiveNode(pipe2);

		GetCurrentScene()->GetPhysicsWorld()->SetGravity(b2Vec2(0, 9));

		//GetCurrentScene()->ShowPhysicsVisualization(true);
	}

	AudioSource *src;
	Node *bg1, *bg2, *bird, *pipe, *pipe2, *pipe_spawner;
};

#endif // GAME_EXAMPLE
