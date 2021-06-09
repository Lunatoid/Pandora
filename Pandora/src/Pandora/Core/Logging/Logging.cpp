#include "Logging.h"

#include <string.h>
#include <cstdlib>

namespace pd {

void LogInternal(Stream& out, const char* remainingFormat) {
    int formatLength = (int)strlen(remainingFormat);

    bool parsingParam = false;
    for (int i = 0; i < formatLength; i++) {
        if (remainingFormat[i] == '{') {
            if (i + 1 < formatLength && remainingFormat[i + 1] == '{') {
                out.Write("{");
                i += 1;
                continue;
            }

            parsingParam = true;
        } else if (remainingFormat[i] == '}') {
            if (i + 1 < formatLength && remainingFormat[i + 1] == '}') {
                out.WriteByte('}');
                i += 1;
                continue;
            }

            if (parsingParam) {
                parsingParam = false;
                out.WriteBytes("{ missing }");
                continue;
            }
        } else if (parsingParam) {
            // No parsing because no args
        } else {
            out.WriteByte((remainingFormat + i)[0]);
        }
    }
}

}
