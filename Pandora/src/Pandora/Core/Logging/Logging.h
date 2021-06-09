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

/**
 * \brief The implementation of the logging function.
 * 
 * \param out The stream to log it to.
 * \param remainingFormat The remaining format string.
 */
void LogInternal(Stream& out, const char* remainingFormat);

/**
 * \brief The implementation of the logging function.
 * 
 * \tparam Arg0 The current argument to print.
 * \tparam Args The rest of the arguments.
 * \param out The stream to log it to.
 * \param remainingFormat The remaining format string.
 * \param arg0 The current argument to print.
 * \param args The remaining arguments.
 */
template<typename Arg0, typename... Args>
inline void LogInternal(Stream& out, const char* remainingFormat, const Arg0& arg0, const Args&... args) {
    int formatLength = (int)strlen(remainingFormat);

    FormatInfo info(out);
    bool parsingParam = false;
    for (int i = 0; i < formatLength; i++) {
        if (remainingFormat[i] == '{') {
            if (i + 1 < formatLength && remainingFormat[i + 1] == '{') {
                out.WriteByte('{');
                i += 1;
                continue;
            }

            info.raw = remainingFormat + i;
            info.rawLength = i;

            parsingParam = true;
        } else if (remainingFormat[i] == '}') {
            if (i + 1 < formatLength && remainingFormat[i + 1] == '}') {
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
                case 'D':
                    info.base = DisplayBase::Decimal;
                    break;

                // Octal
                case 'O':
                    info.base = DisplayBase::Octal;
                    break;

                // Hexadecimal
                case 'X':
                    info.base = DisplayBase::Hexadecimal;
                    break;

                case '#':
                    info.pretty = true;
                    break;

                // Precision
                case '.':
                    char buffer[255];
                    int bufferWrite = 0;

                    i += 1;

                    bool positive = true;
                    if (remainingFormat[i] == '-') {
                        positive = false;
                        i += 1;
                    }

                    while (remainingFormat[i] >= '0' && remainingFormat[i] <= '9') {
                        buffer[bufferWrite] = remainingFormat[i++];

                        if (++bufferWrite >= 254) {
                            break;
                        }
                    }
                    buffer[bufferWrite] = '\0';

                    info.precisionSpecified = true;
                    info.precision = (int)strtol(buffer, nullptr, 10) * ((positive) ? 1 : -1);

                    i -= 1;
                    break;
            }

        } else {
            out.WriteByte((remainingFormat + i)[0]);
        }
    }
}

/**
 * \brief Prints a formatted string to the specified stream.
 * This variant gets called when no additional arguments are supplied.
 * 
 * \tparam Args The log arguments.
 * \param out The stream to log it to.
 * \param fmt The format string.
 */
template<typename... Args>
inline void Log(Stream& out, StringView fmt) {
    LogInternal(out, fmt.CStr());
}

/**
 * \brief Prints a formatted string to the specified stream.
 * 
 * \tparam Args The log arguments.
 * \param out The stream to log it to.
 * \param fmt The format string.
 * \param args The arguments.
 */
template<typename... Args>
inline void Log(Stream& out, StringView fmt, const Args&... args) {
    LogInternal(out, fmt.CStr(), args...);
}

}
