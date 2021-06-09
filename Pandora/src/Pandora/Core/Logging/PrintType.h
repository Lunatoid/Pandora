#pragma once

#include <string.h>

#include "Pandora/Core/IO/Stream.h"

namespace pd {

enum class DisplayBase : byte {
    NotSpecified,
    Decimal,
    Octal,
    Hexadecimal
};

struct FormatInfo {
    FormatInfo(Stream& output) : output(output) {}

    const char* raw = nullptr;
    int rawLength = 0;

    Stream& output;

    DisplayBase base = DisplayBase::NotSpecified;

    bool pretty = false;

    bool precisionSpecified = false;
    int precision = 0;
};

/**
 * \brief Formats a printf formatted string and writes it to the output stream.
 * 
 * \param out The stream to log it to.
 * \param fmt The printf format string/
 * \param ... The printf arguments.
 */
void PrintfToStream(Stream& out, const char* fmt, ...);

template<typename T>
inline void PrintType(T& type, FormatInfo& info) {
    PrintfToStream(info.output, "0x%p", &type);
}

template<>
inline void PrintType(i8& type, FormatInfo& info) {
    switch (info.base) {
        case DisplayBase::NotSpecified:
        case DisplayBase::Decimal:
            PrintfToStream(info.output, "%hhd", type);
            break;

        case DisplayBase::Octal:
            PrintfToStream(info.output, "%hho", type);
            break;

        case DisplayBase::Hexadecimal:
            PrintfToStream(info.output, "%s%hhX", (info.pretty) ? "0x" : "", type);
            break;
    }
}

template<>
inline void PrintType(i16& type, FormatInfo& info) {
    switch (info.base) {
        case DisplayBase::NotSpecified:
        case DisplayBase::Decimal:
            PrintfToStream(info.output, "%hd", type);
            break;

        case DisplayBase::Octal:
            PrintfToStream(info.output, "%ho", type);
            break;

        case DisplayBase::Hexadecimal:
            PrintfToStream(info.output, "%s%hX", (info.pretty) ? "0x" : "", type);
            break;
    }
}

template<>
inline void PrintType(i32& type, FormatInfo& info) {
    switch (info.base) {
        case DisplayBase::NotSpecified:
        case DisplayBase::Decimal:
            PrintfToStream(info.output, "%d", type);
            break;

        case DisplayBase::Octal:
            PrintfToStream(info.output, "%o", type);
            break;

        case DisplayBase::Hexadecimal:
            PrintfToStream(info.output, "%s%X", (info.pretty) ? "0x" : "", type);
            break;
    }
}

template<>
inline void PrintType(i64& type, FormatInfo& info) {
    switch (info.base) {
        case DisplayBase::NotSpecified:
        case DisplayBase::Decimal:
            PrintfToStream(info.output, "%lld", type);
            break;

        case DisplayBase::Octal:
            PrintfToStream(info.output, "%llo", type);
            break;

        case DisplayBase::Hexadecimal:
            PrintfToStream(info.output, "%s%llX", (info.pretty) ? "0x" : "", type);
            break;
    }
}

template<>
inline void PrintType(u8& type, FormatInfo& info) {
    switch (info.base) {
        case DisplayBase::NotSpecified:
        case DisplayBase::Decimal:
            PrintfToStream(info.output, "%hhu", type);
            break;

        case DisplayBase::Octal:
            PrintfToStream(info.output, "%hho", type);
            break;

        case DisplayBase::Hexadecimal:
            PrintfToStream(info.output, "%s%hhX", (info.pretty) ? "0x" : "", type);
            break;
    }
}

template<>
inline void PrintType(u16& type, FormatInfo& info) {
    switch (info.base) {
        case DisplayBase::NotSpecified:
        case DisplayBase::Decimal:
            PrintfToStream(info.output, "%hu", type);
            break;

        case DisplayBase::Octal:
            PrintfToStream(info.output, "%ho", type);
            break;

        case DisplayBase::Hexadecimal:
            PrintfToStream(info.output, "%s%hX", (info.pretty) ? "0x" : "", type);
            break;
    }
}

template<>
inline void PrintType(u32& type, FormatInfo& info) {
    switch (info.base) {
        case DisplayBase::NotSpecified:
        case DisplayBase::Decimal:
            PrintfToStream(info.output, "%u", type);
            break;

        case DisplayBase::Octal:
            PrintfToStream(info.output, "%o", type);
            break;

        case DisplayBase::Hexadecimal:
            PrintfToStream(info.output, "%s%X", (info.pretty) ? "0x" : "", type);
            break;
    }
}

template<>
inline void PrintType(u64& type, FormatInfo& info) {
    switch (info.base) {
        case DisplayBase::NotSpecified:
        case DisplayBase::Decimal:
            PrintfToStream(info.output, "%llu", type);
            break;

        case DisplayBase::Octal:
            PrintfToStream(info.output, "%llo", type);
            break;

        case DisplayBase::Hexadecimal:
            PrintfToStream(info.output, "%s%llX", (info.pretty) ? "0x" : "", type);
            break;
    }
}

template<>
inline void PrintType(f32& type, FormatInfo& info) {
    if (info.pretty) {
        PrintfToStream(info.output, "%g", type);
        return;
    }

    switch (info.base) {
        case DisplayBase::NotSpecified:
        case DisplayBase::Decimal:
            if (info.precisionSpecified) {
                PrintfToStream(info.output, "%.*f", info.precision, type);
            } else {
                PrintfToStream(info.output, "%f", type);
            }
            break;

        case DisplayBase::Hexadecimal:
            if (info.precisionSpecified) {
                PrintfToStream(info.output, "%.*A", info.precision, type);
            } else {
                PrintfToStream(info.output, "%A", type);
            }
            break;
    }
}

template<>
inline void PrintType(f64& type, FormatInfo& info) {
    if (info.pretty) {
        PrintfToStream(info.output, "%g", type);
        return;
    }

    switch (info.base) {
        case DisplayBase::NotSpecified:
        case DisplayBase::Decimal:
            if (info.precisionSpecified) {
                PrintfToStream(info.output, "%.*f", info.precision, type);
            } else {
                PrintfToStream(info.output, "%f", type);
            }
            break;

        case DisplayBase::Hexadecimal:
            if (info.precisionSpecified) {
                PrintfToStream(info.output, "%.*A", info.precision, type);
            } else {
                PrintfToStream(info.output, "%A", type);
            }
            break;
    }
}

template<>
inline void PrintType(bool& type, FormatInfo& info) {
    switch (info.base) {
        case DisplayBase::NotSpecified:
            PrintfToStream(info.output, "%s", (type) ? "true" : "false");
            break;

        case DisplayBase::Hexadecimal:
            if (info.pretty) {
                PrintfToStream(info.output, "0x");
            }
            // Fall through

        default:
            PrintfToStream(info.output, "%d", (type) ? 1 : 0);
            break;
    }
}

// Narrow strings

template<>
inline void PrintType(char& type, FormatInfo& info) {
    switch (info.base) {
        case DisplayBase::NotSpecified:
            if (info.precisionSpecified) {
                for (int i = 0; i < info.precision; i++) {
                    PrintfToStream(info.output, "%c", type);
                }
            } else {
                PrintfToStream(info.output, "%c", type);
            }
            break;

        case DisplayBase::Decimal:
            PrintfToStream(info.output, "%d", type);
            break;

        case DisplayBase::Octal:
            PrintfToStream(info.output, "%o", type);
            break;

        case DisplayBase::Hexadecimal:
            PrintfToStream(info.output, "%s%hhX", (info.pretty) ? "0x" : "", type);
            break;
    }
}

template<>
inline void PrintType(const char*& type, FormatInfo& info) {

    int count = (int)strlen(type);

    if (info.precisionSpecified && info.precision >= 0 && info.precision < count) {
        count = info.precision;
    }

    PrintfToStream(info.output, "%.*s", count, type);
}

template<>
inline void PrintType(char*& type, FormatInfo& info) {

    int count = 0;

    if (info.precisionSpecified && info.precision >= 0) {
        count = info.precision;
    }

    if (count > 0) {
        PrintfToStream(info.output, "%.*s", count, type);
    } else {
        PrintfToStream(info.output, "%s", type);
    }
}

template<int size>
inline void PrintType(char(& type)[size], FormatInfo& info) {

    int count = size - 1;

    if (info.precisionSpecified && info.precision >= 0 && info.precision < size) {
        count = info.precision;
    }

    PrintfToStream(info.output, "%.*s", count, type);
}

// Wide strings

#if defined(PD_WINDOWS)

template<>
inline void PrintType(const wchar*& type, FormatInfo& info) {

    int count = (int)wcslen(type);

    if (info.precisionSpecified && info.precision >= 0) {
        count = info.precision;
    }

    char* narrow = (char*)WideToUTF8(type);
    PrintfToStream(info.output, "%.*s", count, narrow);
}

template<>
inline void PrintType(wchar*& type, FormatInfo& info) {

    int count = 0;

    if (info.precisionSpecified && info.precision >= 0 && info.precision < count) {
        count = info.precision;
    }

    char* narrow = (char*)WideToUTF8(type);

    if (count > 0) {
        PrintfToStream(info.output, "%.*s", count, narrow);
    } else {
        PrintfToStream(info.output, "%s", narrow);
    }
}

template<int size>
inline void PrintType(wchar(type)[size], FormatInfo* info) {

    int count = size - 1;

    if (info.precisionSpecified && info.precision >= 0 && info.precision < count) {
        count = info.precision;
    }

    char* narrow = (char*)WideToUTF8(type);
    PrintfToStream(info.output, "%.*s", count, narrow);
}

#endif

}
