#ifndef SIGMA_SPRITE_BATCH
#define SIGMA_SPRITE_BATCH

#include "Sprite.h"

#include "Vector.h"
#include "Color.h"
#include "Rect.h"
#include "SIG_OpenGL.h"
#include "SIG_Utilities.h"

#include "Shader.h"
#include "Texture2D.h"
#include "sig_gpu_builtin.h"
#include "Matrix4.h"

#include <vector>
using namespace std;

namespace sig
{
	using namespace math;
	
	typedef struct _vertex {
		Vector2 position;
		Vector2 uv;
		Color color;
	} Vertex;
	
	enum class GlyphSortingMode {
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE,
		NONE
	};
	
	class Glyph {
	public:
		~Glyph() {}
		
		Shader *shader;
		Texture2D *texture;
		int z;
		
		Vertex topLeft;
		Vertex topRight;
		Vertex bottomLeft;
		Vertex bottomRight;
		
		math::Matrix4 transform;
	};
	
	class RenderBatch {
	public:
		RenderBatch(Shader *prog, u32 _offset, u32 _numVertices, Texture2D *_texture, math::Matrix4 t)
			:	shader(prog), offset(_offset),
				numVertices(_numVertices), texture(_texture),
				transform(t)
		{}
		
		Shader *shader;
		u32 offset;
		u32 numVertices;
		Texture2D *texture;
		math::Matrix4 transform;
	};
	
	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();
		
		void Initialize();
		void Begin(GlyphSortingMode sortingMode = GlyphSortingMode::TEXTURE);
		void End();
		
		void Draw(Glyph *glyph);
		void Draw(const math::Matrix4 &transform, Shader *shader, float w, float h, const Rect &uv, Texture2D *texture, int z, const Color &col);
		void Draw(const math::Matrix4 &transform, Sprite *sprite);
		void Draw(float x, float y, Sprite *sprite);
		void Draw(float x, float y, float scalex, float scaley, Sprite *sprite);
		void Draw(float x, float y, float scalex, float scaley, float orix, float oriy, float rot, Sprite *sprite);
		void Draw(float x, float y, float scalex, float scaley, float orix, float oriy, float rot, const Rect &uv, Sprite *sprite);
		void Draw(float x, float y, float orix, float oriy, float rot, Sprite *sprite);
		void Draw(float x, float y, float rot, Sprite *sprite);
		
		void Render();
		
		bool IsDrawing() const { return m_drawing; }
	private:
		bool m_drawing;
	
		void CreateRenderBatches();
		void CreateVAO();
		void SortGlyphs();
		
		static bool CompareFRONT_TO_BACK(Glyph *a, Glyph *b);
		static bool CompareBACK_TO_FRONT(Glyph *a, Glyph *b);
		static bool CompareTEXTURE(Glyph *a, Glyph *b);
		
		u32 m_vbo, m_vao;
		GlyphSortingMode m_sortingMode;
		vector<Glyph*> m_glyphs;
		vector<RenderBatch> m_renderBatches;
		
		static int drawOrder;
	};

}

#endif // SIGMA_SPRITE_BATCH
