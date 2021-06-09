#pragma once

#include "Pandora/Core/IO/Stream.h"
#include "Pandora/Core/Data/String.h"

namespace pd {

/**
 * \brief `Read` opens the file for reading. File must exists.
 * `Write` opens the file for writing. Will overwrite any existing content.
 * `ReadWrite` opens the file for reading and writing. File must exist.
 * `Truncate` opens the file for reading and writing. Will overwrite any existing content.
 * `Append` opens the file for writing at the end of the file. Seeking is not supported.
 */
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

    /**
     * \brief Opens the file at the specified path.
     * 
     * \param path The path.
     * \param mode The mode.
     */
    FileStream(StringView path, FileMode mode = FileMode::ReadWrite);

    virtual ~FileStream();

    /**
     * \brief Opens the file at the specified path.
     * 
     * \param path The path.
     * \param mode The mode.
     * \return Whether or not the file opened successfully.
     */
    bool Open(StringView path, FileMode mode = FileMode::ReadWrite);

    /**
     * \brief Closes the file.
     * Gets called on destruction.
     */
    void Close();

    /**
     * \brief Reads a byte.
     * Mode must support reading. 
     * 
     * \param out Where to read the byte into. 
     * \return How many bytes were read.
     */
    virtual int ReadByte(byte* out) override;

    /**
     * \brief Reads a sequence of bytes.
     * Mode must support reading.
     * 
     * \param data Where to read the bytes into.
     * \param length How many bytes to read.
     * \return How many bytes were read.
     */
    virtual int ReadBytes(byte* data, u64 length) override;

    /**
     * \brief Writes a byte. Mode must support writing.
     * 
     * \param b The byte to write.
     * \return How many bytes were read.
     */
    virtual int WriteByte(byte b) override;

    /**
     * \brief Writes a sequence of bytes.
     * Mode must support writing.
     * 
     * \param bytes The bytes to write.
     * \return How many bytes were written.
     */
    virtual int WriteBytes(Slice<byte> bytes) override;

    /**
     * \brief Writes printf-formatted UTF-8 to the file.
     * Mode must support writing.
     * 
     * \param fmt The printf format string.
     * \param ... The arguments.
     * \return How many bytes were written.
     */
    int WriteFormatF(const uchar* fmt, ...);

    /**
     * \brief Writes formatted UTF-8 to the file.
     * Mode must support writing.
     * 
     * \tparam Args The log arguments.
     * \param fmt The format string/
     * \param args The arguments.
     */
    template<typename... Args>
    void WriteFormat(StringView fmt, const Args &... args) {
        pd::Log(*this, fmt, args...);
    }

    /**
     * \brief Writesthe byte-order mark at the current position.
     * Mode must support writing.
     * 
     * \return How many bytes were written.
     */
    int WriteBOM();

    /**
     * \brief Flushes the file stream.
     */
    virtual void Flush() override;

    /**
     * \brief Seeks to the specified offset relative to the origin.
     * Mode must support seeking.
     * 
     * \param offset The relative offset.
     * \param origin The origin.
     */
    virtual void Seek(i64 offset, SeekOrigin origin = SeekOrigin::Current) override;

    /**
     * \return Whether or not the file and mode supports reading.
     * Note that this will return false if the EOF is reached.
     */
    virtual bool CanRead() override;

    /**
     * \return Whether or not the file and mode supports writing.
     */
    virtual bool CanWrite() override;

    /**
     * \return Whether or not the file and mode supports seeking.
     */
    virtual bool CanSeek() override;

    /**
     * \return Whether or not the end-of-file has been reached.
     */
    bool EndOfFile();

    /**
     * \return Whether or not the file is open.
     */
    virtual bool IsOpen();

    /**
     * \return How many bytes long the file is.
     */
    virtual i64 SizeInBytes() override;

    /**
     * \return The current position of the file cursor. of the file cursor.
     */
    virtual i64 Position() override;


private:
    FileMode mode = FileMode::Read;
    FILE* file = nullptr;

    bool endOfFile = true;
};

}
