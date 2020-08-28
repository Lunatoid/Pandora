#pragma once

#include "Pandora/Core/Types.h"

#include "Pandora/Core/Logging/PrintType.h"

namespace pd {

struct Time {
    Time(i64 timestamp);

    int hours;
    int minutes;
    int seconds;

    // Unix timestamp
    i64 timestamp;
};

enum class Month : byte {
    January,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December
};

enum class Weekday : byte {
    Sunday = 0, // Windows says that Sunday is the first day of the week
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

struct Date {
    int year = 0;
    Month month = Month::January;
    int day = 0;
    Weekday weekday = Weekday::Sunday;

    int dayInYear = 0;
};

/// <summary>
/// Converts a Windows timestamp to a Unix timestamp.
/// </summary>
/// <param name="windowsTime">The Windows timestamp.</param>
/// <returns>The Unix timestamp.</returns>
i64 WindowsToUnixTime(u64 windowsTime);

/// <summary>
/// Converts a Unix timestamp to a UTC timestamp.
/// </summary>
/// <param name="unixTime">The Unix timestamp.</param>
/// <returns>The Windows timestamp.</returns>
u64 UnixToWindowsTime(i64 unixTime);

/// <summary>
/// Gets the current time.
/// </summary>
/// <param name="asLocal">Whether or not to convert to local time.</param>
/// <returns>The current time.</returns>
Time GetTime(bool asLocal = false);

/// <summary>
/// Gets a date from the time.
/// </summary>
/// <param name="time">The time.</param>
/// <returns>The date at that time.</returns>
Date GetDate(Time time);

/// <summary>
/// Initializes the tick counting.
/// </summary>
void InitTicks();

/// <summary>
/// Gets the milliseconds since ticks have started.
/// If ticks are not initialized it will initialize it.
/// </summary>
/// <returns>The time since the start in milliseconds</returns>
u32 GetTicks();

/// <summary>
/// Sleep the current thread for a certain amount of milliseconds.
/// </summary>
/// <param name="milliseconds">How long to sleep the thread for in milliseconds.</param>
void SleepFor(u32 milliseconds);

template<>
inline void PrintType(Time* value, FormatInfo* info) {

    // Pretty for nominal time notation, otherwise we'll use the dot since it can be accepted in file names
    char seperator = (info->pretty) ? ':' : '.';

    if (value->hours < 10) {
        PrintfToStream(info->output, "0");
    }

    PrintType(&value->hours, info);
    PrintfToStream(info->output, "%c%s", seperator, (value->minutes < 10) ? "0" : "");
    PrintType(&value->minutes, info);
    PrintfToStream(info->output, "%c%s", seperator, (value->seconds < 10) ? "0" : "");
    PrintType(&value->seconds, info);
}

#define PRINT_MONTH(x) case Month::x: PrintfToStream(info->output, "%s", PD_STRINGIFY(x)); break;

inline void PrintType(Month* month, FormatInfo* info) {
    if (info->pretty) {
        switch (*month) {
            PRINT_MONTH(January);
            PRINT_MONTH(February);
            PRINT_MONTH(March);
            PRINT_MONTH(April);
            PRINT_MONTH(May);
            PRINT_MONTH(June);
            PRINT_MONTH(July);
            PRINT_MONTH(August);
            PRINT_MONTH(September);
            PRINT_MONTH(October);
            PRINT_MONTH(November);
            PRINT_MONTH(December);
        }
    } else {
        int m = (int)*month + 1;
        PrintType(&m, info);
    }
}

#undef PRINT_MONTH

#define PRINT_WEEKDAY(x) case Weekday::x: PrintfToStream(info->output, "%s", PD_STRINGIFY(x)); break;

inline void PrintType(Weekday* value, FormatInfo* info) {
    if (info->pretty) {
        switch (*value) {
            PRINT_WEEKDAY(Monday);
            PRINT_WEEKDAY(Tuesday);
            PRINT_WEEKDAY(Wednesday);
            PRINT_WEEKDAY(Thursday);
            PRINT_WEEKDAY(Friday);
            PRINT_WEEKDAY(Saturday);
            PRINT_WEEKDAY(Sunday);
        }
    } else {
        int m = (int)*value;
        PrintType(&m, info);
    }
}

#undef PRINT_WEEKDAY

template<>
inline void PrintType(Date* value, FormatInfo* info) {
    if (info->pretty) {
        PrintType(&value->weekday, info);
        PrintfToStream(info->output, ", %s", ((int)value->day < 10) ? "0" : "");
        PrintType(&value->day, info);
        PrintfToStream(info->output, " ");
        PrintType(&value->month, info);
        PrintfToStream(info->output, " ");
        PrintType(&value->year, info);
    } else {
        PrintType(&value->year, info);
        PrintfToStream(info->output, "-%s", ((int)value->month < 10) ? "0" : "");
        PrintType(&value->month, info);
        PrintfToStream(info->output, "-%s", ((int)value->day < 10) ? "0" : "");
        PrintType(&value->day, info);
    }
}

}
