#include "Stopwatch.h"

namespace pd {

Stopwatch::Stopwatch() {
    Restart();
}

Duration Stopwatch::Restart() {
    u32 end = GetTicks();
    u32 elapsed = end - start;
    start = end;
    return Duration(elapsed);
}

Duration Stopwatch::GetElapsed() {
    return Duration(GetTicks() - start);
}

}
