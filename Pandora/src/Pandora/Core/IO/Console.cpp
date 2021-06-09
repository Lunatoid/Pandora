#include "Console.h"

#include <cstdio>

#if defined(PD_WINDOWS)
#include <Windows.h>
#endif

#include "Pandora/Core/Data/String.h"

namespace pd {

const char* CON_COLOR_ANSI[] = {
    "\x1b[30m",
    "\x1b[2;34m",
    "\x1b[2;32m",
    "\x1b[2;36m",
    "\x1b[2;31m",
    "\x1b[2;35m",
    "\x1b[2;33m",
    "\x1b[0m",
    "\x1b[2;37m",
    "\x1b[34m",
    "\x1b[32m",
    "\x1b[36m",
    "\x1b[31m",
    "\x1b[35m",
    "\x1b[33m",
    "\x1b[37m"
};


Console console;

Console::Console(FILE* stream) : stream(stream) {}

int Console::ReadByte(byte* out) {
    return 0;
}

int Console::ReadBytes(byte* data, u64 length) {
    return 0;
}

int Console::WriteByte(byte b) {
    return (int)fwrite(&b, 1, 1, stream);
}

int Console::WriteBytes(Slice<byte> bytes) {
    return (int)fwrite(bytes.Data(), 1, bytes.SizeInBytes(), stream);
}

void Console::SetColorEnabled(bool isEnabled) {
    colorsEnabled = isEnabled;
}

bool Console::IsColorEnabled() const {
    return colorsEnabled;
}

void Console::SetColor(ConColor color) {
#if defined(PD_WINDOWS)
    // @TODO: preserve background color

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color);
#elif defined(PD_LINUX)
    printf("%s", CON_COLOR_ANSI[(int)color]);
#endif
}

void Console::SetCursor(int x, int y) {
#if defined(PD_WINDOWS)
    COORD coords;
    coords.X = x;
    coords.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coords);
#elif defined(PD_LINUX)
    printf("\033[%d;%dH", x + 1, y + 1);
#endif
}

void Console::SetTitle(StringView title) {
#if defined(PD_WINDOWS)
    SetConsoleTitleW(title.ToWide());
#elif defined(PD_LINUX)
    printf("\033]0;%s\007", title.Data());
#endif
}

void Console::Flush() {
    fflush(stream);
}

void Console::Seek(i64 offset, SeekOrigin origin) {
    // Disallowed
}

bool Console::CanRead() {
    return false;
}

bool Console::CanWrite() {
    return true;
}

bool Console::CanSeek() {
    return false;
}

i64 Console::SizeInBytes() {
    return 0;
}

i64 Console::Position() {
    return 0;
}

}
