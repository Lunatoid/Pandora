#include "MemoryStream.h"

#include "Pandora/Core/IO/File.h"
#include "Pandora/Core/IO/FileStream.h"
#include "Pandora/Core/Math/Math.h"

namespace pd {

MemoryStream::MemoryStream(u64 sizeInBytes, bool canGrow, pd::Allocator allocator) {
    ownsBuffer = true;
    SetGrow(canGrow);
    this->allocator = allocator;

    Resize(sizeInBytes);
}

MemoryStream::MemoryStream(Slice<byte> buffer) {
    Open(buffer);
}

MemoryStream::~MemoryStream() {
    Delete();
}

void MemoryStream::Open(Slice<byte> buffer) {
    Delete();

    ownsBuffer = false;
    canGrow = false;
    allocator = Allocator::None;

    memory = buffer.Data();
    bufferSize = buffer.SizeInBytes();
}

int MemoryStream::ReadFile(StringView path, bool forceResize) {
    if (!CanWrite()) return 0;

    FileStream file(path, FileMode::Read);

    if (!file.IsOpen()) return 0;

    if (forceResize || CanGrow()) {
        CheckForGrowth(file.SizeInBytes());
    }

    i64 readSize = Min((i64)bufferSize - position, file.SizeInBytes());

    int bytesRead = file.ReadBytes(memory + position, readSize);
    position += (u64)bytesRead;

    return bytesRead;
}

void MemoryStream::Delete() {
    if (ownsBuffer && memory) {
        Free(memory, allocator);
        memory = nullptr;
    }

    bufferSize = 0;
    position = 0;
}

void MemoryStream::Resize(u64 newSize) {
    if (!OwnsBuffer()) return;

    bufferSize = newSize;
    memory = (byte*)Realloc(memory, newSize, allocator);
}

int MemoryStream::ReadByte(byte* out) {
    if (!CanRead() || EndOfStream()) return 0;

    *out = memory[position];
    position++;

    return 1;
}

int MemoryStream::ReadBytes(byte* data, u64 length) {
    if (!CanRead() || EndOfStream()) return 0;

    u64 copySize = Min(length, bufferSize - position);
    MemoryCopy(data, memory + position, copySize);

    position += copySize;

    return (int)copySize;
}

int MemoryStream::WriteByte(byte b) {
    if (!CanWrite()) return 0;

    if (CanGrow()) {
        CheckForGrowth(1);
    }

    memory[position] = b;
    position++;

    return 1;
}

int MemoryStream::WriteBytes(Slice<byte> bytes) {
    if (!CanWrite()) return 0;

    if (CanGrow()) {
        CheckForGrowth(bytes.SizeInBytes());
    }

    u64 copySize = Min(bytes.SizeInBytes(), bufferSize - position);

    MemoryCopy(memory + position, bytes.Data(), copySize);
    position += bytes.SizeInBytes();

    return (int)copySize;
}

void MemoryStream::Flush() {
    // No operation
}

void MemoryStream::Seek(i64 offset, SeekOrigin origin) {
    switch (origin) {
        case SeekOrigin::Start:
            position = offset;
            break;

        case SeekOrigin::Current:
            position += offset;
            break;

        case SeekOrigin::End:
            position = bufferSize + offset;
            break;
    }
}

void MemoryStream::DumpToFile(StringView path, int offset, int count) {
    if (count <= 0) {
        count = (int)position;
    }

    WriteEntireFile(path, AsSlice(offset, count));
}

pd::Allocator MemoryStream::Allocator() const {
    return allocator;
}

bool MemoryStream::CanRead() {
    return true;
}

bool MemoryStream::CanWrite() {
    return ownsBuffer;
}

bool MemoryStream::CanSeek() {
    return true;
}

i64 MemoryStream::Position() {
    return position;
}

bool MemoryStream::EndOfStream() const {
    return position >= (i64)bufferSize;
}

bool MemoryStream::OwnsBuffer() const {
    return ownsBuffer;
}

void MemoryStream::SetGrow(bool canGrow) {
    if (ownsBuffer) {
        this->canGrow = canGrow;
    }
}

bool MemoryStream::CanGrow() const {
    return canGrow;
}

i64 MemoryStream::SizeInBytes() {
    return (i64)bufferSize;
}

byte* MemoryStream::Data() const {
    return memory;
}

Slice<byte> MemoryStream::AsSlice(int offset, int count) const {
    if (count <= 0) {
        count = (int)bufferSize - offset;
    }

    return Slice<byte>(memory + offset, count);
}

void MemoryStream::CheckForGrowth(u64 newBytes) {
    // Should this be here?
    const u64 INITIAL_SIZE = 32;

    if (OwnsBuffer() && position + newBytes > bufferSize) {
        // Set the position to the end of the position if we're past it
        if (position >= (i64)bufferSize) {
            position = (i64)bufferSize;
        }

        if (bufferSize == 0) {
            bufferSize = INITIAL_SIZE;
        }

        while (bufferSize < position + newBytes) {
            bufferSize *= 2;
        }

        Resize(bufferSize);
    }
}

}
