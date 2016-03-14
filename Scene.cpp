#include "Scene.h"
#include "BaseGame.h"
#include "SIG_OpenGL.h"
#include "Window.h"

sig::Scene::Scene(BaseGame *game)
{
	m_game = game;
	m_pixelsPerMeter = 50.0f;
	m_root = nullptr;
	m_camera = nullptr;
}

sig::Scene::~Scene()
{
	SIG_FREE(m_root);
	SIG_FREE(m_physicsWorld);
}

sig::Node* sig::Scene::CreateNode(const string &name)
{
	Node *n = new Node();
	n->m_scene = this;
	n->SetName(name);
	n->InitBody();

	return n;
}

void sig::Scene::BeginContact(b2Contact* contact)
{
	void* userData = contact->GetFixtureA()->GetBody()->GetUserData();
	b2Manifold *mf = contact->GetManifold();
	
	Collision col;
	col.hitNode = static_cast<Node*>(contact->GetFixtureB()->GetBody()->GetUserData());
	col.hitNormal = Vector2(mf->localNormal.x, mf->localNormal.y);
	col.hitPosition = Vector2(mf->localPoint.x, mf->localPoint.y);
	
	Node *to = static_cast<Node*>(userData);
	
	if (to) {
		vector<Component*> bvs = to->GetComponents();
		for (auto it = bvs.begin(); it != bvs.end(); ++it) {
			Component *b = (*it);
			b->CollisionEnter(col);
		}
	}
}

void sig::Scene::EndContact(b2Contact* contact)
{
	void* userData = contact->GetFixtureA()->GetBody()->GetUserData();
	b2Manifold *mf = contact->GetManifold();
	
	Collision col;
	col.hitNode = static_cast<Node*>(contact->GetFixtureB()->GetBody()->GetUserData());
	col.hitNormal = Vector2(mf->localNormal.x, mf->localNormal.y);
	col.hitPosition = Vector2(mf->localPoint.x, mf->localPoint.y);
	
	Node *to = static_cast<Node*>(userData);
	
	if (to) {
		vector<Component*> bvs = to->GetComponents();
		for (auto it = bvs.begin(); it != bvs.end(); ++it) {
			Component *b = (*it);
			b->CollisionExit(col);
		}
	}
}

vector<sig::Node*> sig::Scene::GetAllNodes()
{
	return GetAllNodes(GetRoot());
}

void sig::Scene::Initialize()
{
	m_physicsWorld = new b2World(b2Vec2(0, 140));
	m_physicsWorld->SetAllowSleeping(true);
	m_physicsWorld->SetContinuousPhysics(true);
	
	m_physicsWorld->SetContactListener(this);
	
	if (m_camera == nullptr) {
		m_camera = new Camera2D();
		m_camera->SetName("defaultCamera");
		
		GetRoot()->AddChild(m_camera);
	}
	
	GetRoot()->Initialize();

	if (m_gui) {
		m_gui->Initialize();
	}
}

void sig::Scene::Render()
{
	if (m_camera != nullptr) {
		Window *win = m_game->GetWindow();
		if (win != nullptr) {
			m_camera->ApplyTransformation(	win->GetWidth(),
											win->GetHeight());
		}
	} else {
		Window *win = m_game->GetWindow();
		if (win != nullptr) {
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, win->GetWidth(), win->GetHeight(), 0, -1, 1);
			glMatrixMode(GL_MODELVIEW);
		}
	}
	
	GetRoot()->Render(GetGame()->GetSpriteBatch());
	if (m_gui) {
		m_gui->Render();
	}
}

void sig::Scene::Update(float dt)
{
	m_physicsWorld->Step(dt, 6, 2);
	
	GetRoot()->Update(dt);
	if (m_gui) {
		m_gui->Update(dt);
	}
}

void sig::Scene::Finalize()
{
	GetRoot()->Finalize();
}

vector<sig::Node*> sig::Scene::GetAllNodes(Node* r)
{
	vector<Node*> nodes;

	if (r->GetChildren().size() > 0) {
		for (auto it = r->GetChildren().begin(); it != r->GetChildren().end(); ++it) {
			nodes.push_back(*it);

			vector<Node*> subn = GetAllNodes(*it);
			nodes.insert(nodes.end(), subn.begin(), subn.end());
		}
	}
	
	return nodes;
}

sig::Node* sig::Scene::AddChild(Node* c, float lifeTime)
{
	return GetRoot()->AddChild(c, lifeTime);
}

void sig::Scene::RemoveChild(Node* c)
{
	GetRoot()->RemoveChild(c);
}

sig::Node* sig::Scene::Instantiate(Node* node, float time)
{
	return GetRoot()->Instantiate(node, time);
}

sig::Node* sig::Scene::GetChild(const string &name)
{
	return GetRoot()->GetChild(name);
}

sig::Node* sig::Scene::AddChildInactive(Node* c)
{
	return GetRoot()->AddChildInactive(c);
}

sig::Node* sig::Scene::RemoveChildInactive(const string &name)
{
	return GetRoot()->RemoveChildInactive(name);
}

sig::Node* sig::Scene::GetChildInactive(const string &name)
{
	return GetRoot()->GetChildInactive(name);
}

void sig::Scene::SendMessage(const string& to, const string& body, float delay)
{
	GetRoot()->SendMessage(to, body, delay);
}
