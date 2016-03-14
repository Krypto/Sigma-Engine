#include "ResourceManager.h"
#include "SIG_Utilities.h"

sig::ResourceManager* sig::ResourceManager::ms_instance = nullptr;
sig::RID sig::ResourceManager::currentRID = 0;

sig::ResourceManager::ResourceManager()
{
}

sig::ResourceManager::~ResourceManager()
{
}

sig::ResourceManager* sig::ResourceManager::Instance()
{
	if (ms_instance == nullptr) {
		ms_instance = new sig::ResourceManager();
	}
	return ms_instance;
}

void sig::ResourceManager::Release()
{
	if (ms_instance != nullptr) {
		for (auto 	it	= ms_instance->m_resources.begin();
		     it != ms_instance->m_resources.end();
			 it++)
		{
			SIG_FREE((*it).second);
		}
	}
	SIG_FREE(ms_instance);
}

void sig::ResourceManager::AddResource(const string &name, const string &fileName, ResourceType rtype)
{
	currentRID++;

	for (auto it	= m_resources.begin();
	     it != m_resources.end();
		 it++)
	{
		if ((*it).second->m_fileName == fileName) {
			return;
		}
	}

	Resource *res = new Resource();
	res->m_name = name.empty() ? "Resource" + std::to_string(currentRID) : name;
	res->m_rid = currentRID;
	res->m_type = rtype;
	res->m_fileName = fileName;

	m_resources.insert( { res->m_rid, res });
}

sig::Resource* sig::ResourceManager::GetResource(const RID& resourceID)
{
	auto pos = m_resources.find(resourceID);
	if (pos != m_resources.end()) {
		Resource *res = m_resources[resourceID];
		res->Reference();

		return res;
	}
	return nullptr;
}

sig::Resource* sig::ResourceManager::GetResource(const string& name)
{
	for (auto it	= m_resources.begin();
	     it != m_resources.end();
		 it++)
	{
		if ((*it).second->m_name == name) {
			return GetResource((*it).second->m_rid);
		}
	}
	return nullptr;
}

void sig::ResourceManager::LoadAll()
{
	for (auto it	= m_resources.begin();
	     it != m_resources.end();
		 it++)
	{
		if ((*it).second) {
			if (!(*it).second->m_loaded) {
				(*it).second->m_loaded = (*it).second->Load();
			}
		}
	}
}
