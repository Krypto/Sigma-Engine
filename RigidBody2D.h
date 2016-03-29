#ifndef SIGMA_RIGID_BODY_2D
#define SIGMA_RIGID_BODY_2D

#include "Vector.h"
#include "Component.h"

#include <Box2D/Box2D.h>

#include <vector>
using namespace std;

namespace sig {
	using namespace math;

	template <typename T>
	class IShape2D : public Component
	{
	public:
		virtual T GetBox2DShape() = 0;
		virtual IShape2D *GetInstance(Node *owner) = 0;

		COMPONENT_NAME("IShape2D");
	};

	class BoxShape : public IShape2D<b2PolygonShape>
	{
	public:
		BoxShape();
		BoxShape(float hx, float hy,
				 const Vector2& center = Vector2::Zero,
				 float angle=0);

		b2PolygonShape GetBox2DShape() override;

		virtual BoxShape *GetInstance(Node *owner) override;

		COMPONENT_NAME("BoxShape");
	private:
		float m_hx, m_hy, m_angle;
		Vector2 m_center;
	};

	class CircleShape : public IShape2D<b2CircleShape>
	{
	public:
		CircleShape();
		CircleShape(float radius);

		b2CircleShape GetBox2DShape() override;

		virtual CircleShape *GetInstance(Node *owner) override;

		COMPONENT_NAME("CircleShape");
	private:
		float m_radius;
	};

	class RigidBody2D : public Component
	{
	public:
		RigidBody2D(b2World *world);
		~RigidBody2D() override;

		void Initialize() override;
		void Update(float dt) override;

		b2Body *GetBody() { return m_body; }

		int GetType() const { return int(m_body->GetType()); }
		void SetType(int type) { m_body->SetType(b2BodyType(type)); }

		const Vector2 GetPosition();
		void SetPosition(const Vector2& pos);

		float GetRotation();
		void SetRotation(float rot);

		const Vector2 GetLinearVelocity();
		void SetLinearVelocity(const Vector2& vel);

		RigidBody2D *GetInstance(Node *owner) override;

		void ApplyForce(const Vector2& force);
		void ApplyImpulse(const Vector2& impulse, const Vector2& point);

		COMPONENT_NAME("RigidBody2D");
	private:
		b2Body *m_body;
		b2World *m_world;
		void InitBody();
	};
}

#endif // SIGMA_RIGID_BODY_2D
