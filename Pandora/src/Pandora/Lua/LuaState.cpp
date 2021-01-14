#include "LuaState.h"

#include "Pandora/Core/IO/Console.h"

#include <Lua/lua.hpp>

namespace pd {

LuaState::LuaState(bool openLibs, bool engineAPI) {
    lua_State* L = luaL_newstate();
    this->L = L;

    if (openLibs) {
        luaL_openlibs(L);
    }

    if (engineAPI) {
        BindEngineAPI();
    }

    lua_pushlightuserdata(L, this);
    lua_setglobal(L, "_PD_STATE_PTR");
}

LuaState::~LuaState() {
    lua_State* L = (lua_State*)this->L;
    lua_close(L);
}

int LuaState::Call(StringView name, LuaArgFunction argPasser) {
    lua_State* L = (lua_State*)this->L;

    String str(Allocator::Temporary);
    str.Set(name);

    int stackSize = lua_gettop(L);

    lua_getglobal(L, str.CStr());
    int argCount = (argPasser) ? argPasser(*this) : 0;

    if (lua_pcall(L, argCount, LUA_MULTRET, 0) != LUA_OK) {
        console.Log("[{}Lua Error{}] failed to call function {#}\n\tReason: {}",
            ConColor::Red, ConColor::White, name, lua_tostring(L, -1));
        return 0;
    }

    return lua_gettop(L) - stackSize;
}

void LuaState::BindFunction(StringView name, LuaFunction func) {
    lua_State* L = (lua_State*)this->L;

    String str(Allocator::Temporary);
    str.Set(name);

    lua_pushlightuserdata(L, this);
    lua_pushinteger(L, functions.Add(func));

    lua_pushcclosure(L, [](lua_State* L) -> int {
        LuaState* state = (LuaState*)lua_topointer(L, lua_upvalueindex(1));
        int index = lua_tonumber(L, lua_upvalueindex(2));

        return state->ExecuteFunction(index);
    }, 2);
    lua_setglobal(L, str.CStr());
}

int LuaState::GetGlobal(StringView name) const {
    lua_State* L = (lua_State*)this->L;

    String str(Allocator::Temporary);
    str.Set(name);

    return lua_getglobal(L, str.CStr());
}

void LuaState::PushValue(const i8& val) const {
    lua_State* L = (lua_State*)this->L;
    lua_pushinteger(L, val);
}

void LuaState::PushValue(const i16& val) const {
    lua_State* L = (lua_State*)this->L;
    lua_pushinteger(L, val);
}

void LuaState::PushValue(const i32& val) const {
    lua_State* L = (lua_State*)this->L;
    lua_pushinteger(L, val);
}

void LuaState::PushValue(const i64& val) const {
    lua_State* L = (lua_State*)this->L;
    lua_pushinteger(L, val);
}

void LuaState::PushValue(const u8& val) const {
    lua_State* L = (lua_State*)this->L;
    lua_pushinteger(L, val);
}

void LuaState::PushValue(const u16& val) const {
    lua_State* L = (lua_State*)this->L;
    lua_pushinteger(L, val);
}

void LuaState::PushValue(const u32& val) const {
    lua_State* L = (lua_State*)this->L;
    lua_pushinteger(L, val);
}

void LuaState::PushValue(const u64& val) const {
    lua_State* L = (lua_State*)this->L;
    lua_pushinteger(L, val);
}

void LuaState::PushValue(const f32& val) const {
    lua_State* L = (lua_State*)this->L;
    lua_pushnumber(L, val);
}

void LuaState::PushValue(const f64& val) const {
    lua_State* L = (lua_State*)this->L;
    lua_pushnumber(L, val);
}

void LuaState::PushValue(const String& val) const {
    lua_State* L = (lua_State*)this->L;
    lua_pushstring(L, val.CStr());
}

void LuaState::PushValue(const StringView& val) const {
    // Convert it to a normal string because we don't know
    // if the StringView is only a fraction of a larger string
    String str(Allocator::Temporary);
    str.Set(val);

    PushValue(str);
}

void LuaState::PushValue(const char* val) const {
    lua_State* L = (lua_State*)this->L;
    lua_pushstring(L, val);
}

void LuaState::PushValue(void* val) const {
    lua_State* L = (lua_State*)this->L;
    lua_pushlightuserdata(L, val);
}

void LuaState::PushValue(const bool& val) const {
    lua_State* L = (lua_State*)this->L;
    lua_pushboolean(L, val);
}

i64 LuaState::ToInteger(int index) const {
    lua_State* L = (lua_State*)this->L;
    return lua_tointeger(L, index);
}

f64 LuaState::ToNumber(int index) const {
    lua_State* L = (lua_State*)this->L;
    return lua_tonumber(L, index);
}

bool LuaState::ToBool(int index) const {
    lua_State* L = (lua_State*)this->L;
    return lua_toboolean(L, index);
}

StringView LuaState::ToString(int index) const {
    lua_State* L = (lua_State*)this->L;
    return lua_tostring(L, index);
}

const void* LuaState::ToPointer(int index) const {
    lua_State* L = (lua_State*)this->L;
    return lua_topointer(L, index);
}

void* LuaState::ToUserData(int index) const {
    lua_State* L = (lua_State*)this->L;
    return lua_touserdata(L, index);
}

void* LuaState::GetState() const {
    return L;
}

int LuaState::ExecuteFunction(int index) {
    return functions[index](*this);
}

void LuaState::BindEngineAPI() const {
    lua_State* L = (lua_State*)this->L;

}

}
