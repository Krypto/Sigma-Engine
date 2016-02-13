#ifndef SIGMA_RESOURCE_MANAGER
#define SIGMA_RESOURCE_MANAGER

#include "Resource.h"

#include <memory>
#include <map>
using namespace std;

namespace sig
{

	class ResourceManager
	{
		static ResourceManager* ms_instance;
	public:
		static ResourceManager* Instance();
		static void Release();

		/**
		 * @brief Adds a new Resource to the loading queue.
		 * @param name Resource UNIQUE name
		 * @param fileName Resource file path
		 * @param rtype Resource type
		 */
		void AddResource(const string &name, const string &fileName, ResourceType rtype);
		
		/**
		 * @brief Get a Resource by Resource ID
		 * @param resourceID UNIQUE ID
		 * @return The Resource
		 */
		Resource* GetResource(const RID &resourceID);
		
		/**
		 * @brief Get a Resource by Name
		 * @param name UNIQUE Name
		 * @return The Resource
		 */
		Resource* GetResource(const string &name);
		
		/**
		 * @brief Process and load the Resource queue
		 */
		void LoadAll();
	private:
		static RID currentRID;
		
		ResourceManager();
		~ResourceManager();
		
		map<RID, Resource*> m_resources;
	};

}

#endif // SIGMA_RESOURCE_MANAGER
