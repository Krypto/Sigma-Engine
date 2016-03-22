#include "Scene.h"
#include "BaseGame.h"
#include "SIG_OpenGL.h"
#include "SIG_Utilities.h"
#include "Window.h"

#include <algorithm>

sig::Scene::Scene(BaseGame *game)
{
	m_game = game;
	m_pixelsPerMeter = 50.0f;
	m_root = nullptr;
	m_camera = nullptr;
	m_lastAdded = nullptr;
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
		SIG_FOREACH(it, bvs)
		{
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
		SIG_FOREACH(it, bvs)
		{
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

	GetGame()->GetSpriteBatch()->Begin();
	GetRoot()->Render(GetGame()->GetSpriteBatch());
	GetGame()->GetSpriteBatch()->End();

	GetGame()->GetSpriteBatch()->Render();

	if (m_gui) {
		m_gui->Render();
	}
}

void sig::Scene::Update(float dt)
{
	m_physicsWorld->Step(dt, 6, 2);
	
	GetRoot()->Update(dt);

	// Process Node Tree requests
	while (m_nodeTreeRequests.size() > 0) {
		NodeTreeRequest *req = m_nodeTreeRequests.front();
		req->Process(this);

		SIG_FREE(req);
		m_nodeTreeRequests.pop();
	}

	if (m_gui) {
		m_gui->Update(dt);
	}
}

void sig::Scene::Finalize()
{
	GetRoot()->Finalize();
}

void sig::Scene::CreateNodeTreeRequest(sig::Node *src, sig::Node *dest, sig::NTRAction action, float time)
{
	NodeTreeRequest *req = new NodeTreeRequest(src, dest,
											   action, time);
	m_nodeTreeRequests.push(req);
}

vector<sig::Node*> sig::Scene::GetAllNodes(Node* r)
{
	vector<Node*> nodes;

	if (r->GetChildren().size() > 0) {
		SIG_FOREACH(it, r->GetChildren())
		{
			nodes.push_back(*it);

			vector<Node*> subn = GetAllNodes(*it);
			nodes.insert(nodes.end(), subn.begin(), subn.end());
		}
	}
	
	return nodes;
}

void sig::Scene::AddNode(Node* c, float lifeTime)
{
	CreateNodeTreeRequest(c, GetRoot(),
						  NTRAction::NTR_ADD_VOLATILE,
						  lifeTime);
}

void sig::Scene::RemoveNode(Node* c)
{
	CreateNodeTreeRequest(c, GetRoot(),
						  NTRAction::NTR_DELETE);
}

void sig::Scene::Instantiate(Node* node, float time)
{
	CreateNodeTreeRequest(node, GetRoot(),
						  NTRAction::NTR_INSTANTIATE, time);
}

sig::Node* sig::Scene::GetNode(const string &name)
{
	SIG_FOREACH(it, GetAllNodes())
	{
		Node *n = *it;
		if (n->GetName() == name) {
			return n;
		}
	}
	return nullptr;
}

void sig::Scene::ReparentNodes(sig::Node *src, sig::Node *dest)
{
	CreateNodeTreeRequest(src, dest,
						  NTRAction::NTR_REPARENT);
}

void sig::Scene::AddInactiveNode(Node* c)
{
	m_inactiveNodes.push_back(c);
}

void sig::Scene::RemoveInactiveNode(const string &name)
{
	Node *n = nullptr;
	SIG_FOREACH(it, m_inactiveNodes)
	{
		Node *child = *it;
		if (child->GetName() == name) {
			n = *it;
			break;
		}
	}

	if (n != nullptr) {
		auto pos = std::find(m_inactiveNodes.begin(), m_inactiveNodes.end(), n);
		if (pos != m_inactiveNodes.end()) {
			m_inactiveNodes.erase(pos);
		}
	}
}

sig::Node* sig::Scene::GetInactiveNode(const string &name)
{
	SIG_FOREACH(it, m_inactiveNodes)
	{
		Node *child = *it;
		if (child->GetName() == name) {
			return *it;
		}
	}
	return nullptr;
}

void sig::Scene::SendMessage(const string& to, const string& body, float delay)
{
	GetRoot()->SendMessage(to, body, delay);
}

/* NodeTreeRequest methods */
void sig::NTR::Process(Scene* scene)
{
	if (src == nullptr || dest == nullptr) { return; }
	if (scene == nullptr) { return; }

	switch (action) {
		case NTRAction::NTR_ADD: {
			Node *_src = src;
			auto pos = std::find(scene->m_inactiveNodes.begin(),
								 scene->m_inactiveNodes.end(),
								 src);
			if (pos != scene->m_inactiveNodes.end()) {
				_src = *pos;
			}
			dest->AddChild(_src);
			scene->m_lastAdded = _src;
		} break;
		case NTRAction::NTR_ADD_VOLATILE: {
			Node *_src = src;
			auto pos = std::find(scene->m_inactiveNodes.begin(),
								 scene->m_inactiveNodes.end(),
								 src);
			if (pos != scene->m_inactiveNodes.end()) {
				_src = *pos;
			}
			dest->AddChild(_src, time);
			scene->m_lastAdded = _src;
		} break;
		case NTRAction::NTR_DELETE: {
			Node* rem = dest->RemoveChild(src);
			if (rem != nullptr) {
				scene->m_inactiveNodes.push_back(rem);
			}
		} break;
		case NTRAction::NTR_INSTANTIATE: {
			Node* instance = dest->GetInstance();
			scene->CreateNodeTreeRequest(instance, dest,
										 NTRAction::NTR_ADD_VOLATILE,
										 time);
		} break;
		case NTRAction::NTR_REPARENT: {
			src->SetParent(dest);
		} break;
	}
}
