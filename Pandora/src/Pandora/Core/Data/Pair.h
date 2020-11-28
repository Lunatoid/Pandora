#pragma once

#include "Pandora/Core/Logging/PrintType.h"

namespace pd {

template<typename K, typename V>
struct Pair {
    K key;
    V val;
};

template<typename K, typename V>
inline void PrintType(Pair<K, V>& type, FormatInfo& info) {
    PrintType(type.key, info);
    PrintfToStream(info.output, ": ");
    PrintType(type.val, info);
}

}
