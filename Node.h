#ifndef SIGMA_SPATIAL
#define SIGMA_SPATIAL

#include "Component.h"

#include "Rect.h"
#include "Vector.h"
#include "Matrix4.h"

#include <vector>
#include <string>
using namespace std;

namespace sig
{
	using namespace math;

	class SpriteBatch;
	class Scene;
	class Node
	{
		friend struct NTR;
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

		float& GetRotation() { return m_rotation; }
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
		void SetParent(Node* parent);
		void RemoveParent();
		
		Node* GetChild(const string &name);
		const vector<Node*>& GetChildren() const { return m_children; }
		void AddChild(Node *c, float lifeTime=-1);
		Node* RemoveChild(Node *c);

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
		void SendMessage(const string &to, const string &body, float delay=0);

		virtual void Initialize();
		virtual void Update(float dt);
		virtual void Render(SpriteBatch *batch);
		void Finalize();
		
		Node* GetInstance();
		
		unsigned int GetPickID() const { return m_pickID; }
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
		
		Scene *m_scene;
		
		// Scene graph
		float m_lifeTime;
		bool m_dead;
		bool m_initialized;

		// Picking
		static unsigned int pickID;
		unsigned int m_pickID;
	};
}

#endif // SIGMA_SPATIAL
