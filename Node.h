#ifndef SIGMA_SPATIAL
#define SIGMA_SPATIAL

#include "Component.h"

#include "Rect.h"
#include "Vector.h"
#include "Matrix4.h"

#include <vector>
#include <string>
using namespace std;

#include <Box2D/Box2D.h>

namespace sig
{
	using namespace math;
	
	typedef struct _fix_def_ {
		float friction;
		float density;
		float restitution;
		bool is_sensor;
		b2Shape *shape;
	} Fixture;

	class SpriteBatch;
	class Scene;
	class Node
	{
		friend class Scene;
		friend class Component;
		friend class Sprite;
	public:
		Node();
		virtual ~Node();

		Vector2& GetOrigin() { return m_origin; }
		void SetOrigin(const Vector2& origin);

		Vector2& GetPosition() { return m_position; }
		void SetPosition(const Vector2& position);

		float GetRotation() const { return m_rotation; }
		void SetRotation(float rotation);

		Vector2& GetSize() { return m_size; }
		void SetSize(const Vector2& size) { this->m_size = size; }

		Vector2& GetScale() { return m_scale; }
		void SetScale(const Vector2& scale) { this->m_scale = scale; }

		math::Matrix4 GetTransformMatrix();
		math::Matrix4 GetParentTransformMatrix();
		
		Component* GetComponent(const string &nametype);
		const vector<Component*>& GetComponents() const { return m_components; }
		void AddComponent(Component *component);
		
		Node* GetParent() { return m_parent; }
		Node* SetParent(Node* parent);
		void RemoveParent();
		
		Node* GetChild(const string &name);
		const vector<Node*>& GetChildren() const { return m_children; }		
		Node* AddChild(Node *c, float lifeTime=-1);
		void RemoveChild(Node *c);

		/**
		 * @brief Create a new instance of a Node
		 * @param node The node to Instantiate
		 * @param time The life time of the node (how many seconds until it's deleted)
		 * @return The node instance
		 */
		Node* Instantiate(Node* node, float time=-1);		
		
		Node* AddChildInactive(Node *c);
		Node* RemoveChildInactive(const string &name);
		Node* GetChildInactive(const string &name);
		
		b2Body* GetPhysicsBody() { return m_physicsBody; }
		
		const string& GetTag() const { return m_tag; }
		void SetTag(const string& tag) { this->m_tag = tag; }

		const string& GetName() const { return m_name; }
		void SetName(const string& name) { this->m_name = name; }

		bool IsDead() const { return m_dead; }
		
		Scene* GetScene() { return m_scene; }
		
		/**
		 * @brief Send a message to a specific Node through the MessageNetwork
		 * @param to Node to send the message to (empty string = all nodes)
		 * @param body Message body
		 * @param delay Message delay in seconds
		 * @param udata User data
		 */
		void SendMessage(const string &to, const string &body, float delay=0, void *udata=nullptr);
		
		void ApplyForce(const Vector2 &force, bool wake = true);
		void ApplyForce(const Vector2 &force, const Vector2 &point, bool wake = true);
		void ApplyImpulse(const Vector2 &im, const Vector2 &point, bool wake = true);
		void ApplyTorque(float t, bool wake = true);

		const Vector2 GetLinearVelocity();
		void SetLinearVelocity(const Vector2 &vel);
		float GetAngularVelocity();
		void SetAngularVelocity(float vel);

		float GetMass();
		bool IsAwake();

		void CreatePolygonFixture(const vector<Vector2> &points, float density=0.0f, float friction=0.5f, float restitution=0.0f, bool issensor=false);
		void CreateBoxFixture(float hx, float hy, const Vector2 &center = Vector2::Zero, float angle = 0.0f);
		void CreateFixture(const Fixture &fixture);

		virtual void Initialize();
		virtual void Update(float dt);
		virtual void Render(SpriteBatch *batch);
		void Finalize();
		
		Node* GetInstance();
		
	protected:
		// Identity
		string m_tag;
		string m_name;
		
		// Controllers and Renderers
		vector<Component*> m_components;
		
		// Basic properties
		Vector2 m_position;
		Vector2 m_scale;
		Vector2 m_size;
		Vector2 m_origin;
		float m_rotation;
		
		// Transform hierarchy
		Node *m_parent;
		vector<Node*> m_children;
		// TODO: Move to Scene
		vector<Node*> m_inactiveChildren;
		vector<Node*> m_childrenQueue;
		vector<Node*> m_childrenDeleteQueue;
		
		Scene *m_scene;
		
		// Scene graph
		float m_lifeTime;
		bool m_dead;
		
		// Physics
		b2Body *m_physicsBody;
		void InitBody();
	};
}

#endif // SIGMA_SPATIAL
