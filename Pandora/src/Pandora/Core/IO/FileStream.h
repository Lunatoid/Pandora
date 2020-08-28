#pragma once

#include "Pandora/Core/IO/Stream.h"
#include "Pandora/Core/Data/String.h"

#include <cstdio>

namespace pd {

/// <summary>
/// <c>Read</c> opens the file for reading. File must exist.
/// <c>Write</c> opens the file for writing. Any existing contents will be discarded.
/// <c>ReadWrite</c> opens the file for reading and writing. File must exist.
/// <c>Truncate</c> opens the file for reading and writing. Any existing contents will be discarded.
/// <c>Append</c> opens the file for writing at the end of the file. Seeking is not supported.
/// </summary>
enum class FileMode : byte {
    Read,
    Write,
    ReadWrite,
    Truncate,
    Append
};

class FileStream final : public Stream {
public:
    FileStream() = default;

    /// <summary>
    /// Opens the file at the specified path.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <param name="mode">The mode.</param>
    FileStream(StringView path, FileMode mode = FileMode::ReadWrite);

    virtual ~FileStream();
    
    /// <summary>
    /// Opens the file at the specified path.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <param name="mode">The mode.</param>
    /// <returns>Whether or not it opened successfully.</returns>
    bool Open(StringView path, FileMode mode = FileMode::ReadWrite);
    
    /// <summary>
    /// Closes the file. This is called on destruction.
    /// </summary>
    void Close();
        
    /// <summary>
    /// Reads a byte.
    /// </summary>
    /// <param name="out">Where to read the byte into.</param>
    /// <returns>How many bytes were read.</returns>
    virtual int ReadByte(byte* out) override;

    /// <summary>
    /// Reads a sequence of bytes.
    /// </summary>
    /// <param name="out">Where to read the byte into.</param>
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
    /// Writes formatted UTF-8 text to the file.
    /// </summary>
    /// <param name="fmt">The printf base string.</param>
    /// <param name="">The arguments.</param>
    /// <returns>How many bytes were written.</returns>
    int WriteFormatF(const uchar* fmt, ...);

    /// <summary>
    /// Writes formatted UTF-8 text to the file.
    /// </summary>
    /// <param name="fmt">The base string.</param>
    /// <param name="">The arguments.</param>
    /// <returns>How many bytes were written.</returns>
    template<typename... Args>
    void WriteFormat(StringView fmt, const Args &... args) {
        pd::Log(this, fmt, args...);
    }

    /// <summary>
    /// Writes the byte order mark at the current position.
    /// </summary>
    /// <returns>How many bytes were written.</returns>
    int WriteBOM();
    
    /// <summary>
    /// Flushes the file memory.
    /// </summary>
    virtual void Flush() override;
    
    /// <summary>
    /// Seeks to the specified offset relative to the origin.
    /// </summary>
    /// <param name="offset">The offset.</param>
    /// <param name="origin">The origin.</param>
    virtual void Seek(i64 offset, SeekOrigin origin = SeekOrigin::Current) override;
    
    /// <summary>
    /// Returns <c>true</c> if the file can be read from.
    /// This will return <c>false</c> if end-of-file is reached, even if the filemode includes reading.
    /// </summary>
    /// <returns><c>true</c> if the file can be read from.</returns>
    virtual bool CanRead() override;

    /// <summary>
    /// Returns <c>true</c> if the file can be written to.
    /// </summary>
    /// <returns><c>true</c> if the file can be written to.</returns>
    virtual bool CanWrite() override;

    /// <summary>
    /// Returns <c>true</c> if the file supports seeking.
    /// </summary>
    /// <returns>Returns <c>true</c> if the file supports seeking.</returns>
    virtual bool CanSeek() override;
    
    /// <summary>
    /// Returns <c>true</c> if the end of the file is reached.
    /// </summary>
    /// <returns><c>true</c> if the end of the file is reached.</returns>
    bool EndOfFile();
    
    /// <summary>
    /// Returns <c>true</c> if the file is opened.
    /// </summary>
    /// <returns><c>true</c> if the file is opened.</returns>
    virtual bool IsOpen();
    
    /// <summary>
    /// Returns how many bytes long the file is.
    /// </summary>
    /// <returns>How many bytes long the file is.</returns>
    virtual i64 SizeInBytes() override;
    
    /// <summary>
    /// Returns the position of the file cursor.
    /// </summary>
    /// <returns>The position of the file cursor.</returns>
    virtual i64 Position() override;

private:
    FileMode mode = FileMode::Read;
    FILE* file = nullptr;

    bool endOfFile = true;
};

}
