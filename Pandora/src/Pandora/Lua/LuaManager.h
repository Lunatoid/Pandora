#pragma once

#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/Data/Reference.h"

#include "Pandora/Lua/LuaState.h"
#include "Pandora/Lua/LuaScript.h"

namespace pd {

class LuaManager {
public:
    typedef void(*BindFunc)(LuaState&);

    static LuaManager* Get();

    ScriptId LoadFromFile(StringView path, const Ref<LuaState>& state = nullptr);
    ScriptId LoadFromString(StringView code, const Ref<LuaState>& state = nullptr);

    // Calls this function with the lua state of a new script (if a custom one is not supplied)
    void SetBindFunction(BindFunc func);

    Ref<LuaState> GetState(ScriptId id);

private:
    LuaScript GetScript(ScriptId id);

    BindFunc bindFunc = nullptr;
    Array<LuaScript> scripts;
    ScriptId nextUnusedId = 1;
};

}