
#pragma once


extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}


#include "RyujinCore/OS/Types.hpp"
#include "RyujinCore/CoreUtils/Common.hpp"
#include "RyujinCore/Containers/String.hpp"
#include "RyujinCore/Containers/DynamicArray.hpp"



namespace Ryujin
{
    class LuaVM : public OnlyRValueCopyable
    {
    private:
        lua_State* luaState;
        String scriptName;
        
        bool PushToStack(const String& variableName);
        
        template<typename T>
        FORCEINLINE void GetFromLua(T& out)
        {
            Warnf(Info, "Using stub implementation of GetFromLua()");
        }
        
    public:
        LuaVM();
        ~LuaVM();
        
        LuaVM(LuaVM&& other);
        LuaVM& operator=(LuaVM&& other);
        
        bool DoFile(const char* filename);
        int32 DoString(const char* command);
        
        void CleanStack();
        
        
        template<typename T>
        T GetValue(const String& variableName, const T defaultValue)
        {
            RYUJIN_ASSERT(luaState, "Lua state has not been initialized");
            
            T value;
            if (PushToStack(variableName))
            {
                GetFromLua<T>(value);
            }
            else
            {
                value = defaultValue;
            }
            
            return value;
        }
        
        template<typename T>
        void GetArray(const String& variableName, DynamicArray<T>& out)
        {
            RYUJIN_ASSERT(luaState, "Lua state has not been initialized");
            
            if (PushToStack(variableName))
            {
                int32 n = 0;
                int32 res = DoString(*String::Printf("arraySize = #%s", *variableName));
                if (res)
                {
                    Warnf(Info, *String::Printf("Failed to run Lua string command to get array size for %s", *variableName));
                    return;
                }
                
                if (PushToStack("arraySize"))
                {
                    GetFromLua<int32>(n);
                    lua_pop(luaState, 1);
                }
                
                if (n > 0)
                {
                    out.Resize(n);
                }
                
                for (int32 i = 0; i < n; ++i)
                {
                    T value;
                    lua_rawgeti(luaState, -1, i+1);
                    GetFromLua<T>(value);
                    out.Add(value);
                    lua_pop(luaState, 1);
                }
            }
            
        }
    };
    
    template<>
    FORCEINLINE void LuaVM::GetFromLua<bool>(bool& out)
    {
        out = lua_toboolean(luaState, -1) == 0 ? false : true;
    }
    
    template<>
    FORCEINLINE void LuaVM::GetFromLua<int32>(int32& out)
    {
        // NOTE: lua number is by default a double and can be changed in luaconf.h by defining LUA_32BITS
        out = (int32)lua_tonumber(luaState, -1);
    }
    
    template<>
    FORCEINLINE void LuaVM::GetFromLua<float>(float& out)
    {
        out = (float)lua_tonumber(luaState, -1);
    }
    
    template<>
    FORCEINLINE void LuaVM::GetFromLua<String>(String& out)
    {
        out = lua_tostring(luaState, -1);
    }
    

}