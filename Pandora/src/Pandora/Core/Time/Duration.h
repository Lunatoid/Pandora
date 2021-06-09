#pragma once

#include "Pandora/Core/Types.h"

namespace pd {

struct Duration {
    Duration(f64 seconds);
    Duration(u32 milliseconds);

    f64 seconds;
    u32 milliseconds;
    u64 microseconds;
};

}
