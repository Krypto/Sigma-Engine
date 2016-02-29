#include "Resource.h"
#include "Texture2D.h"
#include "AudioClip.h"
#include "TileMapLayer.h"

#include <fstream>
#include <string>

sig::Resource::Resource()
	:	m_rid(0),
	    m_type(RESOURCE_NULL),
	    m_fileName(""),
	    m_name(""),
	    m_loaded(false),
	    m_referenceCount(0),
	    data(0)
{
}

sig::Resource::~Resource()
{
	switch (m_type) {
		default:
		case RESOURCE_NULL: {
			break;
		}
		case RESOURCE_TEXTURE: {
			delete ((Texture2D*) data); break;
		}
		case RESOURCE_TEXT: {
			delete ((char*) data); break;
		}
		case RESOURCE_AUDIO: {
			delete ((AudioClip*) data); break;
		}
	}
	data = nullptr;
}

bool sig::Resource::Load()
{
	if (m_fileName.empty()) { return false; }
	
	switch (m_type) {
		default:
		case RESOURCE_NULL: {
			data = nullptr;
			return true;
		}
		case RESOURCE_TEXTURE: {
			data = new Texture2D(m_fileName);
			return true;
		}
		case RESOURCE_TEXT: {
			std::ifstream ifs(m_fileName);
			data = new std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
			return true;
		}
		case RESOURCE_AUDIO: {
			data = new AudioClip(m_fileName);
			return true;
		}
		case RESOURCE_TILEMAP: {
			data = new TileMapLayer(m_fileName);
			return true;
		}
	}
	return false;
}
