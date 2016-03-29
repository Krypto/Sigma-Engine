#include "Resource.h"
#include "Texture2D.h"
#include "TileMap.h"
#include "SoundSystem.h"

#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

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
			delete static_cast<Texture2D*>(data); break;
		}
		case RESOURCE_TEXT: {
			delete static_cast<std::string*>(data); break;
		}
		case RESOURCE_AUDIO: {
			delete static_cast<AudioSample*>(data); break;
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
			FILE *f = fopen(m_fileName.c_str(), "rb");
			if (f) {
				u32 len = 0;
				fseek(f, 0, SEEK_END);
				len = ftell(f);
				rewind(f);

				Byte *buffer;
				buffer = (Byte*) calloc(1, len+1);
				fread(buffer, len, 1, f);
				fclose(f);

				data = new AudioSample((void*)buffer, len);
			}
			return true;
		}
		case RESOURCE_TILEMAP: {
			data = new TileMap(m_fileName);
			return true;
		}
	}
	return false;
}
