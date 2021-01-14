#pragma once

#include "Pandora/Core/Data/String.h"

namespace pd {

class LuaState {
public:
    typedef int(*LuaFunction)(LuaState&);
    typedef int(*LuaArgFunction)(LuaState&);

    LuaState(bool openLibs = true, bool engineAPI = true);
    ~LuaState();

    int Call(StringView name, LuaArgFunction argPasser = nullptr);

    void BindFunction(StringView name, LuaFunction func);

    int GetGlobal(StringView name) const;

    void PushValue(const i8& val) const;
    void PushValue(const i16& val) const;
    void PushValue(const i32& val) const;
    void PushValue(const i64& val) const;

    void PushValue(const u8& val) const;
    void PushValue(const u16& val) const;
    void PushValue(const u32& val) const;
    void PushValue(const u64& val) const;

    void PushValue(const f32& val) const;
    void PushValue(const f64& val) const;

    void PushValue(const String& val) const;
    void PushValue(const StringView& val) const;
    void PushValue(const char* val) const;

    void PushValue(void* val) const;
    void PushValue(const bool& val) const;

    i64 ToInteger(int index = -1) const;
    f64 ToNumber(int index = -1) const;
    bool ToBool(int index = -1) const;
    StringView ToString(int index = -1) const;
    const void* ToPointer(int index = -1) const;
    void* ToUserData(int index = -1) const;

    void* GetState() const;

#if defined(PD_LIB)
    int ExecuteFunction(int index);
#endif

private:
    void BindEngineAPI() const;

    Array<LuaFunction> functions;
    void* L = nullptr;
};

}
