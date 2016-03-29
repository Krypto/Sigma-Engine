#include "RigidBody2D.h"
#include "Scene.h"

sig::RigidBody2D::RigidBody2D(b2World *world) : Component()
{
	m_body = nullptr;
	m_world = world;
}

sig::RigidBody2D::~RigidBody2D()
{
	if (m_world != nullptr && m_body != nullptr) {
		m_world->DestroyBody(m_body);
	}
}

void sig::RigidBody2D::Initialize()
{
	if (m_body != nullptr) {
		return;
	} else {
		InitBody();
	}

	m_body->SetUserData(GetOwner());

	vector<Component*> comps = GetOwner()->GetComponents();
	SIG_FOREACH(it, comps)
	{
		Component *c = *it;
		if (c->GetName() == "BoxShape") {
			BoxShape *boxshape = dynamic_cast<BoxShape*>(c);
			b2PolygonShape shape = boxshape->GetBox2DShape();
			m_body->CreateFixture(&shape, 0);
		}
		if (c->GetName() == "CircleShape") {
			CircleShape *circleshape = dynamic_cast<CircleShape*>(c);
			b2CircleShape shape = circleshape->GetBox2DShape();
			m_body->CreateFixture(&shape, 0);
		}
	}
}

void sig::RigidBody2D::Update(float dt)
{
	if (m_body != nullptr) {
		if (m_body->IsAwake()) {
			Scene *sce = GetOwner()->GetScene();
			GetOwner()->SetPosition(Vector2(m_body->GetWorldCenter().x * sce->GetM2P(),
											m_body->GetWorldCenter().y * sce->GetM2P()));
			GetOwner()->SetRotation(-m_body->GetAngle());
		}
	}
}

const sig::math::Vector2 sig::RigidBody2D::GetPosition()
{
	return GetOwner()->GetPosition();
}

void sig::RigidBody2D::SetPosition(const sig::math::Vector2 &pos)
{
	Scene *sce = GetOwner()->GetScene();

	m_body->SetTransform(b2Vec2(pos.X() * sce->GetP2M(),
								pos.Y() * sce->GetP2M()),
						 m_body->GetAngle());
	GetOwner()->SetPosition(pos);
}

float sig::RigidBody2D::GetRotation()
{
	return GetOwner()->GetRotation();
}

void sig::RigidBody2D::SetRotation(float rot)
{
	m_body->SetTransform(m_body->GetWorldCenter(),
						 -rot);
	GetOwner()->SetRotation(rot);
}

const sig::math::Vector2 sig::RigidBody2D::GetLinearVelocity()
{
	b2Vec2 vel = m_body->GetLinearVelocity();
	return Vector2(vel.x, vel.y);
}

void sig::RigidBody2D::SetLinearVelocity(const sig::math::Vector2 &vel)
{
	m_body->SetLinearVelocity(b2Vec2(vel.X(), vel.Y()));
}

sig::RigidBody2D *sig::RigidBody2D::GetInstance(sig::Node *owner)
{
	RigidBody2D *inst = new RigidBody2D(m_world);
	inst->m_owner = owner;
	return inst;
}

void sig::RigidBody2D::ApplyForce(const sig::math::Vector2 &force)
{
	m_body->ApplyForceToCenter(b2Vec2(force.X(), force.Y()), true);
}

void sig::RigidBody2D::ApplyImpulse(const sig::math::Vector2 &impulse, const sig::math::Vector2 &point)
{
	m_body->ApplyLinearImpulse(b2Vec2(impulse.X(), impulse.Y()),
							   b2Vec2(point.X(), point.Y()), true);
}

void sig::RigidBody2D::InitBody()
{
	if (m_body != nullptr) { return; }

	Scene *sce = GetOwner()->GetScene();

	// Initialize Physics
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(
		GetOwner()->GetPosition().X() * sce->GetP2M(),
		GetOwner()->GetPosition().Y() * sce->GetP2M()
	);
	bodyDef.angle = -GetOwner()->GetRotation();
	bodyDef.userData = this;
	bodyDef.fixedRotation = false;

	m_body = m_world->CreateBody(&bodyDef);
}

sig::BoxShape::BoxShape()
{
	m_angle = m_hx = m_hy = 0;
	m_center = Vector2::Zero;
}

sig::BoxShape::BoxShape(float hx, float hy, const Vector2 &center, float angle)
{
	m_hx = hx;
	m_hy = hy;
	m_angle = angle;
	m_center = center;
}

b2PolygonShape sig::BoxShape::GetBox2DShape()
{
	Scene *sce = GetOwner()->GetScene();

	b2PolygonShape s;
	s.SetAsBox(m_hx * sce->GetP2M(),
			   m_hy * sce->GetP2M(),
			   b2Vec2(m_center.X() * sce->GetP2M(),
					  m_center.Y() * sce->GetP2M()),
			   m_angle);
	return s;
}

sig::BoxShape *sig::BoxShape::GetInstance(sig::Node *owner)
{
	BoxShape *inst = new BoxShape(m_hx, m_hy, m_center, m_angle);
	inst->m_owner = owner;
	return inst;
}

sig::CircleShape::CircleShape()
{
	m_radius = 0;
}

sig::CircleShape::CircleShape(float radius)
	:	CircleShape()
{
	m_radius = radius;
}

b2CircleShape sig::CircleShape::GetBox2DShape()
{
	Scene *sce = GetOwner()->GetScene();
	b2CircleShape s;
	s.m_radius = m_radius * sce->GetP2M();
	return s;
}

sig::CircleShape *sig::CircleShape::GetInstance(sig::Node *owner)
{
	CircleShape *inst = new CircleShape(m_radius);
	inst->m_owner = owner;
	return inst;
}
