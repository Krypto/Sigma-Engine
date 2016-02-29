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

	typedef struct _tile {
		int index;
		int layer;
	} Tile;

	class SpriteBatch;
	class TileMapLayer : public Sprite
	{
	public:
		enum {
			MAP_ORTHOGRAPHIC = 0x01,
			MAP_ISOMETRIC = 0x02
		};

		TileMapLayer();
		TileMapLayer(const string &jsonFilName);
		TileMapLayer(Texture2D *tex);
				
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
		
		void SetMapHeight(int mapHeight) {this->m_mapHeight = mapHeight;}
		void SetMapWidth(int mapWidth) {this->m_mapWidth = mapWidth;}
		void SetTexture(Texture2D* texture) {this->m_texture = texture;}
		void SetTileHeight(int tileHeight) {this->m_tileHeight = tileHeight;}
		void SetTileWidth(int tileWidth) {this->m_tileWidth = tileWidth;}
		const map<int, vector<Tile>>& GetMap() const {return m_map;}
		int GetMapHeight() const {return m_mapHeight;}
		int GetMapWidth() const {return m_mapWidth;}
		Texture2D* GetTexture() {return m_texture;}
		int GetTileHeight() const {return m_tileHeight;}
		int GetTileWidth() const {return m_tileWidth;}
		u32 GetProjection() const;
		void SetProjection(const u32 &projection);

	private:
		int m_tileWidth, m_tileHeight;
		int m_mapWidth, m_mapHeight;
		u32 m_projection;
		map<int, vector<Tile>> m_map;

		void NewLayer(int num);
	};

}

#endif // SIGMA_TILEMAP_LAYER
