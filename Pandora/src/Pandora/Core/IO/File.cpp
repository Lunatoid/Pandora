#include "File.h"

#include "Pandora/Core/IO/FileStream.h"

#include "Pandora/Core/Math/Math.h"

#if defined(PD_WINDOWS)
#include <Windows.h>
#endif

namespace pd {

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
#if defined(PD_WINDOWS)
    DWORD attributes = GetFileAttributesW(path.ToWide());

    return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
#else
#error Implement me!
#endif
}

bool FileDelete(StringView path) {
#if defined(PD_WINDOWS)
    return DeleteFileW(path.ToWide());
#else
#error Implement me!
#endif
}

u64 GetFileSize(StringView path) {
    FileStream file(path, FileMode::Read);

    if (!file.IsOpen()) return 0;

    return file.SizeInBytes();
}

}
