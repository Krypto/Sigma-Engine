#ifndef SIGMA_NETWORK_MESSAGE
#define SIGMA_NETWORK_MESSAGE

#include "IUR.h"
#include "Node.h"
using namespace sig;

#include <string>
#include <vector>
using namespace std;

namespace sig
{
	class Message;
	class BaseGame;
	class MessageNetwork : public IUR
	{
	public:
		/**
		 * @brief Adds a new Message to the Queue.
		 * @param msg Message Object
		 */
		void QueueMessage(Message *msg);
		
		/**
		 * @brief Adds a new Message to the Queue.
		 * @param from Node your Message is coming from.
		 * @param to Node your Message is going to be sent to.
		 * @param body Message body
		 * @param wait How many seconds to wait before sending it.
		 */
		void QueueMessage(Node *from, Node *to, string body, float wait=0, void *udata=nullptr);

		void Update(float dt);

		MessageNetwork* SetGame(BaseGame* game) {
			this->m_game = game;
			return this;
		}

		static MessageNetwork* Instance() {
			if (m_instance == nullptr) {
				m_instance = new MessageNetwork();
			}
			return m_instance;
		}

		static void Release() {
			if (m_instance != nullptr) {
				delete m_instance;
				m_instance = nullptr;
			}
		}
	private:
		BaseGame *m_game;

		MessageNetwork();
		virtual ~MessageNetwork();

		vector<Message*> m_messageQueue, m_deleteQueue;

		void MessageSend(Message *msg);

		static MessageNetwork* m_instance;
	};

	class Message : public IUR
	{
		friend class MessageNetwork;
	public:
		Message() {
			userData = nullptr;
		}
		Message(Message& msg) {
			body 		= msg.GetBody();
			to 			= msg.GetTo();
			from 		= msg.GetFrom();
			m_timeout 	= msg.m_timeout;
			userData	= msg.userData;
		}
		Message(const Message&) {}

		Message& SetBody(const string& body) {
			this->body = body;
			return *this;
		}
		Message& SetFrom(Node* from) {
			this->from = from;
			return *this;
		}
		Message& SetTo(Node* to) {
			this->to = to;
			return *this;
		}
		Message& SetUserData(void* data) {
			userData = data;
			return *this;
		}
		const string& GetBody() const {
			return body;
		}
		Node* GetFrom() {
			return from;
		}
		Node* GetTo() {
			return to;
		}
		
		void* GetUserData() { return userData; }

	protected:
		Node *from;
		Node *to;
		void *userData;
		string body;

		float m_timeout;
	};
}

#endif // SIGMA_NETWORK_MESSAGE
