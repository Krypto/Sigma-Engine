#include "LuaEngine.h"
#include "SIG_Utilities.h"
#include "MessageNetwork.h"

#include <fstream>
#include <sstream>
#include <streambuf>
#include <algorithm>

sig::Lua::Lua()
	:	L(luaL_newstate())
{
	luaL_openlibs(L);
}

sig::Lua::~Lua()
{
	lua_close(L);
}

sig::Script *sig::Lua::RunScriptModule(const string &fileName)
{
	std::string fileNameN = fileName;
	std::replace(fileNameN.begin(), fileNameN.end(), '\\', '/');

	std::stringstream stream;
	std::ifstream t(fileNameN);
	stream << t.rdbuf();
	std::string ss = stream.str();

	return RunStringModule(ss);
}

sig::Script *sig::Lua::RunStringModule(const string &expression)
{
	Script *ns = new Script();

	LuaState lua = L;
	ErrReport(lua.doString(expression.c_str()));

	LuaRef tableM(L, -1);
	if (tableM.isTable()) {
		if (tableM.has("properties")) {
			ns->Properties = tableM.get<LuaRef>("properties");
		}
		if (tableM.has("initialize")) {
			ns->OnInitialize = tableM.get<LuaRef>("initialize");
		}
		if (tableM.has("update")) {
			ns->OnUpdate = tableM.get<LuaRef>("update");
		}
		if (tableM.has("on_collision_enter")) {
			ns->OnCollisionEnter = tableM.get<LuaRef>("on_collision_enter");
		}
		if (tableM.has("on_collision_exit")) {
			ns->OnCollisionExit = tableM.get<LuaRef>("on_collision_exit");
		}
		if (tableM.has("on_message")) {
			ns->OnMessageReceived = tableM.get<LuaRef>("on_message");
		}
	}
	lua.pop();

	return ns;
}

void sig::Lua::RunScript(const string &fileName)
{
	if (!fileName.empty()) {
		ErrReport(luaL_dofile(L, fileName.c_str()));
	}
}

void sig::Lua::RunString(const string &expression)
{
	if (!expression.empty()) {
		ErrReport(luaL_dostring(L, expression.c_str()));
	}
}

void sig::Lua::ErrReport(int state)
{
	if  (state != 0) {
		SIG_LOG_ERROR(lua_tostring(L, state));
		lua_pop(L, 1);
	}
}


void sig::Script::Initialize()
{
	if (OnInitialize.isFunction()) {
		OnInitialize(GetOwner());
	}
}

void sig::Script::Update(float dt)
{
	if (OnUpdate.isFunction()) {
		OnUpdate(GetOwner(), dt);
	}
}

void sig::Script::MessageReceived(Message *msg)
{
	if (OnMessageReceived.isFunction()) {
		OnMessageReceived(GetOwner(), msg);
	}
}

void sig::Script::CollisionEnter(const Collision &col)
{
	if (OnCollisionEnter.isFunction()) {
		OnCollisionEnter(GetOwner(), col);
	}
}

void sig::Script::CollisionExit(const Collision &col)
{
	if (OnCollisionExit.isFunction()) {
		OnCollisionExit(GetOwner(), col);
	}
}

Script *Script::GetInstance(Node *owner)
{
	Script *inst = new Script();
	inst->OnCollisionEnter = LuaRef(OnCollisionEnter);
	inst->OnCollisionExit = LuaRef(OnCollisionExit);
	inst->OnInitialize = LuaRef(OnInitialize);
	inst->OnMessageReceived = LuaRef(OnMessageReceived);
	inst->OnUpdate = LuaRef(OnUpdate);

	return inst;
}
