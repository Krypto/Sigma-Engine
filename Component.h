#ifndef SIGMA_BEHAVIOUR
#define SIGMA_BEHAVIOUR

#include "IUR.h"
#include "Vector.h"

#include <map>
#include <vector>
#include <string>
using namespace std;

#define COMPONENT_NAME(name) virtual string GetName() const { return name; } 

namespace sig
{
	using namespace math;
	
	class SpriteBatch;
	class Node;
	class Message;
		
	typedef struct _Collision {
		Node* hitNode;
		Vector2 hitNormal;
		Vector2 hitPosition;
	} Collision;
	
	class Component : public IUR
	{
		friend class Node;
	public:
		
		Component()
		{ 
			m_initialized = false;
		}
		Component(Node* owner);
		virtual ~Component();
		
		/**
		 * @brief Called when the Node is Initialized.
		 * @param owner Owner Node
		 */
		virtual void Initialize() {}
		
		/**
		 * @brief Called after the Node is Updated.
		 * @param owner Owner Node
		 * @param dt Delta Time
		 */
		virtual void Update(float dt) {}
		
		/**
		 * @brief Called after the Node is Rendered. For Debug purposes.
		 * @param owner Owner Node
		 */
		virtual void Render(SpriteBatch *batch) {}
		
		/**
		 * @brief Called when the Node receives a Message from the MessageNetwork.
		 * @param owner Owner Node
		 * @param msg Message Object
		 */
		virtual void MessageReceived(Message& msg) {}
		
		/**
		 * @brief Called when the Owner Node starts colliding with another Node.
		 * @param owner Owner Node
		 * @param col Collision information
		 */
		virtual void CollisionEnter(const Collision &col) {}
		
		/**
		 * @brief Called when the Owner Node stops colliding with another Node.
		 * @param owner Owner Node
		 * @param col Collision information
		 */
		virtual void CollisionExit(const Collision &col) {}
		
		void SetEnabled(bool enabled) { this->m_enabled = enabled; }
		bool IsEnabled() const { return m_enabled; }
		
		virtual Component* GetInstance(Node* owner) {
			return new Component(owner);
		}
		
		Node* GetOwner() {return m_owner;}
		void SetOwner(Node *own) {
			if (!m_initialized) {
				m_owner = own;
			}
		}
		
		void AddUser() { m_users++; }
		void RemoveUser() { m_users--; }
		int GetUsers() const { return m_users; }
		
		COMPONENT_NAME("Component")

	protected:
		int m_users;
		bool m_initialized;
		bool m_updated;
		bool m_enabled;
		Node *m_owner;
	};

}

#endif // SIGMA_BEHAVIOUR
