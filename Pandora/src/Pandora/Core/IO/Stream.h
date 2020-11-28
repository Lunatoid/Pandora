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
    
    /// <summary>
    /// Reads a byte.
    /// </summary>
    /// <param name="out">Where to read the byte into.</param>
    /// <returns>How many bytes were read.</returns>
    virtual int ReadByte(byte* out) = 0;
    
    /// <summary>
    /// Reads a sequence of bytes.
    /// </summary>
    /// <param name="data">Where to read the bytes into.</param>
    /// <param name="length">How many bytes to read.</param>
    /// <returns>How many bytes were read.</returns>
    virtual int ReadBytes(byte* data, u64 length);
    
    /// <summary>
    /// Reads a UTF-8 codepoint.
    /// </summary>
    /// <param name="out">Where to read the codepoint into.</param>
    /// <returns>How many bytes we're read, thus the size of the codepoint.</returns>
    virtual int ReadCodepoint(codepoint* out);
    
    /// <summary>
    /// Reads a codepoint and then seeks back.
    /// </summary>
    /// <param name="out">The out.</param>
    /// <returns>How many bytes we're peeked.</returns>
    virtual int PeekCodepoint(codepoint* out);

    /// <summary>
    /// Skips the byte order mark if present at the current position.
    /// </summary>
    /// <returns>Whether or not it was skipped.</returns>
    virtual bool SkipBOMIfPresent();

    /// <summary>
    /// Reads text until the next newline or null-terminator.
    /// </summary>
    /// <param name="out">The target string.</param>
    void ReadLine(String& out);

    /// <summary>
    /// Reads all lines into a string with a specified newline seperator.
    /// </summary>
    /// <param name="out">The target string.</param>
    /// <param name="newlineSeperator">The string that seperates every line.</param>
    void ReadAllText(String& out, const uchar* newlineSeperator = u8"\n");

    /// <summary>
    /// Reads all lines into an array of strings.
    /// </summary>
    /// <param name="out">The target array.</param>
    /// <param name="stringAllocator">The allocator to use for each string.</param>
    void ReadAllLines(Array<String>& out, Allocator stringAllocator = Allocator::Persistent);

    /// <summary>
    /// Reads <c>sizeof(T)</c> bytes as a <c>T</c>.
    /// </summary>
    /// <param name="out">The target.</param>
    /// <returns>How many bytes were read.</returns>
    template<typename T>
    int Read(T* out) {
        return ReadBytes((byte*)out, sizeof(T));
    }

    /// <summary>
    /// Reads <c>sizeof(T)</c> bytes as a <c>T</c> and then seeks back.
    /// </summary>
    /// <param name="out">The target.</param>
    /// <returns>How many bytes were peeked.</returns>
    template<typename T>
    int Peek(T* out) {
        int read = Read(out);
        Seek(-read);
        return read;
    }
    
    /// <summary>
    /// Writes a byte.
    /// </summary>
    /// <param name="b">The byte.</param>
    /// <returns>How many bytes were written.</returns>
    virtual int WriteByte(byte b) = 0;
    
    /// <summary>
    /// Writes a sequence of bytes.
    /// </summary>
    /// <param name="bytes">The bytes.</param>
    /// <returns>How many bytes were written.</returns>
    virtual int WriteBytes(Slice<byte> bytes);

    /// <summary>
    /// Writes UTF-8 text to the file, excluding the null-terminator.
    /// </summary>
    /// <param name="string">The UTF-8 encoded string.</param>
    /// <returns>How many bytes were written.</returns>
    int WriteText(StringView text);

    /// <summary>
    /// Writes <c>sizeof(T)</c> bytes of <c>t</c>.
    /// </summary>
    /// <param name="t">The variable.</param>
    /// <returns>How many bytes were written.</returns>
    template<typename T>
    int Write(const T& t) {
        return WriteBytes(ToBytes(&t));
    }
    
    /// <summary>
    /// Flushes the stream.
    /// </summary>
    virtual void Flush() = 0;
    
    /// <summary>
    /// Seeks to the specified offset relative to the origin.
    /// </summary>
    /// <param name="offset">The offset.</param>
    /// <param name="origin">The origin.</param>
    virtual void Seek(i64 offset, SeekOrigin origin = SeekOrigin::Current) = 0;
        
    /// <summary>
    /// Compares the bytes at the current position with <c>bytes</c>.
    /// If they're not equal it will seek back.
    /// </summary>
    /// <param name="bytes">The bytes.</param>
    /// <returns>Whether or not it was equal.</returns>
    virtual bool SkipIfEqual(Slice<byte> bytes);

    /// <summary>
    /// Returns whether or not the stream can currently be read from.
    /// </summary>
    /// <returns>Whether or not the stream can currently be read from.</returns>
    virtual bool CanRead();

    /// <summary>
    /// Returns whether or not the stream can currently be written to.
    /// </summary>
    /// <returns>Whether or not the stream can currently be written to.</returns>
    virtual bool CanWrite();

    /// <summary>
    /// Returns whether or not the can currently be seeked.
    /// </summary>
    /// <returns>Whether or not the can currently be seeked.</returns>
    virtual bool CanSeek();

    /// <summary>
    /// Returns the size of the stream in bytes.
    /// </summary>
    /// <returns>The size of the stream in bytes.</returns>
    virtual i64 SizeInBytes();

    /// <summary>
    /// Returns the current reading/writing position of the stream.
    /// </summary>
    /// <returns>The current reading/writing position of the stream.</returns>
    virtual i64 Position();
};

}
