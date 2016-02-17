#include "TileMapLayer.h"
#include "GUIGFX.h"
#include "sig_gpu_builtin.h"
#include "SIG_Utilities.h"
#include "Node.h"
#include "BaseGame.h"
#include "SpriteBatch.h"

#include <sstream>
#include <string>

sig::TileMapLayer::TileMapLayer()
	:	m_tileWidth(16),
	    m_tileHeight(16),
	    m_mapWidth(1),
	    m_mapHeight(1)
{
	m_map.clear();
	for (int i = 0; i < m_mapWidth * m_mapHeight; i++) {
		m_map.push_back(-1);
	}

}

sig::TileMapLayer::TileMapLayer(const string &jsonFilName)
	:	sig::TileMapLayer()
{
	this->LoadFromJSON(jsonFilName);
}

sig::TileMapLayer::TileMapLayer(Texture2D* tex)
	:	sig::TileMapLayer()
{
	m_texture = tex;
}

void sig::TileMapLayer::Render(SpriteBatch *batch)
{
	if (batch == nullptr) { return; }
	
	if (batch->IsDrawing()) {
		if (m_texture != nullptr) {
			
			int tw = m_tileWidth;
			int th = m_tileHeight;
			
			int xc = m_texture->GetWidth() / tw;
			int yc = m_texture->GetHeight() / th;
			
			float uvw = 1.0f / xc;
			float uvh = 1.0f / yc;
					
			for (int iy = 0; iy < m_mapHeight; iy++) {
				for (int ix = 0; ix < m_mapWidth; ix++) {
					int index = iy * m_mapWidth + ix;
					int tile = m_map[index];
					if (tile < 0) { continue; }
					
					float u = (tile % xc) * uvw;
					float v = int(tile / xc) * uvh;
					
					float x = ix * tw;
					float y = iy * th;
					
					batch->Draw(x, y, 1, 1, 0, 0, 0, Rect(u, v, uvw, uvh), this);
				}
			}
		}
	}
}

void sig::TileMapLayer::PutTile(int index, int x, int y)
{
	u32 id = y * m_mapWidth + x;
	if (id > m_map.size()) { return; }
	m_map[id] = index;
}

void sig::TileMapLayer::Configure(int rowCount, int colCount)
{
	if (m_texture == nullptr) {
		return;
	}
	
	m_tileWidth = m_texture->GetWidth() / colCount;
	m_tileHeight = m_texture->GetHeight() / rowCount;
	
	m_map.clear();
	for (int i = 0; i < m_mapWidth * m_mapHeight; i++) {
		m_map.push_back(-1);
	}
}

void sig::TileMapLayer::LoadFromJSON(const string& filename)
{
	std::ifstream t(filename);
										
	picojson::value v;
	picojson::parse(v, t);
	
	if (v.is<picojson::object>()) {
		picojson::value::object obj = v.get<picojson::object>();
		
		picojson::value tileSize = obj["tileSize"];
		picojson::value mapSize = obj["mapSize"];
		picojson::array data = obj["data"].get<picojson::array>();
		
		m_mapWidth = mapSize.get("x").get<double>();
		m_mapHeight = mapSize.get("y").get<double>();
		m_tileWidth = tileSize.get("x").get<double>();
		m_tileHeight = tileSize.get("y").get<double>();
		
		m_map.clear();
		for (int i = 0; i < m_mapWidth * m_mapHeight; i++) {
			m_map.push_back(-1);
		}
		
		int i = 0;
		for (auto it = data.begin(); it != data.end(); ++it) {
			picojson::value d = (*it);
			m_map[i++] = d.get<double>() - 1;
		}
	}
}
