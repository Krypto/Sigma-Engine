#ifndef SIGMA_LUA
#define SIGMA_LUA

#include "Component.h"

extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#define LUAINTF_LINK_LUA_COMPILED_IN_CXX 0
#include <LuaIntf/LuaIntf.h>
#include <LuaIntf/LuaState.h>
using namespace LuaIntf;

#include <string>
#include <vector>
#include <map>
using namespace std;

namespace sig {

	class Message;
	class Script : public Component {
	public:
		Script() : Component() {}

		void Initialize();
		void Update(float dt);
		void MessageReceived(Message& msg);
		void CollisionEnter(const Collision &col);
		void CollisionExit(const Collision &col);

		LuaRef	OnInitialize;
		LuaRef	OnUpdate;
		LuaRef	OnMessageReceived;
		LuaRef	OnCollisionEnter;
		LuaRef	OnCollisionExit;
		LuaRef	Properties;

		COMPONENT_NAME("Script")
	};

	class Lua {
	public:
		Lua();
		~Lua();

		lua_State *GetState() { return L; }

		Script *RunScriptModule(const string &fileName);
		Script *RunStringModule(const string &moduleName, const string &expression);
		void RunScript(const string &fileName);
		void RunString(const string &expression);
	private:
		lua_State *L;
		void ErrReport(int state);
	};
}

#define LUA_REGISTER_FUNC() \
	static void Register(Lua *lua)

namespace LuaIntf
{
	LUA_USING_LIST_TYPE(std::vector)
	LUA_USING_MAP_TYPE(std::map)
}

#endif // SIGMA_LUA
