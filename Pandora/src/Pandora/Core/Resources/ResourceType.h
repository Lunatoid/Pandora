#pragma once

#include "Pandora/Core/Logging/PrintType.h"

namespace pd {

enum class ResourceType : byte {
    Unknown,
    Texture,
    Shader,
    Mesh,
    Binary,
    Font,
    Audio,
    Count
};

const StringView RESOURCE_TYPE_NAMES[] = {
    "Unknown",
    "Texture",
    "Shader",
    "Mesh",
    "Binary",
    "Font",
    "Audio",
    "Count"
};

template<>
inline void PrintType(ResourceType& type, FormatInfo& info) {
    PrintfToStream(info.output, "%s", RESOURCE_TYPE_NAMES[(int)type].Data());
}

}
