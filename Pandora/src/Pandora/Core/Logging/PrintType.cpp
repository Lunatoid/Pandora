#include "PrintType.h"

#include <cstdio>
#include <cstdlib>
#include <stdarg.h>

namespace pd {

void PrintfToStream(Stream& out, const char* fmt, ...) {
    const int BUFFER_SIZE = 8192;
    char buffer[BUFFER_SIZE];

    va_list args;
    va_start(args, fmt);
    int written = vsprintf(buffer, fmt, args);
    va_end(args);

    out.WriteBytes(Slice<byte>((byte*)buffer, written));
}

}
