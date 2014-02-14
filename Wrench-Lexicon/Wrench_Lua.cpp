#include "Wrench_Lua.h"
#include "Wrench_Logging.h"

WLua_Instance::WLua_Instance()
{
	ActiveLogging = false;
	Running = false;
}

WLua_Instance::~WLua_Instance()
{
	if(Running)
		Close();
}

void WLua_Instance::Start()
{
	Running = true;

	if(ActiveLogging == true)
		Wrench::Printfile(" Starting Lua", Wrench::PLACEMENT_PREFIX, __TIMESTAMP__, true, "lua_log.txt");

	this->L = lua_open();
	if(ActiveLogging == true)
		Wrench::Printfile(" Loading packages..", Wrench::PLACEMENT_PREFIX, __TIMESTAMP__, false, "lua_log.txt");

	//Load all available Lua modules
	luaopen_io(L);
	luaopen_base(L);
	luaopen_table(L);
	luaopen_string(L);
	luaopen_math(L);
	luaopen_bit32(L);
	luaopen_coroutine(L);
	luaopen_debug(L);
	luaopen_os(L);
	luaopen_package(L);

	if(ActiveLogging == true)
		Wrench::Printfile(" Lua packages loaded.", Wrench::PLACEMENT_PREFIX, __TIMESTAMP__, false, "lua_log.txt");
	//Finished
}

void WLua_Instance::Close()
{
	if(Running == true)
	{
		if(ActiveLogging == true)
			Wrench::Printfile(" Closing Lua Instance.", Wrench::PLACEMENT_PREFIX, __TIMESTAMP__, false, "lua_log.txt");

		lua_close(L);
		Running = false;
	}
	else
	{
		if(ActiveLogging == true)
			Wrench::Printfile(" Attempted to close a non-running Lua instance.", Wrench::PLACEMENT_PREFIX, __TIMESTAMP__, false, "lua_log.txt");
	}
}

void WLua_Instance::LoadFile( std::string file )
{
	int ret = luaL_loadfile(L, file.c_str());
	if(ret == 0)
	{
		ret = lua_pcall(L, 0, LUA_MULTRET, 0);
	}

	DisplayErrors(ret); //Display any errors. If there are no errors nothing will be called.
}

void WLua_Instance::DisplayErrors( int stat )
{
	if(stat != 0) 
	{
		if(ActiveLogging == true)
		{
			const char* a = Wrench::str_merge(2, " -- ", lua_tolstring(L, -1, NULL));
			Wrench::Print((char*)a);
			Wrench::Printfile((char*)a, Wrench::PLACEMENT_PREFIX, __TIMESTAMP__, false, "lua_error.txt");
			Wrench::Printfile(" Error occurred. Please see lua_error.txt for more details.", Wrench::PLACEMENT_PREFIX, __TIMESTAMP__, false, "lua_log.txt");
		}
		lua_pop(L, 1);
	}
}

int WLua_Instance::RegisterFunction(char* name, lua_CFunction func)
{
	//lua_register(L, name, func);
	lua_pushcfunction(L, func);
	lua_setglobal(L, name);
	//luaL_newlibtable(L, funcs);
	//luaL_setfuncs(L, funcs, 0);
	return 1;
}
