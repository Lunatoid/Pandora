#pragma once

#include "Pandora/Core/Math/Math.h"
#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/IO/MemoryStream.h"
#include "Pandora/Core/Logging/Logging.h"

namespace pd {

class String {
public:
    String(pd::Allocator allocator = Allocator::Persistent);
    String(const uchar* string, pd::Allocator allocator = Allocator::Persistent);

    virtual ~String();

    /// <summary>
    /// Frees the memory associated with the string. This is called on destruction.
    /// </summary>
    void Delete();
    
    /// <summary>
    /// Sets the string to the specified UTF-8 text.
    /// </summary>
    /// <param name="text">The UTF-8 text.</param>
    void Set(const uchar* text);

    /// <summary>
    /// Sets the string to the view.
    /// </summary>
    /// <param name="text">The view.</param>
    void Set(StringView text);

    /// <summary>
    /// Formats the string using a printf string.
    /// </summary>
    /// <param name="fmt">The base string.</param>
    void FormatF(const uchar* fmt, ...);
    
    /// <summary>
    /// Formats the string using the logger.
    /// </summary>
    /// <param name="fmt">The base.</param>
    template<typename... Args>
    void Format(StringView fmt, const Args&... args) {
        MemoryStream stream(32, true, pd::Allocator::Temporary);
        pd::Log(stream, fmt, args...);
        stream.WriteByte('\0');
        Set(stream.Data());
    }

    /// <summary>
    /// Appends the UTF-8 string.
    /// </summary>
    /// <param name="text">The UTF-8 text.</param>
    void Append(const uchar* text);

    /// <summary>
    /// Appends the view.
    /// </summary>
    /// <param name="text">The view.</param>
    void Append(StringView text);

    /// <summary>
    /// Appends the Unicode codepoint.
    /// </summary>
    /// <param name="text">The text.</param>
    void Append(codepoint text);
    
    /// <summary>
    /// Sets <c>out</c> to the specified substring.
    /// </summary>
    /// <param name="out">The output string.</param>
    /// <param name="index">The starting character.</param>
    /// <param name="count">How many codepoints the substring should be. <c>-1</c> for the remainder.</param>
    void Substr(String* out, int index, int count = -1);
    
    /// <summary>
    /// Removes all codepoints contained in <c>chars</c> from the string.
    /// </summary>
    /// <param name="chars">The UTF-8 characters to remove.</param>
    /// <returns>How many codepoints were purged.</returns>
    int PurgeChars(const uchar* chars);
    
    /// <summary>
    /// Removes all codepoints that equal <c>toTrim</c> from the front.
    /// </summary>
    /// <param name="toTrim">The codepoint to trim.</param>
    void TrimFront(codepoint toTrim);

    /// <summary>
    /// Removes all codepoints that equal <c>toTrim</c> from the back.
    /// </summary>
    /// <param name="toTrim">The codepoint to trim.</param>
    void TrimBack(codepoint toTrim);

    /// <summary>
    /// Removes all codepoints that equal <c>toTrim</c> from the front and back.
    /// </summary>
    /// <param name="toTrim">The codepoint to trim.</param>
    void Trim(codepoint toTrim);
    
    /// <summary>
    /// Finds the first codepoint that matches any of the codepoints in <c>chars</c>.
    /// </summary>
    /// <param name="chars">The UTF-8 codepoints to match.</param>
    /// <param name="offset">The offset from which codepoint to Start at.</param>
    /// <returns>The index of the first codepoint that matched.</returns>
    int FindAny(const uchar* chars, int offset = 0);
    
    /// <summary>
    /// Finds the last codepoint that matches any of the codepoints in <c>chars</c>.
    /// </summary>
    /// <param name="chars">The UTF-8 codepoints to match.</param>
    /// <param name="offset">The offset in codepoints relative to the end.</param>
    /// <returns>The index of the last codepoint that matched.</returns>
    int FindAnyReverse(const uchar* chars, int offset = 0);
    
    /// <summary>
    /// Finds the first instance of the UTF-8 substring.
    /// </summary>
    /// <param name="substring">The UTF-8 substring to find.</param>
    /// <param name="offset">The offset in codepoints.</param>
    /// <returns>The index of the first instance that matched.</returns>
    int Find(const uchar* substring, int offset = 0);

    /// <summary>
    /// Finds the last instance of the UTF-8 substring.
    /// </summary>
    /// <param name="substring">The UTF-8 substring to find.</param>
    /// <param name="offset">The offset in codepoints relative to the end.</param>
    /// <returns>The index of the last instance that matched.</returns>
    int FindReverse(const uchar* substring, int offset = 0);
    
    /// <summary>
    /// Inserts the UTF-8 string <c>text</c> at the specified index.
    /// </summary>
    /// <param name="index">The index in codepoints.</param>
    /// <param name="text">The text.</param>
    void Insert(int index, const uchar* text);
    
    /// <summary>
    /// Removes <c>count</c> codepoints starting at <c>index</c>.
    /// </summary>
    /// <param name="index">The index.</param>
    /// <param name="count">The count in codepoints.</param>
    void Remove(int index, int count = 1);
    
    /// <summary>
    /// Finds and replaces the specified UTF-8 strings.
    /// </summary>
    /// <param name="find">The UTF-8 string to find.</param>
    /// <param name="replace">The UTF-8 string to replace.</param>
    /// <returns></returns>
    int Replace(const uchar* find, const uchar* replace);
    
    /// <summary>
    /// Splits the string at the specified seperator into the specified array.
    /// </summary>
    /// <param name="seperator">The UTF-8 seperator.</param>
    /// <param name="out">The output array.</param>
    /// <param name="stringAllocator">The allocator to use for each string.</param>
    void Split(const uchar* seperator, Array<String>* out, pd::Allocator stringAllocator = pd::Allocator::Persistent);
    
