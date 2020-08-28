#pragma once

#include "Pandora/Core/Time/Time.h"
#include "Pandora/Core/Time/Duration.h"

namespace pd {

class Stopwatch {
public:
    Stopwatch();
    
    /// <summary>
    /// Restarts the clock and returns the elapsed time.
    /// </summary>
    /// <returns>The elapsed time.</returns>
    Duration Restart();
    
    /// <summary>
    /// Returns the current elapsed time.
    /// </summary>
    /// <returns>The elapsed current elapsed time.</returns>
    Duration GetElapsed();

private:
    u32 start = 0;
};

}
