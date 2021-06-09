#include "Assert.h"

#include <cstdarg>

#include "Pandora/Core/IO/FileStream.h"
#include "Pandora/Core/IO/File.h"
#include "Pandora/Core/IO/Folder.h"

#include "Pandora/Core/Data/String.h"

#include "Pandora/Core/Time/Time.h"

#if defined(PD_WINDOWS)

#include <Windows.h>
#include <dbghelp.h>

#else
#include <cassert>
#endif

namespace pd {

#if defined(PD_WINDOWS)

char* GetWin32LastErrorMessage() {
    LPSTR message = nullptr;
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&message, 0, NULL);
    return message;
}

void Crash(bool expr, const char* file, const char* function, int line, const char* fmt, ...) {
    if (expr) return;

    const int BUFFER_SIZE = 8192;
    char buffer[BUFFER_SIZE];

    buffer[0] = '\0';

    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);

    String message;
    message.Format("A crash has occurred at:\n{}:{}(...):{}\n\nMessage: {}\n\nA crash log has been generated.",
                   file, function, line, &buffer[0]);
    
    MessageBoxW(NULL, message.ToWide(), L"A crash has occurred!", MB_OK | MB_ICONERROR);

#if defined(PD_DEBUG)
    const int STACK_TRACE_MAX = 255;
    void* stack[STACK_TRACE_MAX];

    // Initialize debug helper
    SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);

    // Get stacktrace
    HANDLE process = GetCurrentProcess();
    SymInitialize(process, NULL, true);

    int count = CaptureStackBackTrace(0, STACK_TRACE_MAX, stack, NULL);
#endif
    
    // Create crashes folder and create crash report
    CreateFolder("crashes");

    Time time = GetTime();
    Date date = GetDate(GetTime());

    String crashPath;
    crashPath.Format("crashes/crash_{}_{}.log", date, time);

    FileStream crashFile(crashPath, FileMode::Write);

    // Something went wrong creating the file
    if (!crashFile.IsOpen()) exit(123);

    crashFile.WriteFormat("=============================================\n");
    crashFile.WriteFormat("= A crash has occurred!\n");
    crashFile.WriteFormat("= Timestamp: {#}, {}\n", time, date);
    crashFile.WriteFormat("= Message:   {}\n", buffer);
    crashFile.WriteFormat("= Location:  {} > {} > L{}\n", file, function, line);
    crashFile.WriteFormat("=============================================\n");

#if defined(PD_DEBUG)

    crashFile.WriteFormat("= Stack trace:\n");

    PSYMBOL_INFOW symbol = (PSYMBOL_INFOW)Alloc(sizeof(SYMBOL_INFOW) + (MAX_SYM_NAME - 1) * sizeof(TCHAR), Allocator::Temporary);
    
    symbol->SizeOfStruct = sizeof(SYMBOL_INFOW);
    symbol->MaxNameLen = MAX_SYM_NAME;

    for (int i = 0; i < count; i++) {
        if (SymFromAddrW(process, (DWORD64)stack[i], NULL, symbol)) {
            crashFile.WriteFormat("= > {}()\n", (wchar*)symbol->Name);
        } else {
            crashFile.WriteFormat("= > (failed to get symbol, error code: {})\n", GetLastError());
        }
    }

    crashFile.WriteFormat("=============================================\n");
    crashFile.Close();

    // We're crashing baby!
    __debugbreak();

#endif

    exit(123);
}

#elif defined(PD_LINUX)

void Crash(bool expr, const char* file, const char* function, int line, const char* fmt, ...) {
    if (expr) return;

    const int BUFFER_SIZE = 8192;
    char buffer[BUFFER_SIZE];

    buffer[0] = '\0';

    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);

    String message;
    message.Format("A crash has occurred at:\n{}:{}(...):{}\n\nMessage: {}\n\nA crash log has been generated.",
                   file, function, line, &buffer[0]);

    printf("%s\n", message.Data());

    exit(123);
}

#endif

}