    /// <summary>
    /// Converts the string to uppercase.
    /// </summary>
    void ToUpper();
    
    /// <summary>
    /// Converts the string to lowercase.
    /// </summary>
    void ToLower();

#if defined(PD_WINDOWS)
    /// <summary>
    /// Converts the string to a wide string.
    /// </summary>
    /// <param name="allocator">Which allocator to use.</param>
    /// <returns>The wide string.</returns>
    inline wchar* ToWide(pd::Allocator allocator = pd::Allocator::Temporary) {
        return UTF8ToWide(Data(), allocator);
    }
#endif

    /// <summary>
    /// Changes the allocator of the string.
    /// If changed to <c>Allocator::None</c> then <c>Delete()</c> is called.
    /// Otherwise, the existing memory is copied to the new memory and the original memory is freed (if necessary).
    /// </summary>
    /// <param name="allocator">The allocator.</param>
    virtual void ChangeAllocator(pd::Allocator allocator);
    
    /// <summary>
    /// </summary>
    /// <returns>Returns true if the string is valid UTF-8.</returns>
    bool IsValid();
    
    /// <summary>
    /// Returns the codepoint at the specified index. This operation is O(n).
    /// </summary>
    /// <param name="index">The index.</param>
    /// <returns>The codepoint.</returns>
    codepoint At(int index) const;
    
    /// <summary>
    /// </summary>
    /// <returns>Returns the first codepoint.</returns>
    codepoint Front();
    
    /// <summary>
    /// </summary>
    /// <returns>Returns the last codepoint.</returns>
    codepoint Back();
    
    /// <summary>
    /// Creates a <c>StringView</c> based on this string.
    /// </summary>
    /// <param name="offset">The offset in codepoints.</param>
    /// <param name="count">The count in codepointss.</param>
    /// <returns>The created view.</returns>
    StringView View(int offset = 0, int count = -1) const;
    
    /// <summary>
    /// Calculates how many bytes there are between the Start and <c>index</c>.
    /// </summary>
    /// <param name="index">The index in codepoints.</param>
    /// <returns>How many bytes there are.</returns>
    int ByteOffset(int index) const;
    
    /// <summary>
    /// Calculates how many bytes there are starting from <c>index</c> to <c>count</c>. 
    /// </summary>
    /// <param name="index">The index in codepoints.</param>
    /// <param name="count">The count in codepoints.</param>
    /// <returns>How many bytes there are.</returns>
    int ByteOffset(int index, int count) const;
    
    /// <summary>
    /// </summary>
    /// <returns>The raw data of the string.</returns>
    virtual uchar* Data() const;
    
    /// <summary>
    /// </summary>
    /// <returns>The raw data of the string as a byte pointer.</returns>
    virtual byte* ByteData() const;

    /// <summary>
    /// </summary>
    /// <returns>The raw data of the string as a <c>const char</c> pointer.</returns>
    const char* CStr() const;

    /// <summary>
    /// </summary>
    /// <returns>The allocator of the string</returns>
    Allocator Allocator() const;
    
    /// <summary>
    /// Calculates the length of the string in codepoints excluding the null terminator.
    /// </summary>
    /// <returns>The length of the string.</returns>
    int Count() const;
    
    /// <summary>
    /// Calculates the size of the string in bytes, excluding the null terminator.
    /// </summary>
    /// <returns>How many bytes long the string is.</returns>
    u64 SizeInBytes() const;
    
    /// <summary>
    /// </summary>
    /// <returns>How many bytes long the buffer is.</returns>
    u64 BufferSize() const;

    codepoint operator[](int index) const;

    String& operator=(const uchar* other);

    String& operator=(const String& other);

    bool operator==(const String& other) const;
    bool operator==(StringView other) const;
    bool operator==(const char* other) const;

    bool operator!=(const String& other) const;
    bool operator!=(StringView other) const;
    bool operator!=(const char* other) const;

    // Ranged for begin/end functions
    StringViewIt begin() const;
    StringViewIt end() const;

protected:    
    /// <summary>
    /// Grows the memory by <c>bytes</c> amount.
    /// </summary>
    /// <param name="bytes">How many bytes to grow the memory by.</param>
    void Grow(int bytes);

    pd::Allocator allocator = Allocator::None;
    byte* memory = nullptr;

    // BufferSize of the memory in bytes
    u64 bufferSize = 0;
};

template<int maxCapacity>
class BoundedString final : public String {
public:
    BoundedString() : String(Allocator::None) {
        bufferSize = maxCapacity;
    }

    BoundedString(StringView text) : BoundedString() {
        Set(text);
    }

    BoundedString(const uchar* text) : BoundedString() {
        Set(text);
    }

    virtual void ChangeAllocator(pd::Allocator allocator) override {
        // Not allowed
    }

    virtual inline byte* ByteData() const override {
        return (byte*)&stackMemory[0];
    }

private:
    byte stackMemory[maxCapacity] = {};
};

// Print

template<>
inline void PrintType(StringView& type, FormatInfo& info) {

    int size = (int)type.SizeInBytes();
    if (info.precisionSpecified) {
        size = info.precision;
    }

    size = Clamp(size, 0, (int)type.SizeInBytes());

    if (info.pretty) {
        PrintfToStream(info.output, "\"");
    }

    PrintfToStream(info.output, "%.*s", size, type.CStr());

    if (info.pretty) {
        PrintfToStream(info.output, "\"");
    }
}

template<>
inline void PrintType(String& type, FormatInfo& info) {
    StringView view = type.View();
    PrintType(view, info);
}

}
