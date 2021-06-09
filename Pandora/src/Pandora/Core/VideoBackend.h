#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Data/StringView.h"
#include "Pandora/Core/Logging/PrintType.h"

namespace pd {

enum class VideoBackend : byte {
    OpenGL,
    Count,
    Default = OpenGL
};

const StringView VIDEO_BACKEND_NAMES[] = {
    "OpenGL"
};

template<>
inline void PrintType(VideoBackend& type, FormatInfo& info) {
    PrintfToStream(info.output, "%s", VIDEO_BACKEND_NAMES[(int)type].Data());
}

}
