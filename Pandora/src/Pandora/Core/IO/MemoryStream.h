#pragma once

#include "Pandora/Core/IO/Stream.h"

namespace pd {

class MemoryStream : public Stream {
public:
    MemoryStream() = default;
    
    /// <summary>
    /// Creates a new writable stream.
    /// </summary>
    /// <param name="sizeInBytes">The size of the buffer in bytes.</param>
    /// <param name="canGrow">Whether or not the stream can grow if writing at the end of the stream.</param>
    /// <param name="allocator">The allocator.</param>
    MemoryStream(u64 sizeInBytes, bool canGrow = true, pd::Allocator allocator = pd::Allocator::Persistent);

    MemoryStream(Slice<byte> buffer);

    virtual ~MemoryStream();

    /// <summary>
    /// Creates a read-only stream from an existing buffer.
    /// </summary>
    /// <param name="buffer">The buffer.</param>
    void Open(Slice<byte> buffer);
        
    /// <summary>
    /// Reads a file into the current position of the memory stream.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <param name="forceResize">If <c>true</c> it will resize even if <c>CanGrow()</c> is false.</param>
    /// <returns>How many bytes were written.</returns>
    int ReadFile(StringView path, bool forceResize = true);

    /// <summary>
    /// Frees any related memory if we have writing access to it. Gets called on destruction.
    /// </summary>
    void Delete();
    
    /// <summary>
    /// Resizes the buffer if we own it.
    /// </summary>
    /// <param name="newSize">The new size.</param>
    void Resize(u64 newSize);

    /// <summary>
    /// Reads a byte.
    /// </summary>
    /// <param name="out">Where to read the byte into.</param>
    /// <returns>How many bytes were read.</returns>
    virtual int ReadByte(byte* out) override;

    /// <summary>
    /// Reads a sequence of bytes.
    /// </summary>
    /// <param name="data">Where to read the bytes into.</param>
    /// <param name="length">How many bytes to read.</param>
    /// <returns>How many bytes were read.</returns>
    virtual int ReadBytes(byte* data, u64 length) override;

    /// <summary>
    /// Writes a byte.
    /// </summary>
    /// <param name="b">The byte.</param>
    /// <returns>How many bytes were written.</returns>
    virtual int WriteByte(byte b) override;

    /// <summary>
    /// Writes a sequence of bytes.
    /// </summary>
    /// <param name="bytes">The bytes.</param>
    /// <returns>How many bytes were written.</returns>
    virtual int WriteBytes(Slice<byte> bytes) override;
    
    /// <summary>
    /// Flushes the stream. Doesn't do anything for a <c>MemoryStream</c>.
    /// </summary>
    virtual void Flush() override;

    /// <summary>
    /// Seeks to the specified offset relative to the origin.
    /// </summary>
    /// <param name="offset">The offset.</param>
    /// <param name="origin">The origin.</param>
    virtual void Seek(i64 offset, SeekOrigin origin = SeekOrigin::Current) override;
        
    /// <summary>
    /// Dumps the entire stream to a file.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <param name="offset">The offset in bytes.</param>
    /// <param name="count">The count in bytes. -1 for cursor position</param>
    void DumpToFile(StringView path, int offset = 0, int count = -1);

    /// <summary>
    /// </summary>
    /// <returns>The allocator of the stream.</returns>
    virtual pd::Allocator Allocator() const;

    /// <summary>
    /// </summary>
    /// <returns>Whether or not the stream can be read from.</returns>
    virtual bool CanRead() override;

    /// <summary>
    /// </summary>
    /// <returns>Whether or not the stream can be written to.</returns>
    virtual bool CanWrite() override;

    /// <summary>
    /// </summary>
    /// <returns>Whether or not the stream supports seeking.</returns>
    virtual bool CanSeek() override;

    /// <summary>
    /// </summary>
    /// <returns>The current reading/writing position of the stream.</returns>
    virtual i64 Position() override;
    
    /// <summary>
    /// </summary>
    /// <returns>Returns <c>true</c> if the end of the stream is reached.</returns>
    bool EndOfStream() const;
    
    /// <summary>
    /// </summary>
    /// <returns>Returns <c>true</c> if we own the buffer, <c>false</c> if it's based on other memory.</returns>
    bool OwnsBuffer() const;
        
    /// <summary>
    /// Sets whether or not we can automatically grow our buffer.
    /// Only gets set if we own our buffer.
    /// </summary>
    /// <param name="canGrow">Whether or not the stream can grow if writing at the end of the stream.</param>
    void SetGrow(bool canGrow);

    /// <summary>
    /// </summary>
    /// <returns>Whether or not we can automatically grow our buffer if we attempt to write at the end of the stream.</returns>
    bool CanGrow() const;

    /// <summary>
    /// </summary>
    /// <returns>The size of the buffer in bytes.</returns>
    i64 SizeInBytes() override;

    /// <summary>
    /// </summary>
    /// <returns>The the raw data of the stream.</returns>
    byte* Data() const;

    /// <summary>
    /// </summary>
    /// <param name="offset">The offset in bytes.</param>
    /// <param name="count">The count in bytes. -1 for remaining space.</param>
    /// <returns>The buffer of the stream as a slice.</returns>
    Slice<byte> AsSlice(int offset = 0, int count = -1) const;

protected:
    /// <summary>
    /// Checks if we own the buffer and if we're at the end of the stream.
    /// If so we will double our buffer size, or if we have no buffer, allocate one.
    /// </summary>
    /// <param name="newBytes">How many new bytes are going to be added.</param>
    void CheckForGrowth(u64 newBytes);

    pd::Allocator allocator = pd::Allocator::Persistent;
    byte* memory = nullptr;
    u64 bufferSize = 0;

    bool ownsBuffer = true;
    bool canGrow = true;

    i64 position = 0;

};

}
