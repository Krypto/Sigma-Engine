#ifndef SIGMA_SCENE
#define SIGMA_SCENE

#include "SpriteBatch.h"
#include "Node.h"
#include "Camera2D.h"
#include "Color.h"
#include "IUR.h"

#include <vector>
#include <string>
using namespace std;

#include <Box2D/Box2D.h>

namespace sig
{
	class BaseGame;
	class Scene : public IUR, public b2ContactListener
	{
		friend class BaseGame;
	public:
		Scene(BaseGame *game);
		~Scene();
		
		void BeginContact (b2Contact *contact);
		void EndContact (b2Contact *contact);

		/**
		 * @brief Gets all the Nodes and sub-Nodes attached to ROOT
		 */
		vector<Node*> GetAllNodes();
		
		/**
		 * Creates a new pre-configured Node with all initialized parameters.
		 * 
		 * @brief Creates a new Node.
		 * @param name Node name
		 * @return The node with this Game instance
		 */
		Node* CreateNode(const string &name);
		
		Node* AddChild(Node *c, float lifeTime=-1);
		void RemoveChild(Node *c);
		Node* Instantiate(Node* node, float time=-1);
		Node* GetChild(const string &name);
		
		Node* AddChildInactive(Node *c);
		Node* RemoveChildInactive(const string &name);
		Node* GetChildInactive(const string &name);
		
		void SendMessage(const string &to, const string &body, float delay=0);
		
		void Initialize();
		void Render();
		void Update(float dt);
		void Finalize();
		
		b2World* GetPhysicsWorld() { return m_physicsWorld; }
		
		Scene* SetPixelsPerMeter(float pixelsPerMeter) {this->m_pixelsPerMeter = pixelsPerMeter; return this;}
		float GetPixelsPerMeter() const {return m_pixelsPerMeter;}
		
		void SetAmbient(const Color& ambient) {this->m_ambient = ambient;}
		void SetBackground(const Color& background) {this->m_background = background;}
		const Color& GetAmbient() const {return m_ambient;}
		const Color& GetBackground() const {return m_background;}
		
		void SetCamera(Camera2D* camera) {this->m_camera = camera;}
		Camera2D* GetCamera() {return m_camera;}
		
		Node* GetRoot() {
			if (m_root == nullptr) {
				m_root = CreateNode("[root]");
			}
			return m_root;
		}
		
		BaseGame* GetGame() { return m_game; }
	private:
		Color m_ambient, m_background;
		Node *m_root;
		BaseGame *m_game;
		
		// Physics
		b2World *m_physicsWorld;
		float m_pixelsPerMeter;
		
		Camera2D *m_camera;
		
		vector<Node*> GetAllNodes(Node *r);
	};

}

#endif // SIGMA_SCENE
