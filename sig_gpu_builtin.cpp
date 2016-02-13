#include "sig_gpu_builtin.h"
#include "SIG_Utilities.h"

sig::Shader* sig::GPUShaders::SIG_GPU_SPRITEDEFAULT = nullptr;

void sig::GPUShaders::Initialize()
{
	SIG_GPU_SPRITEDEFAULT = Shader::FromString(	SIG_GPU_VERT_SPRITEDEFAULT,
												SIG_GPU_FRAG_SPRITEDEFAULT);
}

void sig::GPUShaders::Free()
{
	SIG_FREE(SIG_GPU_SPRITEDEFAULT);
}
