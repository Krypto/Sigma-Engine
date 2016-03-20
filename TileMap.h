#ifndef SIGMA_TILEMAP_LAYER
#define SIGMA_TILEMAP_LAYER

#include "Sprite.h"
#include "Matrix4.h"
#include "Texture2D.h"
#include "Shader.h"
#include "Rect.h"
#include "SIG_OpenGL.h"

#include "picojson.h"

#include <map>
#include <vector>
using namespace std;

namespace sig
{
	using namespace math;

	class SpriteBatch;
	class TileMap : public Sprite
	{
	public:
		enum {
			MAP_ORTHOGRAPHIC = 0x01,
			MAP_ISOMETRIC = 0x02
		};

		TileMap();
		TileMap(const string &jsonFilName);
		TileMap(Texture2D *tex);
				
		/**
		 * @brief Put a Tile at a specified position
		 * @param index Tile index in the tile map
		 * @param x X position
		 * @param y Y position
		 */
		void PutTile(int index, int x, int y, int layer);
		
		/**
		 * @brief Configure the tile map 
		 * @param rowCount Number of rows
		 * @param colCount Number of columns
		 */
		void Configure(int rowCount, int colCount);
		
		/**
		 * @brief Load a tile map from JSON
		 * @param filename JSON file name
		 */
		void LoadFromJSON(const string &filename);
		void LoadFromJSONString(const string &data);
		
		void Render(SpriteBatch *batch);
		
		int GetMapHeight() const { return m_mapHeight; }
		void SetMapHeight(int mapHeight) { this->m_mapHeight = mapHeight; }

		int GetMapWidth() const { return m_mapWidth; }
		void SetMapWidth(int mapWidth) { this->m_mapWidth = mapWidth; }

		Texture2D* GetTexture() { return m_texture; }
		void SetTexture(Texture2D* texture) { this->m_texture = texture; }

		int GetTileHeight() const { return m_tileHeight; }
		void SetTileHeight(int tileHeight) { this->m_tileHeight = tileHeight; }

		int GetTileWidth() const { return m_tileWidth; }
		void SetTileWidth(int tileWidth) { this->m_tileWidth = tileWidth; }

		const map<int, vector<int>>& GetMap() const { return m_map; }

		u32 GetProjection() const;
		void SetProjection(const u32 &projection);

		COMPONENT_NAME("TileMap")
	private:
		int m_tileWidth, m_tileHeight;
		int m_mapWidth, m_mapHeight;
		u32 m_projection;
		map<int, vector<int>> m_map;

		void NewLayer(int num);
	};

}

#endif // SIGMA_TILEMAP_LAYER
