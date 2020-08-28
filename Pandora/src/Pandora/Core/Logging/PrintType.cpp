#include "PrintType.h"

#include <cstdio>
#include <cstdlib>
#include <stdarg.h>

namespace pd {

void PrintfToStream(Stream* out, const char* fmt, ...) {
    char buffer[1024];

    va_list args;
    va_start(args, fmt);
    int written = vsprintf_s(buffer, 1024, fmt, args);
    va_end(args);

    out->WriteBytes(Slice<byte>((byte*)buffer, written));
}

}
