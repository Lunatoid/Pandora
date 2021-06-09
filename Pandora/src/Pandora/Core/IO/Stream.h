#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Data/Slice.h"
#include "Pandora/Core/Data/StringView.h"

namespace pd {

enum class SeekOrigin : byte {
    Start,
    Current,
    End,
};

class Stream {
public:
    virtual ~Stream() = default;

    /**
     * \brief Reads a byte.
     * 
     * \param out Where to read the byte into.
     * \return How many bytes were read.
     */
    virtual int ReadByte(byte* out) = 0;

    /**
     * \brief Reads a sequence of bytes.
     * 
     * \param data Where to read the bytes into.
     * \param length How many bytes to read.
     * \return How many bytes were read.
     */
    virtual int ReadBytes(byte* data, u64 length);

    /**
     * \brief Reads a UTF-8 codepoint.
     * 
     * \param out Where to read the codepoint into.
     * \return How many bytes were read, in this case the size of the codepoint.
     */
    virtual int ReadCodepoint(codepoint* out);

    /**
     * \brief Reads a codepoint and then seeks back.
     * 
     * \param out Where to read the codepoint into.
     * \return How many bytes were peeked.
     */
    virtual int PeekCodepoint(codepoint* out);

    /**
     * \brief Checks if the byte-order mark is present at the currnet position and
     * seeks past it if it is.
     * 
     * \return Whether or not the BOM was skipped.
     */
    virtual bool SkipBOMIfPresent();

    /**
     * \brief Reads text until a newline or null-terminator is reached.
     * 
     * \param out The output string.
     */
    void ReadLine(String& out);

    /**
     * \brief Reads all the lines into a string with a specified newline seperator.
     * 
     * \param out The output string.
     * \param newlineSeperator The newline seperator.
     */
    void ReadAllText(String& out, const uchar* newlineSeperator = u8"\n");

    /**
     * \brief Reads all the lines into an array of strings.
     * 
     * \param out The output array.
     * \param stringAllocator The allocator to use for each string.
     */
    void ReadAllLines(Array<String>& out, Allocator stringAllocator = Allocator::Persistent);

    /**
     * \brief Reads sizeof(T) bytes as T.
     * 
     * \tparam T The type to read.
     * \param out The output T.
     * \return How many bytes were read.
     */
    template<typename T>
    int Read(T* out) {
        return ReadBytes((byte*)out, sizeof(T));
    }

    /**
     * \brief Reads sizeof(T) bytes as T, then seeks back.
     * 
     * \tparam T The type to read.
     * \param out The output T.
     * \return How many bytes were peeked.
     */
    template<typename T>
    int Peek(T* out) {
        int read = Read(out);
        Seek(-read);
        return read;
    }

    /**
     * \brief Writes a byte.
     * 
     * \param b The byte to write.
     * \return How many bytes were written.
     */
    virtual int WriteByte(byte b) = 0;

    /**
     * \brief Writes a sequence of bytes.
     * 
     * \param bytes The bytes to write.
     * \return How many bytes were written.
     */
    virtual int WriteBytes(Slice<byte> bytes);

    /**
     * \brief Writes UTF-8 text to the file, excluding the null-terminator.
     * 
     * \param text The UTF-8 encoded string.
     * \return How many bytes were written.
     */
    int WriteText(StringView text);

    /**
     * \brief Writes sizeof(T) bytes of t.
     * 
     * \tparam T The type to write.
     * \param t The variable to write.
     * \return How many bytes were written.
     */
    template<typename T>
    int Write(const T& t) {
        return WriteBytes(ToBytes(&t));
    }
 
    /**
     * \brief Flushes the stream.
     */
    virtual void Flush() = 0;
 
    /**
     * \brief Seeks to the specified offset relative to the origin.
     * 
     * \param offset The relative offset.
     * \param origin The origin.
     */
    virtual void Seek(i64 offset, SeekOrigin origin = SeekOrigin::Current) = 0;

    /**
     * \brief Compares the bytes at the current position with `bytes`.
     * If they are not equal, it will seek back.
     * 
     * \param bytes The bytes to compare.
     * \return Whether or not it was equal.
     */
    virtual bool SkipIfEqual(Slice<byte> bytes);

    /**
     * \return Whether or not the stream can currently be read from.
     */
    virtual bool CanRead();

    /**
     * \return Whether or not the stream can currently be written to.
     */
    virtual bool CanWrite();

    /**
     * \return Whether or not the stream can currently be seeked.
     */
    virtual bool CanSeek();

    /**
     * \return The size of the stream in bytes.
     */
    virtual i64 SizeInBytes();

    /**
     * \return The current stream cursor.
     */
    virtual i64 Position();
};

}
