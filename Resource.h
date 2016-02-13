#ifndef SIGMA_IRESOURCE
#define SIGMA_IRESOURCE

#include <string>
using namespace std;

namespace sig
{
	typedef unsigned int RID;
	
	enum ResourceType {
		RESOURCE_NULL		= 0x0,
		RESOURCE_TEXTURE	= 0x1,
		RESOURCE_AUDIO		= 0x3,
		RESOURCE_TEXT		= 0x4,
		RESOURCE_TILEMAP	= 0x5
	};
	
	class Resource
	{
		friend class ResourceManager;
	public:
		Resource();
		virtual ~Resource();
		
		bool Load();
		
		Resource* SetFileName(const string& fileName) { this->m_fileName = fileName; return this; }
		const string& GetFileName() const { return m_fileName; }
		const string& GetName() const { return m_name; }
		bool IsLoaded() const { return m_loaded; }
		const RID& GetRID() const { return m_rid; }
		const ResourceType& GetType() const { return m_type; }
		
		void Reference() { m_referenceCount++; }
		void DeReference() { m_referenceCount--; if (m_referenceCount < 0) { m_referenceCount = 0; } }
		
		int GetReferenceCount() const { return m_referenceCount; }
		
		void* GetData() {return data;}
	protected:
		RID m_rid;
		ResourceType m_type;
		string m_fileName;
		string m_name;
		bool m_loaded;
		int m_referenceCount;
		void *data;
	};

}

#endif // SIGMA_IRESOURCE
