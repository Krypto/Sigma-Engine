#ifndef SIGMA_SCENE
#define SIGMA_SCENE

#include "SpriteBatch.h"
#include "Node.h"
#include "Camera2D.h"
#include "Color.h"
#include "GUI.h"
#include "IUR.h"

#include <vector>
#include <queue>
#include <string>
using namespace std;

#include <Box2D/Box2D.h>

namespace sig
{
	enum class NTRAction {
		NTR_ADD,
		NTR_ADD_VOLATILE,
		NTR_DELETE,
		NTR_REPARENT,
		NTR_INSTANTIATE
	};

	// Node tree modification request
	class Scene;
	typedef struct NTR {
		Node *src;
		Node *dest;
		NTRAction action;
		float time;

		NTR(Node *from, Node *to, NTRAction a, float t = -1)
			:	src(from), dest(to), action(a), time(t)
		{}

		void Process(Scene* scene);
	} NodeTreeRequest;

	class BaseGame;
	class Scene : public IUR, public b2ContactListener
	{
		friend class BaseGame;
		friend struct NTR;
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
		
		void AddNode(Node *c, float lifeTime=-1);
		void RemoveNode(Node *c);
		void Instantiate(Node* node, float time=-1);
		Node* GetNode(const string &name);
		Node* GetLastAdded() { return m_lastAdded; }
		void ReparentNodes(Node* src, Node* dest);
		
		void AddInactiveNode(Node *c);
		void RemoveInactiveNode(const string &name);
		Node* GetInactiveNode(const string &name);
		
		void SendMessage(const string &to, const string &body, float delay=0);
		
		void Initialize();
		void Render();
		void Update(float dt);
		void Finalize();
		
		b2World* GetPhysicsWorld() { return m_physicsWorld; }
		
		void SetPixelsPerMeter(float pixelsPerMeter) { this->m_pixelsPerMeter = pixelsPerMeter; }
		float GetPixelsPerMeter() const {return m_pixelsPerMeter;}
		
		void SetAmbient(const Color& ambient) {this->m_ambient = ambient;}
		void SetBackground(const Color& background) {this->m_background = background;}
		const Color& GetAmbient() const {return m_ambient;}
		Color& GetBackground() {return m_background;}
		
		void SetCamera(Camera2D* camera) {this->m_camera = camera;}
		Camera2D* GetCamera() {return m_camera;}
		
		Node* GetRoot() {
			if (m_root == nullptr) {
				m_root = CreateNode("[root]");
			}
			return m_root;
		}
		
		BaseGame* GetGame() { return m_game; }

		GUI* GetGUI() { return m_gui; }
		void SetGUI(GUI *gui) { m_gui = gui; }

		void CreateNodeTreeRequest(Node *src, Node *dest, NTRAction action, float time=-1);
	private:
		Color m_ambient, m_background;
		Node *m_root, *m_lastAdded;
		BaseGame *m_game;
		GUI *m_gui;
		
		// Physics
		b2World *m_physicsWorld;
		float m_pixelsPerMeter;
		
		Camera2D *m_camera;
		
		vector<Node*> GetAllNodes(Node *r);

		// Node manager
		vector<Node*> m_inactiveNodes;
		queue<NodeTreeRequest*> m_nodeTreeRequests;
	};

}

#endif // SIGMA_SCENE
