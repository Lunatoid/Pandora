#pragma once

#include <string.h>

namespace pd {

// We don't want to include the strings in our release exe
#if defined(PD_DEBUG)
  #define PD_FILE __FILE__
#else
  #define PD_FILE ""
#endif

#define PD_LINE __LINE__
#define PD_FUNCTION __FUNCTION__

// Hacky workaround to only get the filename instead of the full path
#if defined(PD_DEBUG)
  #if defined(PD_WINDOWS)
    #define PD_FILENAME (strrchr(PD_FILE, '\\') ? strrchr(PD_FILE, '\\') + 1 : PD_FILE)
  #else
    #define PD_FILENAME (strrchr(PD_FILE, '/') ? strrchr(PD_FILE, '/') + 1 : PD_FILE)
  #endif
#else
  #define PD_FILENAME ""
#endif

#define PD_ASSERT(expr, fmt, ...) pd::Crash((expr), PD_FILENAME, PD_FUNCTION, PD_LINE, (fmt),##__VA_ARGS__ )

// This asserts gets taken out on release
#if defined(PD_DEBUG)
  #define PD_ASSERT_D(expr, fmt, ...) PD_ASSERT(expr, fmt,##__VA_ARGS__)
#else
  #define PD_ASSERT_D(expr, fmt, ...)
#endif

#if defined(PD_WINDOWS)

char* GetWin32LastErrorMessage();

#endif

void Crash(bool expr, const char* file, const char* function, int line, const char* fmt, ...);

}
