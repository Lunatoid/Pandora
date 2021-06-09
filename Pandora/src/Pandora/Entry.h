#pragma once

#if !defined(PD_LIB)

#include "Pandora/App.h"
#include "Pandora/Core/IO/Console.h"

extern pd::App* pd::CreateApp(int argc, char** argv);

#if defined(PD_WINDOWS) && defined(PD_RELEASE)
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR args, int show) {
    int argc = __argc;
    char** argv = __argv;
#else
int main(int argc, char** argv) {
#endif

    pd::App* app = pd::CreateApp(argc, argv);
    app->Run();
    pd::Delete(app);

    pd::u64 allocated = pd::GetAllocatedBytes();
    PD_ASSERT_D(allocated == 0, "we're leaking %llu bytes", allocated);

    pd::DeleteTemporaryAllocator();

    return 0;
}

#endif
