#include "LuaManager.h"

#include "Pandora/Core/IO/File.h"
#include "Pandora/Core/IO/Console.h"

#include <Lua/lua.hpp>

namespace pd {

LuaManager* LuaManager::Get() {
    static LuaManager manager;
    return &manager;
}

ScriptId LuaManager::LoadFromFile(StringView path, const Ref<LuaState>& state) {
    if (!FileExists(path)) return 0;

    LuaScript& script = scripts[scripts.Add()];
    script.id = nextUnusedId++;

    if (state) {
        script.state = state.NewRef();
    } else {
        script.state = Ref<LuaState>::Create();

        if (bindFunc) {
            bindFunc(*script.state);
        }
    }

    String pathStr(Allocator::Temporary);
    pathStr.Set(path);

    if (luaL_dofile((lua_State*)script.state->GetState(), pathStr.CStr())) {
        console.Log("[{}Lua Error{}] failed to compile script {#}\n\tReason: {}",
            ConColor::Red, ConColor::White, path, lua_tostring((lua_State*)script.state->GetState(), -1));
        
        // Remove script
        scripts.Remove(scripts.Count() - 1);
        return 0;
    }

    return script.id;
}

ScriptId LuaManager::LoadFromString(StringView code, const Ref<LuaState>& state) {
    LuaScript& script = scripts[scripts.Add()];
    script.id = nextUnusedId++;

    if (state) {
        script.state = state.NewRef();
    } else {
        script.state = Ref<LuaState>::Create();

        if (bindFunc) {
            bindFunc(*script.state);
        }
    }

    String codeStr(Allocator::Temporary);
    codeStr.Set(code);

    if (luaL_dostring((lua_State*)script.state->GetState(), codeStr.CStr())) {
        console.Log("[{}Lua Error{}] failed to compile script from string\n\tReason: {}",
                    ConColor::Red, ConColor::White, lua_tostring((lua_State*)script.state->GetState(), -1));

        // Remove script
        scripts.Remove(scripts.Count() - 1);
        return 0;
    }

    return script.id;
}

void LuaManager::SetBindFunction(BindFunc func) {
    bindFunc = func;
}

Ref<LuaState> LuaManager::GetState(ScriptId id) {
    if (id == 0) return nullptr;

    return GetScript(id).state;
}

LuaScript LuaManager::GetScript(ScriptId id) {
    if (id == 0) return LuaScript();

    int index = scripts.Find([&](const LuaScript& script) {
        return script.id == id;
    });

    return (index >= 0) ? scripts[index] : LuaScript();
}

}
