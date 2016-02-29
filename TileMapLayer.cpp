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
		m_mapHeight(1),
		m_projection(MAP_ISOMETRIC)
{
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

			for (auto it = m_map.begin(); it != m_map.end(); ++it) {
				int layer = it->first;
				vector<Tile> data = it->second;
				for (int iy = 0; iy < m_mapHeight; iy++) {
					for (int ix = 0; ix < m_mapWidth; ix++) {
						Tile tile = data[iy * m_mapWidth + ix];
						if (tile.index < 0) { continue; }
						float u = (tile.index % xc) * uvw;
						float v = (tile.index / xc) * uvh;

						float x = (ix * tw) + GetOwner()->GetPosition().X();
						float y = (iy * th) + GetOwner()->GetPosition().Y();
						float px = x, py = y;
						if (m_projection == MAP_ISOMETRIC) {
							x = (ix * tw/2) + GetOwner()->GetPosition().X();
							y = (iy * th/2) + GetOwner()->GetPosition().Y();
							px = (x - y) - tw/2;
							py = ((x + y) / 2.0f) - th/2;
						}
						int sort_key = (int)py + layer;

						batch->Draw(sort_key, px, py, 1, 1, 0, 0, 0, Rect(u, v, uvw, uvh), this);
					}
				}
			}
		}
	}
}

u32 sig::TileMapLayer::GetProjection() const
{
	return m_projection;
}

void sig::TileMapLayer::SetProjection(const u32 &projection)
{
	m_projection = projection;
}

void sig::TileMapLayer::NewLayer(int num)
{
	m_map.insert({num, vector<Tile>(m_mapWidth*m_mapHeight)});
}

void sig::TileMapLayer::PutTile(int index, int x, int y, int layer)
{
	u32 id = y * m_mapWidth + x;
	if (id > m_map.size()) { return; }

	Tile t;
	t.index = index;
	t.layer = layer;

	if (id > m_map[layer].size()) {
		m_map[layer].push_back(t);
	} else {
		m_map[layer][id] = t;
	}
}

void sig::TileMapLayer::Configure(int rowCount, int colCount)
{
	if (m_texture == nullptr) {
		return;
	}
	
	m_tileWidth = m_texture->GetWidth() / colCount;
	m_tileHeight = m_texture->GetHeight() / rowCount;

	m_map.clear();
}

void sig::TileMapLayer::LoadFromJSON(const string& filename)
{
	std::stringstream stream;
	std::ifstream t(filename);
	stream << t.rdbuf();
	std::string filecontent = stream.str();

	LoadFromJSONString(filecontent);
}

void sig::TileMapLayer::LoadFromJSONString(const string &data)
{
	if (data.empty()) { return; }

	picojson::value v;
	picojson::parse(v, data);

	if (v.is<picojson::object>()) {
		picojson::value::object obj = v.get<picojson::object>();

		picojson::value tileSize = obj["tileSize"];
		picojson::value mapSize = obj["mapSize"];
		picojson::value layers = obj["layers"];

		m_mapWidth = mapSize.get("x").get<double>();
		m_mapHeight = mapSize.get("y").get<double>();
		m_tileWidth = tileSize.get("x").get<double>();
		m_tileHeight = tileSize.get("y").get<double>();

		m_map.clear();

		picojson::value::object &objl = layers.get<picojson::object>();
		for (auto it = objl.begin(); it != objl.end(); ++it) {
			std::string layerstr = it->first;
			int layer = ToNumber<int>(layerstr);
			picojson::array _data = it->second.get<picojson::array>();

			m_map.insert({layer, vector<Tile>()});
			for (auto dit = _data.begin(); dit != _data.end(); ++dit) {
				Tile tn;
				tn.layer = layer;
				tn.index = (*dit).get<double>() - 1;
				m_map[layer].push_back(tn);
			}
		}
	}
}
