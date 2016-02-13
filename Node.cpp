#include "Node.h"
#include "Input.h"
#include "SIG_OpenGL.h"
#include "SIG_Utilities.h"
#include "Scene.h"
#include "MessageNetwork.h"
#include "SpriteBatch.h"
#include "BaseGame.h"

#include <algorithm>
#include <math.h>

namespace sig
{

	Node::Node()
	{
		m_origin = Vector2(0, 0);
		m_scale = Vector2(1, 1);
		m_size = Vector2(1);
		m_parent = 0;
		m_lifeTime = -1;
		m_dead = false;
		m_rotation = 0;
		m_scene = nullptr;

		m_physicsType = PHYSICS_TYPE_STATIC;
		m_physicsBody = nullptr;
	}

	Node::~Node()
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it) {
			Component *comp = *it;
			// This prevents double-freeing the pointer
			if (comp->GetUsers() > 1) {
				comp->RemoveUser();
			} else {
				SIG_FREE((*it));
			}
		}
		for (auto it = m_children.begin(); it != m_children.end(); ++it) {
			SIG_FREE((*it));
		}
	}

	void Node::Initialize()
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it) {
			(*it)->Initialize();
			(*it)->m_initialized = true;
		}
		for (auto it = m_children.begin(); it != m_children.end(); ++it) {
			if ((*it) == nullptr) {
				continue;
			}
			if (!(*it)->m_dead) {
				(*it)->Initialize();
			}
		}
	}

	void Node::Render(SpriteBatch *batch)
	{
		for (auto it = m_children.begin(); it != m_children.end(); ++it) {
			if ((*it) == nullptr) {
				continue;
			}
			if (!(*it)->m_dead) {
				(*it)->Render(batch);
			}
		}

		for (auto it = m_components.begin(); it != m_components.end(); ++it) {
			if (!(*it)->IsEnabled()) {
				continue;
			}
			(*it)->Render(batch);
		}
	}

	void Node::Update(float dt)
	{
		if (!m_dead) {
			if (std::round(m_lifeTime) > -1) {
				m_lifeTime -= dt;
				if (m_lifeTime <= 0) {
					m_dead = true;
				}
			}
		} else {
			if (m_parent != nullptr) {
				m_parent->m_childrenDeleteQueue.push_back(this);
			}
		}

		if (m_physicsBody != nullptr) {
			if (m_physicsBody->GetType() != b2BodyType::b2_staticBody) {
				if (m_physicsBody->IsAwake()) {
					m_position = Vector2(m_physicsBody->GetPosition().x, m_physicsBody->GetPosition().y);
					m_rotation = -m_physicsBody->GetAngle();
				}
			}
		}

		for (auto it = m_components.begin(); it != m_components.end(); ++it) {
			if (!(*it)->IsEnabled()) {
				continue;
			}
			if (!(*it)->m_updated) {
				(*it)->Update(dt);
				(*it)->m_updated = true;
			}
		}

		for (auto it = m_children.begin(); it != m_children.end(); ++it) {
			if ((*it) == nullptr) {
				continue;
			}
			if (!(*it)->m_dead) {
				(*it)->Update(dt);
			}
		}
		
		Vector2 camPos = Vector2::Zero;
		if (GetScene()->GetCamera() != nullptr) {
			camPos = GetScene()->GetCamera()->GetOrigin() - GetScene()->GetCamera()->GetPosition();
		}
		Vector2 pos = Input::GetMousePosition() - camPos;
		Vector2 dx 	= pos - GetPosition();
		float scale = m_scale.X() > m_scale.Y() ? m_scale.Y() * m_size.Y() : m_scale.X() * m_size.X();
		
		if (dx.Length() <= scale) {
			GetScene()->GetGame()->OnNodeHover(this, pos.X(), pos.Y());
		}

		if (m_childrenQueue.size() > 0) {
			for (auto it = m_childrenQueue.begin(); it != m_childrenQueue.end(); ++it) {
				(*it)->Initialize();
				m_children.push_back(*it);
			}
			m_childrenQueue.clear();
		}

		if (m_childrenDeleteQueue.size() > 0) {
			for (auto it = m_childrenDeleteQueue.begin(); it != m_childrenDeleteQueue.end(); ++it) {
				auto pos = std::find(m_children.begin(), m_children.end(), (*it));
				if (pos != m_children.end()) {
					(*it)->RemoveParent();
				}
			}
			m_childrenDeleteQueue.clear();
		}
	}

	void Node::Finalize()
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it) {
			if (!(*it)->IsEnabled()) {
				continue;
			}
			if ((*it)->m_updated) {
				(*it)->m_updated = false;
			}
		}
		for (auto it = m_children.begin(); it != m_children.end(); ++it) {
			(*it)->Finalize();
		}
	}

	Matrix4 Node::GetTransformMatrix()
	{
		Matrix4 pos = Matrix4::MakeTranslation(Vector3(m_position.X(), m_position.Y(), 0));
		Matrix4 rot = Matrix4::MakeRotation(m_rotation, Vector3(0, 0, 1));
		Matrix4 ori = Matrix4::MakeTranslation(Vector3(-m_origin.X() * m_size.X(), -m_origin.Y() * m_size.Y(), 0));
		Matrix4 siz = Matrix4::MakeScale(Vector3(m_scale.X(), m_scale.Y(), 1));

		return GetParentTransformMatrix() * pos * siz * rot * ori;
	}

	Matrix4 Node::GetParentTransformMatrix()
	{
		Matrix4 ret = Matrix4::Identity;
		if (m_parent != nullptr) {
			ret = m_parent->GetTransformMatrix();
		}
		return ret;
	}

	void Node::AddComponent(Component* component)
	{
		if (component == nullptr) {
			return;
		}
		component->SetEnabled(true);
		component->SetOwner(this);

		m_components.push_back(component);
	}

	Node* Node::AddChild(Node* c, float lifeTime)
	{
		c->m_scene = m_scene;
		c->m_dead = false;
		c->m_lifeTime = lifeTime;
		c->m_parent = this;

		if (c->m_physicsBody == nullptr) {
			c->InitBody();
		}

		m_childrenQueue.push_back(c);

		return this;
	}

	Node* Node::SetParent(Node* parent)
	{
		if (parent != nullptr) {
			parent->AddChild(this);
		} else {
			this->m_parent = parent;
		}
		return this;
	}

	void Node::RemoveParent()
	{
		if (m_parent != nullptr) {
			m_parent->RemoveChild(this);
			m_parent = nullptr;
		}
	}

	void Node::RemoveChild(Node* c)
	{
		auto pos = std::find(m_children.begin(), m_children.end(), c);
		if (pos != m_children.end()) {
			auto comps = c->GetComponents();
			for (auto it = comps.begin(); it != comps.end(); ++it) {
				(*it)->RemoveUser();
			}
			delete m_children[std::distance(m_children.begin(), pos)];
			m_children[std::distance(m_children.begin(), pos)] = nullptr;
			m_children.erase(pos);
		}
	}

	Node* Node::GetInstance()
	{
		Node* newNode 			= new Node();
		newNode->m_position 	= Vector2(m_position);
		newNode->m_scale 		= Vector2(m_scale);
		newNode->m_origin 		= Vector2(m_origin);
		newNode->m_rotation 	= m_rotation;
		newNode->m_tag 			= m_tag;
		newNode->m_name			= m_name;

		for (auto it = m_children.begin(); it != m_children.end(); ++it) {
			newNode->AddChild((*it)->GetInstance());
		}

		for (auto it = m_components.begin(); it != m_components.end(); ++it) {
			newNode->AddComponent((*it)->GetInstance(newNode));
		}

		return newNode;
	}

	Node* Node::GetChild(const string &name)
	{
		for (auto it = m_children.begin(); it != m_children.end(); ++it) {
			if ((*it)->GetName() == name) {
				return *it;
			}
		}

		return nullptr;
	}

	Node* Node::Instantiate(Node* node, float time)
	{
		Node* n = node->GetInstance();
		AddChild(n, time);

		return n;
	}

	Node* Node::AddChildInactive(Node* c)
	{
		c->m_scene = m_scene;
		c->m_dead = false;
		c->m_lifeTime = -1;
		c->m_parent = this;

		m_inactiveChildren.push_back(c);

		return this;
	}

	Node* Node::RemoveChildInactive(const string &name)
	{
		Node *n;
		for (auto it = m_inactiveChildren.begin(); it != m_inactiveChildren.end(); ++it) {
			if ((*it)->GetName() == name) {
				n = *it;
				break;
			}
		}

		if (n != nullptr) {
			auto pos = std::find(m_inactiveChildren.begin(), m_inactiveChildren.end(), n);
			if (pos != m_inactiveChildren.end()) {
				m_inactiveChildren.erase(pos);
			}
		}

		return this;
	}

	Node* Node::GetChildInactive(const string &name)
	{
		for (auto it = m_inactiveChildren.begin(); it != m_inactiveChildren.end(); ++it) {
			if ((*it)->GetName() == name) {
				return *it;
			}
		}
		return nullptr;
	}

	void Node::SendMessage(const string &to, const string &body, float delay, void *udata)
	{
		Node *nto = nullptr;

		if (!to.empty()) {
			auto nodes = m_scene->GetAllNodes();
			for (auto it = nodes.begin(); it != nodes.end(); ++it) {
				Node *current = *it;
				if (current->GetName() == to) {
					nto = current;
					break;
				}
			}
		}

		MessageNetwork::Instance()->QueueMessage(this, nto, body, delay, udata);
	}

	Node* Node::SetPhysicsType(const PhysicsType& physicsType)
	{
		this->m_physicsType = physicsType;

		switch (m_physicsType) {
			case PHYSICS_TYPE_DYNAMIC:
				m_physicsBody->SetType(b2_dynamicBody);
				break;
			default:
			case PHYSICS_TYPE_STATIC:
				m_physicsBody->SetType(b2_staticBody);
				break;
		}

		return this;
	}

	void Node::InitBody()
	{
		// Initialize Physics
		b2World *world = m_scene->GetPhysicsWorld();

		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(
		    m_position.X() / m_scene->GetPixelsPerMeter(),
		    m_position.Y() / m_scene->GetPixelsPerMeter()
		);
		bodyDef.angle = m_rotation;
		bodyDef.userData = this;
		bodyDef.fixedRotation = false;

		m_physicsBody = world->CreateBody(&bodyDef);
	}
}

Node* sig::Node::SetPosition(const Vector2& position)
{
	this->m_position = position;

	if (m_physicsBody != nullptr) {
		m_physicsBody->SetTransform(b2Vec2(m_position.X(), m_position.Y()), m_rotation);
	}
	return this;
}

Node* sig::Node::SetRotation(float rotation)
{
	this->m_rotation = rotation;

	if (m_physicsBody != nullptr) {
		m_physicsBody->SetTransform(b2Vec2(m_position.X(), m_position.Y()), m_rotation);
	}
	return this;
}

Node* sig::Node::SetOrigin(const Vector2& origin)
{
	this->m_origin = origin;

	return this;
}
