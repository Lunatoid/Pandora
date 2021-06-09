#include "Time.h"

#include <time.h>

#include <SDL2/SDL.h>

#include "Pandora/Core/Math/Math.h"

namespace pd {

const i64 UNIX_TIME_START = 0x019db1ded53e8000; // Jan 1, 1970
const i64 TICKS_PER_SECOND = 10000000; // 100ns

const i64 SECONDS_PER_MINUTE = 60;
const i64 SECONDS_PER_HOUR = 60 * 60;
const i64 SECONDS_PER_DAY = 24 * SECONDS_PER_HOUR;

Time::Time(i64 timestamp) :
    timestamp(timestamp) {

    seconds = timestamp % SECONDS_PER_DAY;
    hours = seconds / SECONDS_PER_HOUR;
    seconds -= hours * SECONDS_PER_HOUR;
    minutes = seconds / SECONDS_PER_MINUTE;
    seconds -= minutes * SECONDS_PER_MINUTE;
}

i64 WindowsToUnixTime(u64 windowsTime) {
    return (windowsTime - UNIX_TIME_START) / TICKS_PER_SECOND;
}

u64 UnixToWindowsTime(i64 unixTime) {
    return (unixTime * TICKS_PER_SECOND) + UNIX_TIME_START;
}

Time GetTime() {
    return Time(time(NULL));
}

Date GetDate(Time time) {
    time_t t = time.timestamp;

    tm rawDate;

    // These do the same things right?
#if defined(PD_WINDOWS)
    localtime_s(&rawDate, &t);
#elif defined(PD_LINUX)
    localtime_r(&t, &rawDate);
#endif

    Date date;
    date.day = rawDate.tm_mday;
    date.month = Month(rawDate.tm_mon);
    date.year = 1900 + rawDate.tm_year;

    date.dayInYear = rawDate.tm_yday;
    date.weekday = Weekday(rawDate.tm_wday);

    return date;
}

u32 GetTicks() {
    return SDL_GetTicks();
}

void SleepFor(u32 milliseconds) {
    SDL_Delay(milliseconds);
}

}
