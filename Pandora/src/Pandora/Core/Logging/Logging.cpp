#include "Logging.h"

#include <string.h>
#include <cstdlib>

namespace pd {

void LogInternal(Stream& out, wchar* remainingFormat) {
    int formatLength = (int)wcslen(remainingFormat);

    bool parsingParam = false;
    for (int i = 0; i < formatLength; i++) {
        if (remainingFormat[i] == L'{') {
            if (i + 1 < formatLength && remainingFormat[i + 1] == L'{') {
                out.WriteByte('{');
                i += 1;
                continue;
            }

            parsingParam = true;
        } else if (remainingFormat[i] == L'}') {
            if (i + 1 < formatLength && remainingFormat[i + 1] == L'}') {
                out.WriteByte('}');
                i += 1;
                continue;
            }

            if (parsingParam) {
                parsingParam = false;
                out.WriteText("{ missing }");
                continue;
            }
        } else if (parsingParam) {
            // No parsing because no args
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

}
