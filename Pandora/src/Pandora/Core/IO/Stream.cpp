#include "Stream.h"

#include "Pandora/Core/Logging/Logging.h"

#include "Pandora/Core/Data/Memory.h"
#include "Pandora/Core/Data/String.h"

namespace pd {

int Stream::ReadBytes(byte* data, u64 length) {
    if (!CanRead()) return 0;

    for (u64 i = 0; i < length; i++) {
        bool success = ReadByte(&data[i]) == 1;

        if (!success) return (int)i;
    }

    return (int)length;
}

int Stream::ReadCodepoint(codepoint* out) {
    if (!CanRead()) {
        *out = '\0';
        return 0;
    }

    // Read up to 4 bytes
    byte characters[4];

    int read = ReadBytes(characters, 4);
    if (read == 0) {
        *out = '\0';
        return read;
    }

    GetNextCodepoint(characters, out);

    int size = CodepointSize(*out);

    // Seek backwards, we want to stay aligned with the UTF-8
    Seek((i64)size - read);

    return size;
}

int Stream::PeekCodepoint(codepoint* out) {
    if (!CanRead()) return false;

    int read = ReadCodepoint(out);
    Seek(-read);
    return read;
}

bool Stream::SkipBOMIfPresent() {
    if (!CanRead()) return false;

    byte bomCheck[3];
    int read = ReadBytes(bomCheck, 3);

    // We could't read 3 bytes
    if (read != 3) {
        Seek(-read);
        return false;
    }

    if (!MemoryCompare(bomCheck, (void*)"\xEF\xBB\xBF", 3)) {
        // Data was not BOM, rewind
        Seek(-3);
        return false;
    }

    return true;
}

void Stream::ReadLine(String& out) {
    if (!CanRead()) return;

    // Some searching told me that ~90 is the average line length
    const int BUFFER_SIZE = 128;
    byte buffer[BUFFER_SIZE] = {};

    Array<byte> largeBuffer(Allocator::Temporary);
    bool stopAndAppend = false;

    int read = 0;
    do {
        u64 endPosition = Position();

        // -1 to reserve the last spot for a null-terminator when we append
        read = ReadBytes(buffer, BUFFER_SIZE - 1);
        buffer[read] = '\0';

        // Search for newline characters
        byte* newline = (byte*)strpbrk((char*)buffer, "\r\n");

        SeekOrigin origin = SeekOrigin::Start;

        if (newline) {
            // We found a newline! Turn it into a null-terminator and add it
            *newline = '\0';
            stopAndAppend = true;

            endPosition += (newline - buffer) + 1;

            // Check if the next one is a newline in case of CRLF
            if (newline[1] == '\n') {
                endPosition += 1;
            }

        } else {
            // No newline, search for a null-terminator
            byte* null = (byte*)memchr(buffer, '\0', read);

            if (null) {
                stopAndAppend = true;
                endPosition += (null - buffer) + 1;
            } else if (read < BUFFER_SIZE - 1) {
                // We did not find any line ends and we didn't read the full buffer.
                // Most likely we're at the end of the stream, pack it up boys
                stopAndAppend = true;
                endPosition = 0;
                origin = SeekOrigin::End;
            }
        }

        // If we didn't find a line end we append the buffer to the large buffer
        // because the line is more than BUFFER_SIZE bytes long
        if (stopAndAppend) {
            if (largeBuffer.Count() == 0) {
                out.Append(buffer);

                // Seek backwards in case we overshot
                Seek(endPosition, origin);
                return;
            } else {
                largeBuffer.AddRange(buffer, read + 1);
                out.Append(largeBuffer.Data());

                // Seek backwards in case we overshot
                Seek(endPosition, origin);
                return;
            }
        }

        largeBuffer.AddRange(buffer, read);
    } while (read > 0);

    // We somehow reached this...
    largeBuffer.Add('\0');
    out.Append(largeBuffer.Data());

#if 0
    while (CanRead()) {
        codepoint point;
        if (ReadCodepoint(&point) == 0) return;

        if (point == L'\r' || point == L'\n' || point == L'\0') {

            if (point == L'\r') {
                // Read the next byte, it will be a single byte if it's an ascii character so the utf8 string will be valid
                byte newline[2];
                if (ReadByte(&newline[0]) == 0) return;

                newline[1] = '\0';

                // If it isn't valid UTF-8, rewind
                // If it is valid UTF-8 and the second character is not a newline, rewind
                if (IsValidUTF8(newline)) {
                    GetNextCodepoint(newline, &point);
                    if (point != L'\n') {
                        Seek(-1);
                    }
                }
            }

            return;
        }

        // Add codepoint to string
        out.Append(point);
    }
#endif
}

void Stream::ReadAllText(String& out, const uchar* newlineSeperator) {
    if (!CanRead()) return;

    while (CanRead()) {
        ReadLine(out);
        if (!CanRead()) return;
        out.Append(newlineSeperator);
    }
    }

void Stream::ReadAllLines(Array<String>& out, Allocator stringAllocator) {
    if (!CanRead()) return;

    while (CanRead()) {
        out.Reserve(1);
        out.Last().ChangeAllocator(stringAllocator);
        ReadLine(out.Last());
    }
}

int Stream::WriteBytes(Slice<byte> bytes) {
    if (!CanWrite()) return 0;

    for (int i = 0; i < bytes.Count(); i++) {
        bool success = WriteByte(bytes[i]) == 1;

        if (!success) return i;
    }

    return bytes.Count();
}

int Stream::WriteText(StringView string) {
    return WriteBytes(string.ToSlice());
}

bool Stream::SkipIfEqual(Slice<byte> bytes) {
    if (!CanRead() && !CanSeek()) return false;

    // For small allocations so we don't have to allocate a buffer
    const int STACK_BUFFER_SIZE = 255;
    byte stackBuffer[STACK_BUFFER_SIZE];

    // Check if we can use the stack buffer or if we need to allocate a temporary one.
    byte* buffer = (bytes.SizeInBytes() > STACK_BUFFER_SIZE) ? (byte*)Alloc(bytes.SizeInBytes(), Allocator::Temporary) : stackBuffer;

    ReadBytes(buffer, bytes.SizeInBytes());

    bool equal = MemoryCompare(bytes.Data(), buffer, bytes.SizeInBytes());

    // Not equal, seek back
    if (!equal) {
        Seek(-((i64)bytes.SizeInBytes()));
    }

    return equal;
}

bool Stream::CanRead() {
    return false;
}

bool Stream::CanWrite() {
    return false;
}

bool Stream::CanSeek() {
    return false;
}

i64 Stream::SizeInBytes() {
    return 0;
}

i64 Stream::Position() {
    return 0;
}

}
