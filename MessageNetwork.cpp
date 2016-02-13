#include "MessageNetwork.h"
#include "BaseGame.h"
#include "Component.h"
#include <algorithm>

sig::MessageNetwork* sig::MessageNetwork::m_instance;

sig::MessageNetwork::MessageNetwork()
{
}

sig::MessageNetwork::~MessageNetwork()
{
}


void sig::MessageNetwork::QueueMessage(Message* msg)
{
	if (m_messageQueue.size() < 64) {
		m_messageQueue.push_back(new Message(*msg));
	} else {
		SIG_LOG_ERROR("Message Queue is Full! Wait until all messages are delivered!");
	}
}

void sig::MessageNetwork::QueueMessage(Node *from, Node *to, string body, float wait, void *udata)
{
	Message *msg = new Message();
	msg->SetBody(body);
	msg->SetFrom(from);
	msg->SetTo(to);
	msg->m_timeout = wait;
	msg->userData = udata;
	
	QueueMessage(msg);
}

void sig::MessageNetwork::Update(float dt)
{
	for (auto it = m_messageQueue.begin(); it != m_messageQueue.end(); ++it) {
		Message *msg = (*it);
		if (msg->m_timeout > 0) {
			msg->m_timeout -= dt;
		} else {
			m_deleteQueue.push_back(msg);
			MessageSend(msg);
		}
	}

	if ( m_deleteQueue.size() > 0) {
		for (auto it = m_deleteQueue.begin(); it != m_deleteQueue.end(); ++it) {
			auto pos = std::find(m_messageQueue.begin(), m_messageQueue.end(), (*it));
			if (pos != m_messageQueue.end()) {
				m_messageQueue.erase(pos);
			}
		}
		m_messageQueue.clear();
		m_deleteQueue.clear();
	}
}
void sig::MessageNetwork::MessageSend(Message* msg)
{
	if (m_game == nullptr) { return; }
	
	Node *to = msg->GetTo();

	if (to == nullptr) {
		auto nodes = m_game->GetCurrentScene()->GetAllNodes();
		for (auto it = nodes.begin(); it != nodes.end(); ++it) {
			auto behavs = (*it)->GetComponents();
			for (auto bi = behavs.begin(); bi != behavs.end(); ++bi) {
				Component* b = (*bi);
				b->MessageReceived(*msg);
			}
		}
	} else {
		auto behavs = to->GetComponents();
		for (auto bi = behavs.begin(); bi != behavs.end(); ++bi) {
			Component* b = (*bi);
			b->MessageReceived(*msg);
		}
	}
}
