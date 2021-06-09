#pragma once

#include "Pandora/Core/IO/Stream.h"
#include "Pandora/Core/Logging/Logging.h"

#include <cstdio>

namespace pd {

enum class ConColor : byte {
    Black,
    DarkBlue,
    DarkGreen,
    DarkCyan,
    DarkRed,
    DarkPurple,
    DarkYellow,
    White,
    Grey,
    Blue,
    Green,
    Cyan,
    Red,
    Purple,
    Yellow,
    BrightWhite,
    Count
};

class Console final : public Stream {
public:
    Console(FILE* stream = stdout);

    /**
     * \brief Does nothing.
     * 
     * \param out Ignored.
     * \return Returns 0.
     */
    virtual int ReadByte(byte* out) override;

    /**
     * \brief Does nothing.
     * 
     * \param data Ignored.
     * \param length Ignored.
     * \return Returns 0.
     */
    virtual int ReadBytes(byte* data, u64 length) override;

    /**
     * \brief Writes a byte to the output stream.
     * 
     * \param b The byte to write.
     * \return How many bytes were written.
     */
    virtual int WriteByte(byte b) override;

    /**
     * \brief Writes a sequence of bytes to the output stream.
     * 
     * \param bytes The bytes to write.
     * \return How many bytes were written.
     */
    virtual int WriteBytes(Slice<byte> bytes) override;

    /**
     * \brief Writes a formatted string to `stdout`.
     * 
     * \tparam Args The format arguments.
     * \param fmt The format string.
     * \param args The arguments.
     */
    template<typename... Args>
    void Log(StringView fmt, const Args&... args) {
        pd::Log(*this, fmt, args...);
    }

    /**
     * \brief Enables or disables color output.
     * 
     * \param isEnabled Whether to enable or disable it.
     */
    void SetColorEnabled(bool isEnabled);

    /**
     * \return Whether or not color is enabled.
     */
    bool IsColorEnabled() const;

    /**
     * \brief Sets the foreground color.
     * 
     * \param color The color. 
     */
    void SetColor(ConColor color);

    /**
     * \brief Sets the console's cursor to the specified location.
     * 
     * \param x The x position/column.
     * \param y The y position/row.
     */
    void SetCursor(int x, int y);

    /**
     * \brief Sets the title of the console window.
     * 
     * \param title The title.
     */
    void SetTitle(StringView title);

    /**
     * \brief Flushes the output stream.
     */
    virtual void Flush() override;

    /**
     * \brief Does nothing.
     * 
     * \param offset Ignored.
     * \param origin Ignored.
     */
    virtual void Seek(i64 offset, SeekOrigin origin = SeekOrigin::Current) override;

    virtual bool CanRead() override;
    virtual bool CanWrite() override;
    virtual bool CanSeek() override;
    virtual i64 SizeInBytes() override;
    virtual i64 Position() override;

private:
    FILE* stream;
    bool colorsEnabled = true;
};

extern Console console;

#if defined(PD_DEBUG)
#define CONSOLE_LOG_DEBUG(fmt, ...) console.Log(fmt, __VA_ARGS__);
#else
#define CONSOLE_LOG_DEBUG(fmt, ...)
#endif

template<>
inline void PrintType(ConColor& type, FormatInfo& info) {
    // Only change color if we're the console
    if (&info.output == &console) {
        if (!console.IsColorEnabled()) return;
        console.SetColor(type);
    }
}

}
