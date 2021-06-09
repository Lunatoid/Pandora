#include "File.h"

#include <sys/stat.h>

#include "Pandora/Core/IO/FileStream.h"

namespace pd {

Mutex& GetIOMutex() {
    // @GLOBAL
    static Mutex ioMutex;
    return ioMutex;
}

bool ReadAllLines(StringView path, String& out, const uchar* newlineSeperator) {
    FileStream file(path, FileMode::Read);

    if (!file.IsOpen()) return false;

    file.ReadAllText(out, newlineSeperator);

    return true;
}

u64 ReadEntireFile(StringView path, String& out) {
    FileStream file(path, FileMode::Read);

    if (!file.IsOpen()) return 0;

    u64 fileSize = file.SizeInBytes();

    // Read file into temp buffer and then append it to the string
    byte* buffer = (byte*)Alloc(fileSize + 1, Allocator::Temporary);
    file.ReadBytes(buffer, fileSize);
    buffer[fileSize] = '\0';

    out.Append(buffer);

    return fileSize;
}

u64 ReadEntireFile(StringView path, Array<byte>& out) {
    FileStream file(path, FileMode::Read);

    if (!file.IsOpen()) return 0;

    u64 fileSize = file.SizeInBytes();

    // Make sure to resize first so that out.Data() is actually allocated
    out.Resize((int)fileSize);

    int oldCount = out.Count();
    byte* reserveStart = out.Data() + oldCount;
    out.Reserve((int)fileSize);
    u64 read = file.ReadBytes(reserveStart, fileSize);

    if (read < fileSize) {
        // We didn't read the same amount, shrink our reserved amount
        out.RemoveRange(oldCount + (int)read, oldCount + (int)fileSize);
    }

    return read;
}

bool WriteEntireFile(StringView path, StringView contents, bool writeBOM) {
    FileStream file(path, FileMode::Write);

    if (!file.IsOpen()) return false;

    if (writeBOM) {
        file.WriteBOM();
    }

    file.WriteText(contents);

    return true;
}

bool WriteEntireFile(StringView path, Slice<byte> contents) {
    FileStream file(path, FileMode::Write);

    if (!file.IsOpen()) return false;

    file.WriteBytes(contents);

    return true;
}

bool FileExists(StringView path) {
    struct stat s;
    return stat(path.CStr(), &s);
}

bool FileDelete(StringView path) {
    return remove(path.CStr()) == 0;
}

u64 GetFileSize(StringView path) {
    FileStream file(path, FileMode::Read);

    if (!file.IsOpen()) return 0;

    return file.SizeInBytes();
}

}

