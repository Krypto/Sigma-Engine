#include "TileMap.h"
#include "GUIGFX.h"
#include "sig_gpu_builtin.h"
#include "SIG_Utilities.h"
#include "Node.h"
#include "BaseGame.h"
#include "SpriteBatch.h"

#include <sstream>
#include <string>

sig::TileMap::TileMap()
	:	m_tileWidth(16),
		m_tileHeight(16),
		m_mapWidth(1),
		m_mapHeight(1),
		m_projection(MAP_ORTHOGRAPHIC)
{
}

sig::TileMap::TileMap(const string &jsonFilName)
	:	sig::TileMap()
{
	this->LoadFromJSON(jsonFilName);
}

sig::TileMap::TileMap(Texture2D* tex)
	:	sig::TileMap()
{
	m_texture = tex;
}

void sig::TileMap::Render(SpriteBatch *batch)
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

			float px = 0.0f;
			float py = 0.0f;

			SIG_FOREACH(it, m_map)
			{
				int layer			= it->first;
				vector<int> data	= it->second;

				for (int iy = 0; iy < m_mapHeight; iy++) {
					for (int ix = 0; ix < m_mapWidth; ix++) {
						int index = data[iy * m_mapWidth + ix];
						if (index < 0) {
							continue;
						}

						float u = (index % xc) * uvw;
						float v = int(index / xc) * uvh;

						switch (m_projection) {
							default:
							case MAP_ORTHOGRAPHIC: {
								px = ix * tw;
								py = iy * th;
							} break;
							case MAP_ISOMETRIC: {
								px = (ix - iy) * tw/2;
								py = (ix + iy) * th/2;
							} break;
						}

						px += GetOwner()->GetPosition().X();
						py += GetOwner()->GetPosition().Y();
						int sort_key = int(py) + layer;

						batch->Draw(sort_key, px, py, 1, 1, 0, 0, 0,
									Rect(u, v, uvw, uvh), this);
					}
				}
			}
		}
	}
}

u32 sig::TileMap::GetProjection() const
{
	return m_projection;
}

void sig::TileMap::SetProjection(const u32 &projection)
{
	m_projection = projection;
}

void sig::TileMap::NewLayer(int num)
{
	m_map.insert({num, vector<int>(m_mapWidth * m_mapHeight)});
}

void sig::TileMap::PutTile(int index, int x, int y, int layer)
{
	u32 id = y * m_mapWidth + x;
	if (id > m_map.size()) { return; }

	if (id < m_map[layer].size()) {
		m_map[layer][id] = index;
	}
}

void sig::TileMap::Configure(int rowCount, int colCount)
{
	if (m_texture == nullptr) {
		return;
	}
	
	m_tileWidth = m_texture->GetWidth() / colCount;
	m_tileHeight = m_texture->GetHeight() / rowCount;

	m_map.clear();
}

void sig::TileMap::LoadFromJSON(const string& filename)
{
	std::stringstream stream;
	std::ifstream t(filename);
	stream << t.rdbuf();
	std::string filecontent = stream.str();

	LoadFromJSONString(filecontent);
}

void sig::TileMap::LoadFromJSONString(const string &data)
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
		SIG_FOREACH(it, objl)
		{
			std::string layerstr = it->first;
			int layer = ToNumber<int>(layerstr);
			picojson::array _data = it->second.get<picojson::array>();

			m_map.insert({layer, vector<int>()});
			for (auto dit = _data.begin(); dit != _data.end(); ++dit) {
				m_map[layer].push_back((*dit).get<double>() - 1);
			}
		}
	}
}
