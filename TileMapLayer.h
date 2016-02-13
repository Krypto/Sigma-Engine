#ifndef SIGMA_TILEMAP_LAYER
#define SIGMA_TILEMAP_LAYER

#include "Sprite.h"
#include "Matrix4.h"
#include "Texture2D.h"
#include "Shader.h"
#include "Rect.h"
#include "SIG_OpenGL.h"

#include "picojson.h"

#include <vector>
using namespace std;

namespace sig
{
	using namespace math;
	
	class SpriteBatch;
	class TileMapLayer : public Sprite
	{
	public:
		TileMapLayer();
		TileMapLayer(const string &jsonFilName);
		TileMapLayer(Texture2D *tex);
				
		/**
		 * @brief Put a Tile at a specified position
		 * @param index Tile index in the tile map
		 * @param x X position
		 * @param y Y position
		 */
		void PutTile(int index, int x, int y);
		
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
		
		void Render(SpriteBatch *batch);
		
		TileMapLayer* SetMapHeight(int mapHeight) {this->m_mapHeight = mapHeight; return this;}
		TileMapLayer* SetMapWidth(int mapWidth) {this->m_mapWidth = mapWidth; return this;}
		TileMapLayer* SetTexture(Texture2D* texture) {this->m_texture = texture; return this;}
		TileMapLayer* SetTileHeight(int tileHeight) {this->m_tileHeight = tileHeight; return this;}
		TileMapLayer* SetTileWidth(int tileWidth) {this->m_tileWidth = tileWidth; return this;}
		const vector<int>& GetMap() const {return m_map;}
		int GetMapHeight() const {return m_mapHeight;}
		int GetMapWidth() const {return m_mapWidth;}
		Texture2D* GetTexture() {return m_texture;}
		int GetTileHeight() const {return m_tileHeight;}
		int GetTileWidth() const {return m_tileWidth;}
		
		const int& operator ()(int x, int y) const {
			return m_map[y * m_mapWidth + x];
		}
		int& operator ()(int x, int y) {
			return m_map[y * m_mapWidth + x];
		}
	private:
		int m_tileWidth, m_tileHeight;
		int m_mapWidth, m_mapHeight;
		vector<int> m_map;
	};

}

#endif // SIGMA_TILEMAP_LAYER
