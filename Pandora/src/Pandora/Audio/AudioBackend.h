#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Data/StringView.h"

#include "Pandora/Core/Logging/PrintType.h"

namespace pd {


enum class AudioBackend : byte {
    SoLoud,
    Count
};

const StringView AUDIO_BACKEND_NAMES[] = {
    "SoLoud"
};

template<>
inline void PrintType(AudioBackend& type, FormatInfo& info) {
    PrintfToStream(info.output, "%s", AUDIO_BACKEND_NAMES[(int)type].Data());
}

}
