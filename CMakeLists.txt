cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(SigmaEngine)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SigmaEngine_SOURCE_DIR}/CMakeModules)

include(${SigmaEngine_SOURCE_DIR}/CMakeModules/submodules.cmake)

add_subdirectory(${SigmaEngine_SOURCE_DIR}/dependencies/OAL)

set(BOX2D_LIB ${SigmaEngine_SOURCE_DIR}/dependencies/Box2D/lib)
find_library(BOX_2D libBox2D.a ${BOX2D_LIB} STATIC)

find_package(GLEW REQUIRED STATIC)
find_package(SDL2 REQUIRED STATIC)
find_package(Vorbis REQUIRED STATIC)
find_package(VorbisFile REQUIRED STATIC)
find_package(OpenAL REQUIRED STATIC)
find_package(OpenGL REQUIRED STATIC)
find_package(Lua52 REQUIRED STATIC)

include_directories(
	${SDL2_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${Vorbis_INCLUDE_DIR}
	${VorbisFile_INCLUDE_DIR}
	${LUA_INCLUDE_DIR}
	${OpenAL_INCLUDE_DIR}
	${SigmaEngine_SOURCE_DIR}/
	${SigmaEngine_SOURCE_DIR}/dependencies/Box2D/include
	${SigmaEngine_SOURCE_DIR}/dependencies/OAL/include
	${SigmaEngine_SOURCE_DIR}/dependencies/lua-intf
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  ")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" -L\"dependencies/Box2D/lib\" -L\"dependencies/OAL/lib\" ")

# Define the CXX sources
set( CXX_SRCS
	main.cpp
	GameExample.h
	Window.cpp
	GameLoop.cpp
	Time.cpp
	IUR.cpp
	Matrix4.cpp
	Quaternion.cpp
	Vector.cpp
	Color.cpp
	Rect.cpp
	Texture2D.cpp
	BaseGame.cpp
	Input.cpp
	MessageNetwork.cpp
	AnimatedSprite.cpp
	Node.cpp
	DebugDraw.cpp
	Button.cpp
	GUI.cpp
	GUIGFX.cpp
	Label.cpp
	Widget.cpp
	Entry.cpp
	NumberEntry.cpp
	Boolean.cpp
	Slider.cpp
	Box.cpp
	Shader.cpp
	sig_gpu_builtin.cpp
	Resource.cpp
	ResourceManager.cpp
	SoundSystem.cpp
	TileMap.cpp
	Camera2D.cpp
	LightNode.cpp
	SpriteBatch.cpp
	Component.cpp
	Sprite.cpp
	Scene.cpp
	LuaEngine.cpp
	SigmaTypeReg.h
	Sigma.h
	ParticleEmitter.cpp
	Image.cpp
	ListView.cpp
	RigidBody2D.cpp
)

set_source_files_properties(
	${CXX_SRCS} PROPERTIES COMPILE_FLAGS
	" -g -O2 -Wall -std=c++11")

add_executable(sig ${CXX_SRCS} ${C_SRCS})

target_link_libraries(sig
	GL
	GLU
	${GLEW_LIBRARY}
	${BOX_2D}
	${SDL2_LIBRARY}
	OALWrapper
	${VORBIS_LIBRARY}
	${VORBISFILE_LIBRARY}
	${OPENAL_LIBRARY}
	${LUA_LIBRARY}
	dl
)

