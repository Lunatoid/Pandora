#include "FileStream.h"

#include <cstdarg>

#if defined(PD_WINDOWS)
#include <Windows.h>
#endif

#if defined(PD_LINUX)
#include <unistd.h>
#endif

namespace pd {

FileStream::FileStream(StringView path, FileMode mode) {
    Open(path, mode);
}


FileStream::~FileStream() {
    Close();
}

bool FileStream::Open(StringView path, FileMode mode) {
    if (IsOpen()) {
        Close();
    }

    this->mode = mode;

    BoundedString<8> modeString;

    switch (mode) {
        case FileMode::Read: modeString.Append("rb"); break;
        case FileMode::Write: modeString.Append("wb"); break;
        case FileMode::ReadWrite: modeString.Append("r+b"); break;
        case FileMode::Truncate: modeString.Append("w+b"); break;
        case FileMode::Append: modeString.Append("ab");
    }

#if defined(PD_WINDOWS)
    wchar* widePath = path.ToWide();
    wchar* wideMode = modeString.ToWide();

    _wfopen_s(&file, widePath, wideMode);

#elif defined(PD_LINUX)
    file = fopen(path.CStr(), modeString.CStr());
#endif

    endOfFile = !IsOpen();
    return IsOpen();
}

void FileStream::Close() {
    if (IsOpen()) {
        Flush();

        fclose(file);
        file = nullptr;
    }
}

int FileStream::ReadByte(byte* out) {
    if (!CanRead() || !IsOpen()) return 0;

    int read = (int)fread(out, 1, 1, file);

    endOfFile = read != 1;
    return read;
}

int FileStream::ReadBytes(byte* data, u64 length) {
    if (!CanRead() || !IsOpen()) return 0;

    int read = (int)fread(data, 1, length, file);

    endOfFile = read != (int)length;
    return read;
}

int FileStream::WriteByte(byte b) {
    if (!CanWrite() || !IsOpen()) return 0;

    return (int)fwrite(&b, 1, 1, file) != 0;
}

int FileStream::WriteBytes(Slice<byte> bytes) {
    if (!CanWrite() || !IsOpen()) return 0;

    return (int)fwrite(bytes.Data(), 1, bytes.SizeInBytes(), file);
}

int FileStream::WriteFormatF(const uchar* fmt, ...) {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    buffer[0] = '\0';

    va_list args;
    va_start(args, fmt);
    int bytesWritten = vsprintf(buffer, (char*)fmt, args);
    va_end(args);

    return WriteText(StringView(buffer, (int)UTF8Count(buffer), (int)UTF8Size(buffer) - 1));
}

int FileStream::WriteBOM() {
    return WriteText("\xEF\xBB\xBF");
}

void FileStream::Flush() {
    if (!IsOpen()) return;

    fflush(file);
}

void FileStream::Seek(i64 offset, SeekOrigin origin) {
    if (!CanSeek() || !IsOpen()) return;

    int seek = 0;

    i64 oldPos = Position();

    switch (origin) {
        case SeekOrigin::Start:   seek = SEEK_SET; break;
        case SeekOrigin::Current: seek = SEEK_CUR; break;
        case SeekOrigin::End:     seek = SEEK_END; break;
    }

#if defined(PD_WINDOWS)
    _fseeki64(file, offset, seek);
#elif defined(PD_LINUX)
    fseeko64(file, offset, seek);
#endif
    
    if (endOfFile && Position() < oldPos) {
        // We were at the end of the file but now we've seeked backwards
        // Assume that we're no longer at the end of the file
        endOfFile = false;
    }
}

bool FileStream::CanRead() {
    return !EndOfFile() && (mode == FileMode::Read || mode == FileMode::ReadWrite || mode == FileMode::Truncate);
}

bool FileStream::CanWrite() {
    return !EndOfFile() && (mode == FileMode::Write || mode == FileMode::ReadWrite || mode == FileMode::Append || mode == FileMode::Truncate);
}

bool FileStream::CanSeek() {
    return (mode != FileMode::Append);
}

bool FileStream::EndOfFile() {
    return endOfFile;
}

bool FileStream::IsOpen() {
    return file != nullptr;
}

i64 FileStream::SizeInBytes() {
    if (!IsOpen()) return 0;

    i64 currentPos = Position();

    Seek(0, SeekOrigin::End);
    i64 size = Position();
    Seek(currentPos, SeekOrigin::Start);

    return size;
}

i64 FileStream::Position() {
    if (!IsOpen()) return 0;

    // Would it be wise to avoid ftell and instead track our own position with read/writes/seeks?
#if defined(PD_WINDOWS)
    return _ftelli64(file);
#elif defined(PD_LINUX)
    return ftello64(file);
#endif
}

}
