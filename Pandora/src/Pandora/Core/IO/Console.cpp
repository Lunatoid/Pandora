#include "Console.h"

#include "Pandora/Core/Data/String.h"

#include <cstdio>

#if defined(PD_WINDOWS)
#include <Windows.h>
#endif

namespace pd {

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
#else
#error Implement me!
#endif
}

void Console::SetBackColor(ConColor color) {
#if defined(PD_WINDOWS)
    // @TODO: preserve foreground color

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color << 4);
#else
#error Implement me!
#endif
}

void Console::SetCursor(int x, int y) {
#if defined(PD_WINDOWS)
    COORD coords;
    coords.X = x;
    coords.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coords);
#else
#error Implement me!
#endif
}

void Console::SetTitle(StringView title) {
#if defined(PD_WINDOWS)
    SetConsoleTitleW(title.ToWide());
#else
#error Implement me!
#endif
}

void Console::GetTitle(String* out) {
#if defined(PD_WINDOWS)
    wchar buffer[1024];
    GetConsoleTitleW(buffer, 1024);

    out->Set(WideToUTF8(buffer));
#else
#error Implement me!
#endif
}

void Console::SetVisible(bool isVisible) {
#if defined(PD_WINDOWS)
    ShowWindow(GetConsoleWindow(), (isVisible) ? SW_SHOW : SW_HIDE);
#else
#error Implement me!
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
