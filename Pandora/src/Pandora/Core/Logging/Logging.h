#pragma once

#include <cstdio>
#include <cstdlib>

#include "Pandora/Core/Data/StringView.h"
#include "Pandora/Core/IO/Stream.h"
#include "Pandora/Core/Logging/PrintType.h"

//
// The base:
//
// {} : A variable
// {{ : Escaped {
// }} : Escaped }
//
// Things that can go inside {}:
//   D : Decimal
//   O : Octal
//   X : Hexadecimal
//  .n : Precision (integer, can be negative)
//   # : Pretty
//
// Example:
//   {#.-123X} : Pretty print as hexadecimal with a precision of -123

namespace pd {

void LogInternal(Stream& out, wchar* remainingFormat);

template<typename Arg0, typename... Args>
inline void LogInternal(Stream& out, wchar* remainingFormat, const Arg0& arg0, const Args&... args) {
    int formatLength = (int)wcslen(remainingFormat);

    FormatInfo info(out);
    bool parsingParam = false;
    for (int i = 0; i < formatLength; i++) {
        if (remainingFormat[i] == L'{') {
            if (i + 1 < formatLength && remainingFormat[i + 1] == L'{') {
                out.WriteByte('{');
                i += 1;
                continue;
            }

            info.raw = remainingFormat + i;
            info.rawLength = i;

            parsingParam = true;
        } else if (remainingFormat[i] == L'}') {
            if (i + 1 < formatLength && remainingFormat[i + 1] == L'}') {
                out.WriteByte('}');
                i += 1;
                continue;
            }

            if (parsingParam) {
                info.rawLength = i - info.rawLength;

                parsingParam = false;
                PrintType((Arg0&)arg0, info);
                LogInternal(out, remainingFormat + i + 1, args...);
                return;
            }
        } else if (parsingParam) {

            switch (remainingFormat[i]) {
                // Decimal
                case L'D':
                    info.base = DisplayBase::Decimal;
                    break;

                // Octal
                case L'O':
                    info.base = DisplayBase::Octal;
                    break;

                // Hexadecimal
                case L'X':
                    info.base = DisplayBase::Hexadecimal;
                    break;

                case L'#':
                    info.pretty = true;
                    break;

                // Precision
                case L'.':
                    wchar buffer[255];
                    int bufferWrite = 0;

                    i += 1;

                    bool positive = true;
                    if (remainingFormat[i] == L'-') {
                        positive = false;
                        i += 1;
                    }

                    while (remainingFormat[i] >= L'0' && remainingFormat[i] <= L'9') {
                        buffer[bufferWrite] = remainingFormat[i++];

                        if (++bufferWrite >= 254) {
                            break;
                        }
                    }
                    buffer[bufferWrite] = L'\0';

                    info.precisionSpecified = true;
                    info.precision = (int)wcstol(buffer, nullptr, 10) * ((positive) ? 1 : -1);

                    i -= 1;
                    break;
            }

        } else {
            // Convert wide char to char
            char buffer[2];

            size_t written; // ignored
            wcstombs_s(&written, buffer, 2, remainingFormat + i, 1);

            buffer[1] = '\0';

            out.WriteByte(buffer[0]);
        }
    }
}

template<typename... Args>
inline void Log(Stream& out, StringView fmt) {
    wchar* wideFormat = fmt.ToWide();

    LogInternal(out, wideFormat);
}

template<typename... Args>
inline void Log(Stream& out, StringView fmt, const Args&... args) {
    wchar* wideFormat = fmt.ToWide();

    LogInternal(out, wideFormat, args...);
}

}
