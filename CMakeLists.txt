cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(SigmaEngine)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SigmaEngine_SOURCE_DIR}/CMakeModules)

set(BOX2D_LIB ${SigmaEngine_SOURCE_DIR}/dependencies/Box2D/lib)
set(OAL_LIB ${SigmaEngine_SOURCE_DIR}/dependencies/OAL/lib)

find_library(BOX_2D libBox2D.a ${BOX2D_LIB})
find_library(OAL_WRAPPER libOALWrapper.a ${OAL_LIB})

find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Vorbis REQUIRED)
find_package(VorbisFile REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Lua52 REQUIRED)

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
    AudioClip.cpp
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
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
	" -g -O2 -Wall -std=c++11")

add_executable(TGT ${CXX_SRCS} ${C_SRCS})

target_link_libraries(TGT
    GL
    GLU
    ${GLEW_LIBRARY}
    ${BOX_2D}
	${SDL2_LIBRARY}
    ${OAL_WRAPPER}
	${VORBIS_LIBRARY}
    ${VORBISFILE_LIBRARY}
	${OPENAL_LIBRARY}
	${LUA_LIBRARY}
	dl
)

