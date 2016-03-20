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

sig::Node::Node()
{
	m_origin = Vector2(0, 0);
	m_scale = Vector2(1, 1);
	m_size = Vector2(1);
	m_parent = 0;
	m_lifeTime = -1;
	m_dead = false;
	m_rotation = 0;
	m_scene = nullptr;
	m_physicsBody = nullptr;
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
}

void sig::Node::Render(SpriteBatch *batch)
{
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

	if (m_childrenQueue.size() > 0) {
		SIG_FOREACH(it, m_childrenQueue)
		{
			Node *child = *it;
			child->Initialize();
			m_children.push_back(child);
		}
		m_childrenQueue.clear();
	}

	if (m_childrenDeleteQueue.size() > 0) {
		SIG_FOREACH(it, m_childrenDeleteQueue)
		{
			Node *child = *it;
			auto pos = std::find(m_children.begin(), m_children.end(), child);
			if (pos != m_children.end()) {
				child->RemoveParent();
			}
		}
		m_childrenDeleteQueue.clear();
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

	m_components.push_back(component);
}

sig::Node* sig::Node::AddChild(Node* c, float lifeTime)
{
	c->m_scene		= m_scene;
	c->m_dead		= false;
	c->m_lifeTime	= lifeTime;
	c->m_parent		= this;

	if (c->m_physicsBody == nullptr) {
		c->InitBody();
	}

	m_childrenQueue.push_back(c);

	return this;
}

sig::Node* sig::Node::SetParent(Node* parent)
{
	if (parent != nullptr) {
		parent->AddChild(this);
	} else {
		this->m_parent = parent;
	}
	return this;
}

void sig::Node::RemoveParent()
{
	if (m_parent != nullptr) {
		m_parent->RemoveChild(this);
		m_parent = nullptr;
	}
}

void sig::Node::RemoveChild(Node* c)
{
	auto pos = std::find(m_children.begin(), m_children.end(), c);
	if (pos != m_children.end()) {
		auto comps = c->GetComponents();
		SIG_FOREACH(it, comps)
		{
			(*it)->RemoveUser();
		}
		delete m_children[std::distance(m_children.begin(), pos)];
		m_children[std::distance(m_children.begin(), pos)] = nullptr;
		m_children.erase(pos);
	}
}

sig::Node* sig::Node::GetInstance()
{
	Node* newNode 			= new Node();
	newNode->m_position 	= Vector2(m_position);
	newNode->m_scale 		= Vector2(m_scale);
	newNode->m_origin 		= Vector2(m_origin);
	newNode->m_rotation 	= m_rotation;
	newNode->m_tag 			= m_tag;
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

sig::Node* sig::Node::Instantiate(Node* node, float time)
{
	Node* n = node->GetInstance();
	AddChild(n, time);

	return n;
}

sig::Node* sig::Node::AddChildInactive(Node* c)
{
	c->m_scene		= m_scene;
	c->m_dead		= false;
	c->m_lifeTime	= -1;
	c->m_parent		= this;

	m_inactiveChildren.push_back(c);

	return this;
}

sig::Node* sig::Node::RemoveChildInactive(const string &name)
{
	Node *n;
	SIG_FOREACH(it, m_inactiveChildren)
	{
		Node *child = *it;
		if (child->GetName() == name) {
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

sig::Node* sig::Node::GetChildInactive(const string &name)
{
	SIG_FOREACH(it, m_inactiveChildren)
	{
		Node *child = *it;
		if (child->GetName() == name) {
			return *it;
		}
	}
	return nullptr;
}

void sig::Node::SendMessage(const string &to, const string &body, float delay, void *udata)
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

	MessageNetwork::Instance()->QueueMessage(this, nto, body, delay, udata);
}

void sig::Node::ApplyForce(const Vector2 &force, bool wake)
{
	if (GetPhysicsBody() == nullptr) { return; }
	GetPhysicsBody()->ApplyForceToCenter(b2Vec2(force.X(), force.Y()), wake);
}

void sig::Node::ApplyForce(const Vector2 &force, const Vector2 &point, bool wake)
{
	if (GetPhysicsBody() == nullptr) { return; }
	GetPhysicsBody()->ApplyForce(b2Vec2(force.X(), force.Y()),
								 b2Vec2(point.X(), point.Y()),
								 wake);
}

void sig::Node::ApplyImpulse(const Vector2 &im, const Vector2 &point, bool wake)
{
	if (GetPhysicsBody() == nullptr) { return; }
	GetPhysicsBody()->ApplyLinearImpulse(b2Vec2(im.X(), im.Y()),
										 b2Vec2(point.X(), point.Y()),
										 wake);
}

void sig::Node::ApplyTorque(float t, bool wake)
{
	if (GetPhysicsBody() == nullptr) { return; }
	GetPhysicsBody()->ApplyTorque(t, wake);
}

const sig::math::Vector2 sig::Node::GetLinearVelocity()
{
	if (GetPhysicsBody() == nullptr) { return Vector2::Zero; }
	b2Vec2 v = GetPhysicsBody()->GetLinearVelocity();
	return Vector2(v.x, v.y);
}

void sig::Node::SetLinearVelocity(const Vector2 &vel)
{
	if (GetPhysicsBody() == nullptr) { return; }
	GetPhysicsBody()->SetLinearVelocity(b2Vec2(vel.X(), vel.Y()));
}

float sig::Node::GetAngularVelocity()
{
	if (GetPhysicsBody() == nullptr) { return 0; }
	return GetPhysicsBody()->GetAngularVelocity();
}

void sig::Node::SetAngularVelocity(float vel)
{
	if (GetPhysicsBody() == nullptr) { return; }
	GetPhysicsBody()->SetAngularVelocity(vel);
}

float sig::Node::GetMass()
{
	if (GetPhysicsBody() == nullptr) { return 0; }
	return GetPhysicsBody()->GetMass();
}

bool Node::IsAwake()
{
	if (GetPhysicsBody() == nullptr) { return 0; }
	return GetPhysicsBody()->IsAwake();
}

void sig::Node::CreatePolygonFixture(const vector<Vector2> &points, float density, float friction,
									 float restitution, bool issensor)
{
	b2PolygonShape shape;
	vector<b2Vec2> pts;
	SIG_FOREACH(it, points)
	{
		Vector2 p = *it;
		pts.push_back(b2Vec2(p.X(), p.Y()));
	}
	shape.Set(pts.data(), points.size());

	Fixture f;
	f.density = density;
	f.friction = friction;
	f.shape = &shape;
	f.restitution = restitution;
	f.is_sensor = issensor;

	CreateFixture(f);
}

void Node::CreateBoxFixture(float hx, float hy, const sig::math::Vector2 &center, float angle)
{
	if (GetPhysicsBody() == nullptr) { return; }

	b2PolygonShape shape;
	shape.SetAsBox(hx, hy, b2Vec2(center.X(), center.Y()), angle);
	GetPhysicsBody()->CreateFixture(&shape, 0.0f);
}

void sig::Node::CreateFixture(const Fixture &fixture)
{
	if (GetPhysicsBody() == nullptr) { return; }

	b2FixtureDef fdef;
	fdef.density = fixture.density;
	fdef.friction = fixture.friction;
	fdef.shape = fixture.shape;
	fdef.restitution = fixture.restitution;
	fdef.isSensor = fixture.is_sensor;

	GetPhysicsBody()->CreateFixture(&fdef);
}

void sig::Node::InitBody()
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

void sig::Node::SetPosition(const Vector2& position)
{
	this->m_position = position;

	if (m_physicsBody != nullptr) {
		m_physicsBody->SetTransform(b2Vec2(m_position.X(), m_position.Y()), m_rotation);
	}
}

void sig::Node::SetRotation(float rotation)
{
	this->m_rotation = rotation;

	if (m_physicsBody != nullptr) {
		m_physicsBody->SetTransform(b2Vec2(m_position.X(), m_position.Y()), m_rotation);
	}
}

void sig::Node::SetOrigin(const Vector2& origin)
{
	this->m_origin = origin;
}
