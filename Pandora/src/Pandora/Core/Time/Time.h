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

/**
 * \brief Converts a Windows timestamp to a Unix timestamp.
 * 
 * \param windowsTime The Windows timestamp
 * \return The Unix timestamp.
 */
i64 WindowsToUnixTime(u64 windowsTime);

/**
 * \brief Converts a Unix timestamp to a Windows timestamp.
 * 
 * \param unixTime The Unix timestamp.
 * \return The Windows timestamp.
 */
u64 UnixToWindowsTime(i64 unixTime);

/**
 * \return The current time.
 */
Time GetTime();

/**
 * \param time The time.
 * \return The date at that time.
 */
Date GetDate(Time time);

/**
 * \return The ticks in milliseconds since the start of the count.
 */
u32 GetTicks();

/**
 * \brief Sleeps the current thread for a certain.
 * 
 * \param milliseconds The sleep time in milliseconds.
 */
void SleepFor(u32 milliseconds);

template<>
inline void PrintType(Time& type, FormatInfo& info) {

    // Pretty for nominal time notation, otherwise we'll use the dot since it can be accepted in file names
    char seperator = (info.pretty) ? ':' : '.';

    if (type.hours < 10) {
        PrintfToStream(info.output, "0");
    }

    PrintType(type.hours, info);
    PrintfToStream(info.output, "%c%s", seperator, (type.minutes < 10) ? "0" : "");
    PrintType(type.minutes, info);
    PrintfToStream(info.output, "%c%s", seperator, (type.seconds < 10) ? "0" : "");
    PrintType(type.seconds, info);
}

#define PRINT_MONTH(x) case Month::x: PrintfToStream(info.output, "%s", PD_STRINGIFY(x)); break;

inline void PrintType(Month& type, FormatInfo& info) {
    if (info.pretty) {
        switch (type) {
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
        int m = (int)type + 1;
        PrintType(m, info);
    }
}

#undef PRINT_MONTH

#define PRINT_WEEKDAY(x) case Weekday::x: PrintfToStream(info.output, "%s", PD_STRINGIFY(x)); break;

inline void PrintType(Weekday& type, FormatInfo& info) {
    if (info.pretty) {
        switch (type) {
            PRINT_WEEKDAY(Monday);
            PRINT_WEEKDAY(Tuesday);
            PRINT_WEEKDAY(Wednesday);
            PRINT_WEEKDAY(Thursday);
            PRINT_WEEKDAY(Friday);
            PRINT_WEEKDAY(Saturday);
            PRINT_WEEKDAY(Sunday);
        }
    } else {
        int m = (int)type;
        PrintType(m, info);
    }
}

#undef PRINT_WEEKDAY

template<>
inline void PrintType(Date& type, FormatInfo& info) {
    if (info.pretty) {
        PrintType(type.weekday, info);
        PrintfToStream(info.output, ", %s", ((int)type.day < 10) ? "0" : "");
        PrintType(type.day, info);
        PrintfToStream(info.output, " ");
        PrintType(type.month, info);
        PrintfToStream(info.output, " ");
        PrintType(type.year, info);
    } else {
        PrintType(type.year, info);
        PrintfToStream(info.output, "-%s", ((int)type.month < 10) ? "0" : "");
        PrintType(type.month, info);
        PrintfToStream(info.output, "-%s", ((int)type.day < 10) ? "0" : "");
        PrintType(type.day, info);
    }
}

}
