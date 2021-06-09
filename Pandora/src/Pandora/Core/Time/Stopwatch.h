#pragma once

#include "Pandora/Core/Time/Time.h"
#include "Pandora/Core/Time/Duration.h"

namespace pd {

class Stopwatch {
public:
    Stopwatch();

    /**
     * \brief Restarts the stopwatch.
     * 
     * \return The elapsed time.
     */
    Duration Restart();

    /**
     * \return The current elapsed time.
     */
    Duration GetElapsed();

private:
    u32 start = 0;
};

}
