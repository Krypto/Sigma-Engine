#ifndef SIGMA_SPATIAL
#define SIGMA_SPATIAL

#include "Component.h"

#include "Vector.h"
#include "Matrix4.h"

#include <vector>
#include <string>
using namespace std;

#include <Box2D/Box2D.h>

namespace sig
{
	using namespace math;
	
	class SpriteBatch;
	class Scene;
	class Node
	{
		friend class Scene;
		friend class Component;
		friend class Sprite;
	public:
		enum PhysicsType {
			PHYSICS_TYPE_DYNAMIC,
			PHYSICS_TYPE_STATIC
		};
		
		enum {
			EMPTY = 2,
			LIGHT = 4
		};
		
		Node();
		virtual ~Node();
		
		virtual int GetType() { return EMPTY; }
		
		Node* SetOrigin(const Vector2& origin) ;
		Node* SetPosition(const Vector2& position) ;
		Node* SetRotation(float rotation) ;
		Node* SetSize(const Vector2& size) { this->m_size = size; return this; }
		const Vector2& GetOrigin() const { return m_origin; }
		const Vector2& GetPosition() const { return m_position; }
		float GetRotation() const { return m_rotation; }
		const Vector2& GetSize() const { return m_size; }
		
		void SetScale(const Vector2& scale) { this->m_scale = scale; }
		const Vector2& GetScale() const { return m_scale; }
				
		math::Matrix4 GetTransformMatrix();
		math::Matrix4 GetParentTransformMatrix();
		
		const vector<Component*>& GetComponent(const string &nametype);
		const vector<Component*>& GetComponents() const { return m_components; }
		void AddComponent(Component *component);
		
		Node* SetParent(Node* parent);
		void RemoveParent();
		
		const vector<Node*>& GetChildren() const { return m_children; }
		Node* GetParent() { return m_parent; }
		
		Node* AddChild(Node *c, float lifeTime=-1);
		void RemoveChild(Node *c);
		Node* Instantiate(Node* node, float time=-1);
		Node* GetChild(const string &name);
		
		Node* AddChildInactive(Node *c);
		Node* RemoveChildInactive(const string &name);
		Node* GetChildInactive(const string &name);
		
		b2Body* GetPhysicsBody() { return m_physicsBody; }		
		Node* SetPhysicsType(const PhysicsType& physicsType) ;
		const PhysicsType& GetPhysicsType() const { return m_physicsType; }
		
		Node* SetTag(const string& tag) {this->m_tag = tag; return this;}
		const string& GetTag() const {return m_tag;}

		Node* SetName(const string& name) {
			this->m_name = name;
			return this;
		}
		const string& GetName() const {return m_name;}

		bool IsDead() const { return m_dead; }
		
		Scene* GetScene() {return m_scene;}
		
		void SendMessage(const string &to, const string &body, float delay=0, void *udata=nullptr);
		
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
		vector<Node*> m_inactiveChildren;
		vector<Node*> m_childrenQueue;
		vector<Node*> m_childrenDeleteQueue;
		
		Scene *m_scene;
		
		// Scene graph
		float m_lifeTime;
		bool m_dead;
		
		// Physics
		b2Body *m_physicsBody;
		PhysicsType m_physicsType;
		void InitBody();
	};
}

#endif // SIGMA_SPATIAL
