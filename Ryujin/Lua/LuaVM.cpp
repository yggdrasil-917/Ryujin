

#include "LuaVM.hpp"

#include "RyujinCore/Logger/Logger.hpp"


namespace Ryujin
{
    LuaVM::LuaVM()
    {
        luaState = luaL_newstate();
    }
    
    LuaVM::~LuaVM()
    {
        if (luaState)
        {
            lua_close(luaState);
            luaState = nullptr;
        }
    }
    
    LuaVM::LuaVM(LuaVM&& other)
    : luaState(Move(other.luaState)),
      scriptName(Move(other.scriptName))
    {
        other.luaState = nullptr;
    }
    
    LuaVM& LuaVM::operator=(LuaVM&& other)
    {
        luaState = Move(other.luaState);
        scriptName = Move(other.scriptName);
        
        other.luaState = nullptr;
        
        return *this;
    }
    
    bool LuaVM::PushToStack(const String &variableName)
    {
        int32 level = 0;
        String var;
        for (int32 i = 0; i < variableName.Length(); ++i)
        {
            if (variableName[i] == '.')
            {
                if (level == 0)
                {
                    lua_getglobal(luaState, *var);
                }
                else
                {
                    lua_getfield(luaState, -1, *var);
                }
                
                if (lua_isnil(luaState, -1))
                {
                    Errorf(Info, "Variable \"%s\" not defined", *var);
                    return false;
                }
                
                var = "";
                ++level;
            }
            else
            {
                var += variableName[i];
            }
        }
        
        if (level == 0)
        {
            lua_getglobal(luaState, *var);
        }
        else
        {
            lua_getfield(luaState, -1, *var);
        }
        
        if (lua_isnil(luaState, -1))
        {
            Errorf(Info, "Variable \"%s\" not defined", *var);
            return false;
        }
        
        return true;
    }
    
    bool LuaVM::DoFile(const char* filename)
    {
        RYUJIN_ASSERT(luaState, "Lua state has not been initialized");
        
        if (luaL_loadfile(luaState, *String::Printf("../../../Content/Lua/%s", filename)) || lua_pcall(luaState, 0, 0, 0))
        {
            Errorf(Info, "LuaError -> %s", lua_tostring(luaState, -1));
            return false;
        }
        
        scriptName = filename;
        
        return true;
    }
    
    int32 LuaVM::DoString(const char *command)
    {
        RYUJIN_ASSERT(luaState, "Lua state has not been initialized");
        return luaL_dostring(luaState, command);
    }
    
    void LuaVM::CleanStack()
    {
        int32 n = lua_gettop(luaState);
        lua_pop(luaState, n);
    }
}
