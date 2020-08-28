#include "Time.h"

#include <time.h>

#if defined(PD_WINDOWS)
#include <Windows.h>
#endif

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

Time GetTime(bool asLocal) {
#if defined(PD_WINDOWS)
    FILETIME ft;

    GetSystemTimeAsFileTime(&ft);

    if (asLocal) {
        FILETIME utc = ft;
        FileTimeToLocalFileTime(&utc, &ft);
    }

    u64 dateTime = (u64)ft.dwLowDateTime | ((u64)ft.dwHighDateTime << 32);

    return WindowsToUnixTime(dateTime);
#else
#error Implement me!
#endif
}

Date GetDate(Time time) {
    time_t t = time.timestamp;

    tm rawDate;
    localtime_s(&rawDate, &t);

    Date date;
    date.day = rawDate.tm_mday;
    date.month = Month(rawDate.tm_mon);
    date.year = 1900 + rawDate.tm_year;

    date.dayInYear = rawDate.tm_yday;
    date.weekday = Weekday(rawDate.tm_wday);

    return date;
}

// @GLOBAL
struct {
    bool started = false;
    i64 ticksPerSecond = 0;
    i64 startTicks = 0;
} tickData;

void InitTicks() {
    if (tickData.started) return;
    tickData.started = true;

#if defined(PD_WINDOWS)
    LARGE_INTEGER ticksPerSecond;
    LARGE_INTEGER startTicks;

    QueryPerformanceFrequency(&ticksPerSecond);
    QueryPerformanceCounter(&startTicks);

    tickData.ticksPerSecond = ticksPerSecond.QuadPart;
    tickData.startTicks = startTicks.QuadPart;
#else
#error Implement me!
#endif
}

u32 GetTicks() {
    InitTicks();

#if defined(PD_WINDOWS)
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    now.QuadPart -= tickData.startTicks;
    now.QuadPart *= 1000; // We want our resolution to be milliseconds
    now.QuadPart /= tickData.ticksPerSecond;

    return (u32)now.QuadPart;
#else
#error Implement me!
#endif
}

void SleepFor(u32 milliseconds) {
#if defined(PD_WINDOWS)
    Sleep(milliseconds);
#else
#error Implement me!
#endif
}

}
