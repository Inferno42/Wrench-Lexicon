#include "Wrench.h"
#include "Wrench_Logging.h"
#include "Wrench_Manipulation.h"

extern "C" {
#include "Lua/lua.h" //General lua
#include "Lua/lauxlib.h" //Starting up Lua and registering modules
#include "Lua/luaconf.h" //Config
#include "Lua/lualib.h" //Library options.
};

#define WLua_ModFunctions static const struct luaL_Reg

#define WLUA_STARTEXTERN extern "C" {
#define WLUA_ENDEXTERN };

typedef int (*lua_CFunction) (lua_State *L);
#define WLUA_FUNC(name, data) __declspec(dllexport) static int name(lua_State* l) {data}

#define lua_open()  luaL_newstate() //lua_open is just a macro for luaL_newstate, it has been removed from the files in this version of lua.

class WRENCHLIB_API WLua_Instance
{
	public:
		WLua_Instance();
		~WLua_Instance();
		void Start();
		void Close();
		void LoadFile(std::string file);
		void EnableLogging() {ActiveLogging = true;}
		void DisableLogging() {ActiveLogging = false;}
		int RegisterFunction(char* name, lua_CFunction func);

	private:
		void DisplayErrors(int stat);
		lua_State *L;
		bool ActiveLogging;
		bool Running;
};


