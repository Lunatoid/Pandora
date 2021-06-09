#pragma once

#include "Pandora/Core/IO/Stream.h"

namespace pd {

class MemoryStream : public Stream {
public:
    MemoryStream() = default;

    /**
     * \brief Creates a new writable stream.
     * 
     * \param sizeInBytes The size of the buffer in bytes.
     * \param canGrow Whether or not the buffer can grow when writing past the end of the buffer.
     * \param allocator The allocator used for the buffer.
     */
    MemoryStream(u64 sizeInBytes, bool canGrow = true, pd::Allocator allocator = pd::Allocator::Persistent);

    /**
     * \brief Creates a new read-only stream based on an existing buffer.
     * 
     * \param buffer The existing buffer to create a view for.
     */
    MemoryStream(Slice<byte> buffer);

    virtual ~MemoryStream();

    /**
     * \brief Creates a read-only stream based on an existing buffer.
     * 
     * \param buffer The existing buffer to create a view for.
     */
    void Open(Slice<byte> buffer);

    /**
     * \brief Reads a file into the current position of the memory stream.
     * Note that we must own the buffer.
     * 
     * \param path The path to the file.
     * \param forceResize If true it will resize, even if `canGrow` is false.
     * \return How many bytes were written.
     */
    int ReadFile(StringView path, bool forceResize = true);

    /**
     * \brief Frees the buffer.
     * Note that we must own the buffer.
     */
    void Delete();

    /**
     * \brief Resizes the buffer.
     * Note that we must own the buffer.
     * 
     * \param newSize The new buffer size in bytes.
     */
    void Resize(u64 newSize);

    /**
     * \brief Reads a byte.
     * 
     * \param out Where to read the byte into.
     * \return How many bytes were read.
     */
    virtual int ReadByte(byte* out) override;

    /**
     * \brief Reads a sequence of bytes.
     * 
     * \param data Where to read the bytes into.
     * \param length How many bytes to read.
     * \return How many bytes were read.
     */
    virtual int ReadBytes(byte* data, u64 length) override;

    /**
     * \brief Writes a byte.
     * 
     * \param b The byte to write.
     * \return How many bytes were written.
     */
    virtual int WriteByte(byte b) override;

    /**
     * \brief Writes a sequence of bytes.
     * 
     * \param bytes The bytes to write.
     * \return How many bytes were written.
     */
    virtual int WriteBytes(Slice<byte> bytes) override;

    /**
     * \brief Does nothing.
     */
    virtual void Flush() override;

    /**
     * \brief Seeks to the specified offset relative to the origin.
     * 
     * \param offset The relative offset.
     * \param origin The origin.
     */
    virtual void Seek(i64 offset, SeekOrigin origin = SeekOrigin::Current) override;

    /**
     * \brief Dumps the entire buffer to a file.
     * 
     * \param path The path of the file.
     * \param offset The offset in bytes.
     * \param count The count in bytes. Pass -1 for the cursor position.
     */
    void DumpToFile(StringView path, int offset = 0, int count = -1);

    /**
     * \return The allocator of the stream.
     */
    virtual pd::Allocator Allocator() const;

    /**
     * \return True.
     */
    virtual bool CanRead() override;

    /**
     * \return Whether or not we own the buffer.
     */
    virtual bool CanWrite() override;

    /**
     * \return True.
     */
    virtual bool CanSeek() override;

    /**
     * \return The current cursor of the stream.
     */
    virtual i64 Position() override;

    /**
     * \return Will return true if the position is past the end of the buffer.
     */
    bool EndOfStream() const;

    /**
     * \return Whether or not we own the buffer.
     */
    bool OwnsBuffer() const;

    /**
     * \brief Sets whether or not the buffer is allowed to automatically grow.
     * Can only be set if we own the buffer.
     * 
     * \param canGrow Whether or not it is allowed to grow.
     */
    void SetGrow(bool canGrow);

    /**
     * \return Whether or not the buffer can automatically grow.
     */
    bool CanGrow() const;

    /**
     * \return The size of the buffer in bytes.
     */
    i64 SizeInBytes() override;

    /**
     * \return The buffer pointer of the stream.
     */
    byte* Data() const;

    /**
     * \brief Returns the buffer as a slice.
     * 
     * \param offset The offset in bytes from the start.
     * \param count The count in bytes. Pass -1 for the remaining space.
     * \return The slice.
     */
    Slice<byte> AsSlice(int offset = 0, int count = -1) const;

protected:

    /**
     * \brief Checks if we own the buffer and if we're at the end of the stream.
     * If we do, it will double the buffer, or allocate one if we don't have any.
     * 
     * \param newBytes How many bytes are going to be written.
     */
    void CheckForGrowth(u64 newBytes);

    pd::Allocator allocator = pd::Allocator::Persistent;
    byte* memory = nullptr;
    u64 bufferSize = 0;

    bool ownsBuffer = true;
    bool canGrow = true;

    i64 position = 0;

};

}
