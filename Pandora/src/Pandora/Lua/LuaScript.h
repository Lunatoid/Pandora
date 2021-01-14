#pragma once

#include "Pandora/Core/Types.h"

#include "Pandora/Lua/LuaState.h"

namespace pd {

// 0 is reserved for "invalid"
typedef u32 ScriptId;

struct LuaScript {
    ScriptId id = 0;
    Ref<LuaState> state = nullptr;
};

}
