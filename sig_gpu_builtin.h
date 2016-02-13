#ifndef SIGMA_GPU_BUILIN_SHADERS
#define SIGMA_GPU_BUILIN_SHADERS

#include "Shader.h"
using namespace std;

#define STR(x) #x

const string SIG_GPU_VERT_SPRITEDEFAULT = STR(
			attribute vec2 v_position;
			attribute vec2 v_texCoord;
			attribute vec4 v_color;
			varying vec4 color;
			void main() {
				color = v_color;
				gl_Position = gl_ModelViewProjectionMatrix * vec4(v_position, 0.0, 1.0);
				gl_TexCoord[0] = vec4(v_texCoord, 0.0, 1.0);
			}
);

const string SIG_GPU_FRAG_SPRITEDEFAULT = STR(
			uniform sampler2D tex0;
			
			varying vec4 color;
			void main() {
				vec4 finalColor = color * texture2D(tex0, gl_TexCoord[0].st);				
				gl_FragColor = finalColor;
			}
);

namespace sig
{
	class GPUShaders
	{
	public:
		static void Initialize();
		static void Free();
		
		static Shader *SIG_GPU_SPRITEDEFAULT;
	};
}
#endif // SIGMA_GPU_BUILIN_SHADERS