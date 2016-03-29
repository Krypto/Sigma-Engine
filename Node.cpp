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

unsigned int sig::Node::pickID = 0;

sig::Node::Node()
{
	m_origin = Vector2(0, 0);
	m_scale = Vector2(1, 1);
	m_size = Vector2(10);
	m_parent = 0;
	m_lifeTime = -1;
	m_dead = false;
	m_rotation = 0;
	m_scene = nullptr;
	m_components.clear();
}

sig::Node::~Node()
{
	SIG_FOREACH(it, m_components)
	{
		Component *comp = *it;
		// This prevents double-freeing the pointer
		if (comp->GetUsers() > 1) {
			comp->RemoveUser();
		} else {
			SIG_FREE((*it));
		}
	}
	SIG_FOREACH(it, m_children)
	{
		SIG_FREE((*it));
	}
}

void sig::Node::Initialize()
{
	SIG_FOREACH(it, m_components)
	{
		Component *comp = *it;
		if (!comp->m_initialized) {
			comp->Initialize();
			comp->m_initialized = true;
		}
	}
	SIG_FOREACH(it, m_children)
	{
		Node *child = *it;
		if (child == nullptr) {
			continue;
		}
		if (!child->m_dead) {
			child->Initialize();
		}
	}
	m_pickID = pickID;
	pickID++;
}

void sig::Node::Render(SpriteBatch *batch)
{
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_ALWAYS, m_pickID, -1);

	batch->Draw(GetTransformMatrix(), m_size);

	SIG_FOREACH(it, m_children)
	{
		Node *child = *it;
		if (child == nullptr) {
			continue;
		}
		if (!child->m_dead) {
			child->Render(batch);
		}
	}
	SIG_FOREACH(it, m_components)
	{
		Component *comp = *it;
		if (!comp->IsEnabled()) {
			continue;
		}
		comp->Render(batch);
	}
}

void sig::Node::Update(float dt)
{
	if (!m_dead) {
		if (std::round(m_lifeTime) > -1) {
			m_lifeTime -= dt;
			if (std::round(m_lifeTime) == 0) {
				m_dead = true;
			}
		}
	}

	if (m_dead) {
		if (m_parent != nullptr) {
			m_scene->CreateNodeTreeRequest(this, m_parent,
										   NTRAction::NTR_DELETE);
		}
	}

	SIG_FOREACH(it, m_components)
	{
		Component *comp = *it;
		if (!comp->IsEnabled()) {
			continue;
		}
		if (!comp->m_updated) {
			comp->Update(dt);
			comp->m_updated = true;
		}
	}
	SIG_FOREACH(it, m_children)
	{
		Node *child = *it;
		if (child == nullptr) {
			continue;
		}
		if (!child->m_dead) {
			child->Update(dt);
		}
	}
}

void sig::Node::Finalize()
{
	SIG_FOREACH(it, m_components)
	{
		Component *comp = *it;
		if (!comp->IsEnabled()) {
			continue;
		}
		if (comp->m_updated) {
			comp->m_updated = false;
		}
	}
	SIG_FOREACH(it, m_children)
	{
		Node *child = *it;
		child->Finalize();
	}
}

sig::math::Matrix4 sig::Node::GetTransformMatrix()
{
	Matrix4 pos = Matrix4::MakeTranslation(Vector3(m_position.X(), m_position.Y(), 0));
	Matrix4 rot = Matrix4::MakeRotation(m_rotation, Vector3(0, 0, 1));
	Matrix4 ori = Matrix4::MakeTranslation(Vector3(-m_origin.X() * m_size.X(), -m_origin.Y() * m_size.Y(), 0));
	Matrix4 siz = Matrix4::MakeScale(Vector3(m_scale.X(), m_scale.Y(), 1));

	return GetParentTransformMatrix() * pos * siz * rot * ori;
}

sig::math::Matrix4 sig::Node::GetParentTransformMatrix()
{
	Matrix4 ret = Matrix4::Identity;
	if (m_parent != nullptr) {
		ret = m_parent->GetTransformMatrix();
	}
	return ret;
}

sig::Component *sig::Node::GetComponent(const string &nametype)
{
	SIG_FOREACH(it, m_components)
	{
		Component *c = *it;
		if (c->GetName() == nametype) {
			return c;
		}
	}
	return nullptr;
}

void sig::Node::AddComponent(Component* component)
{
	if (component == nullptr) {
		return;
	}
	component->SetEnabled(true);
	component->SetOwner(this);
	component->AddUser();

	m_components.push_back(component);
}

void sig::Node::AddChild(Node* c, float lifeTime)
{
	c->m_scene		= m_scene;
	c->m_dead		= false;
	c->m_lifeTime	= lifeTime;
	c->m_parent		= this;

	c->Initialize();

	m_children.push_back(c);
}

void sig::Node::SetParent(Node* parent)
{
	if (parent != nullptr && m_parent != parent) {
		RemoveParent();

		Vector2 newPos = this->GetPosition() - parent->GetPosition();
		this->SetPosition(newPos);

		parent->AddChild(this);
	}
}

void sig::Node::RemoveParent()
{
	if (m_parent != nullptr) {
		m_parent->RemoveChild(this);
		m_parent = nullptr;
	}
}

sig::Node* sig::Node::RemoveChild(Node* c)
{
	Node *removed = nullptr;
	auto pos = std::find(m_children.begin(), m_children.end(), c);
	if (pos != m_children.end()) {
		auto comps = c->GetComponents();
		SIG_FOREACH(it, comps)
		{
			Component *c = *it;
			c->RemoveUser();
			if (c->GetName() == "RigidBody2D") {
				SIG_FREE(c);
			}
		}
		auto i = std::distance(m_children.begin(), pos);
		removed = m_children[i];
		m_children[i] = nullptr;
		m_children.erase(pos);
	}
	return removed;
}

sig::Node* sig::Node::GetInstance()
{
	Node* newNode = m_scene->CreateNode(m_name);

	newNode->SetPosition(Vector2(m_position));
	newNode->SetScale(Vector2(m_scale));
	newNode->SetOrigin(Vector2(m_origin));
	newNode->SetRotation(m_rotation);

	newNode->m_tag			= m_tag;
	newNode->m_name			= m_name;
	newNode->m_scene		= m_scene;

	SIG_FOREACH(it, m_children)
	{
		Node *child = *it;
		newNode->AddChild(child->GetInstance());
	}

	SIG_FOREACH(it, m_components)
	{
		Component *comp = *it;
		newNode->AddComponent(comp->GetInstance(newNode));
	}

	return newNode;
}

sig::Node* sig::Node::GetChild(const string &name)
{
	SIG_FOREACH(it, m_children)
	{
		Node *child = *it;
		if (child->GetName() == name) {
			return *it;
		}
	}

	return nullptr;
}

void sig::Node::SendMessage(const string &to, const string &body, float delay)
{
	Node *nto = nullptr;

	if (!to.empty()) {
		auto nodes = m_scene->GetAllNodes();
		SIG_FOREACH(it, nodes)
		{
			Node *current = *it;
			if (current->GetName() == to) {
				nto = current;
				break;
			}
		}
	}

	MessageNetwork::Instance()->QueueMessage(this, nto, body, delay, nullptr);
}

void sig::Node::SetPosition(const Vector2& position)
{
	this->m_position = position;
}

void sig::Node::SetRotation(float rotation)
{
	this->m_rotation = rotation;
}

void sig::Node::SetOrigin(const Vector2& origin)
{
	this->m_origin = origin;
}
