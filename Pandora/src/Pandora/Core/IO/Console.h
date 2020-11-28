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

    /// <summary>
    /// Does nothing.
    /// </summary>
    /// <param name="out"></param>
    /// <returns></returns>
    virtual int ReadByte(byte* out) override;

    /// <summary>
    /// Does nothing.
    /// </summary>
    /// <param name="data"></param>
    /// <param name="length"></param>
    /// <returns></returns>
    virtual int ReadBytes(byte* data, u64 length) override;

    /// <summary>
    /// Writes a byte to the output stream.
    /// </summary>
    /// <param name="b">The byte.</param>
    /// <returns>How many bytes were written.</returns>
    virtual int WriteByte(byte b) override;

    /// <summary>
    /// Writes a sequence of bytes to the output stream.
    /// </summary>
    /// <param name="bytes">The bytes.</param>
    /// <returns>How many bytes were written.</returns>
    virtual int WriteBytes(Slice<byte> bytes) override;

    /// <summary>
    /// Logs a formatted stream to <c>stdout</c>.
    /// </summary>
    /// <param name="fmt">The base string.</param>
    /// <param name="...args">The arguments.</param>
    template<typename... Args>
    void Log(StringView fmt, const Args&... args) {
        pd::Log(*this, fmt, args...);
    }
    
    /// <summary>
    /// Enables or disables setting colors through the <c>SetColor</c> and <c>SetBackColor</c> functions.
    /// </summary>
    /// <param name="isEnabled">Whether or not it should be enabled.</param>
    void SetColorEnabled(bool isEnabled);
    
    /// <summary>
    /// Returns whether or not colors are enabled.
    /// </summary>
    /// <returns>Whether or not colors are enabled.</returns>
    bool IsColorEnabled() const;

    /// <summary>
    /// Sets the color of the console text.
    /// </summary>
    /// <param name="color">The color.</param>
    void SetColor(ConColor color);

    /// <summary>
    /// Sets the color of the console text.
    /// </summary>
    /// <param name="color">The color.</param>
    void SetBackColor(ConColor color);

    /// <summary>
    /// Sets the console cursor to the specified position.
    /// </summary>
    /// <param name="x">The X coordinate in columns.</param>
    /// <param name="y">The Y coordinate in rows.</param>
    void SetCursor(int x, int y);

    /// <summary>
    /// Sets the title of the console.
    /// </summary>
    /// <param name="title">The title.</param>
    void SetTitle(StringView title);

    /// <summary>
    /// Gets the title of the console.
    /// </summary>
    /// <param name="out">The title destination.</param>
    void GetTitle(String* out);
    
    /// <summary>
    /// Shows or hides the console.
    /// </summary>
    /// <param name="isVisible">Whether or not it should be visible.</param>
    void SetVisible(bool isVisible);

    /// <summary>
    /// Flushes the output stream.
    /// </summary>
    virtual void Flush() override;

    /// <summary>
    /// Does nothing.
    /// </summary>
    /// <param name="offset"></param>
    /// <param name="origin"></param>
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

        if (info.pretty) {
            console.SetBackColor(type);
        } else {
            console.SetColor(type);
        }
    }
}

}
