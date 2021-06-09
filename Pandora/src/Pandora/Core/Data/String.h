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
    String(const String& other);

    virtual ~String();

    /**
     * \brief Frees the memory associated with the string.
     * This is called on destruction.
     */
    void Delete();

    /**
     * \param text The new string text.
     */
    void Set(const uchar* text);

    /**
     * \param text The new string text.
     */
    void Set(StringView text);

    /**
     * \brief Formats the string using printf-formatting.
     * 
     * \param fmt The printf format string.
     * \param ... The format arguments.
     */
    void FormatF(const uchar* fmt, ...);

    /**
     * \brief Format the string using the logger.
     * 
     * \param fmt The format string.
     * \param args The format arguments.
     */
    template<typename... Args>
    void Format(StringView fmt, const Args&... args) {
        MemoryStream stream(32, true, pd::Allocator::Temporary);
        pd::Log(stream, fmt, args...);
        stream.WriteByte('\0');
        Set(stream.Data());
    }

    /**
     * \param text The UTF-8 text to append.
     */
    void Append(const uchar* text);

    /**
     * \param text The UTF-8 text to append.
     */
    void Append(StringView text);

    /**
     * \param text Appends the unicode codepoint.
     */
    void Append(codepoint text);

    /**
     * \brief Sets `out` to the specified substring.
     * 
     * \param out The output string.
     * \param index The starting index.
     * \param count The substring count. Pass -1 for the remaining characters.
     */
    void Substr(String& out, int index, int count = -1);

    /**
     * \brief Removes all characters containing any character from the `chars` string.
     * 
     * \param chars The list of characters to purge.
     * \return How many characters were purged.
     */
    int PurgeChars(const uchar* chars);

    /**
     * \brief Removes all characters from the front that are equal to `toTrim`.
     * 
     * \param toTrim The codepoint to trim.
     */
    void TrimFront(codepoint toTrim);

    /**
     * \brief Removes all characters from the back that are equal to `toTrim`.
     * 
     * \param toTrim The codepoint to trim.
     */
    void TrimBack(codepoint toTrim);

    /**
     * \brief Removes all characters at the front and back that
     * are equal to `toTrim`.
     * 
     * \param toTrim The codepoint to trim.
     */
    void Trim(codepoint toTrim);

    /**
     * \brief Finds the first occurence that matches any codepoint in `chars`.
     * 
     * \param chars The list of UTF-8 codepoints to match.
     * \param offset The offset from the start.
     * \return The index of the first occurence, if any.
     */
    Optional<int> FindAny(const uchar* chars, int offset = 0);

    /**
     * \brief Finds the last occurence that matches any codepoint in `chars`.
     * 
     * \param chars The list of UTF-8 codepoints to match.
     * \param offset The offset from the end.
     * \return The index of the last occurence, if any.
     */
    Optional<int> FindAnyReverse(const uchar* chars, int offset = 0);

    /**
     * \brief Finds the first occurence of the substring.
     * 
     * \param substring The UTF-8 substring.
     * \param offset The offset from the start.
     * \return The index of the first substring.
     */
    Optional<int> Find(const uchar* substring, int offset = 0);

    /**
     * \brief Finds the last occurence of the substring.
     * 
     * \param substring The UTF-8 substring.
     * \param offset The offset from the end.
     * \return The index of the last occurence.
     */
    Optional<int> FindReverse(const uchar* substring, int offset = 0);

    /**
     * \brief Inserts the substring into the string at index.
     * 
     * \param index The index.
     * \param text The UTF-8 string to insert.
     */
    void Insert(int index, const uchar* text);

    /**
     * \brief Removes `count` characters starting at the index.
     * 
     * \param index The starting index.
     * \param count How many characters to remove.
     */
    void Remove(int index, int count = 1);

    /**
     * \brief Finds and replaces all occurences.
     * 
     * \param find The substring to find.
     * \param replace The substring to replace it with.
     * \return How many occurences were replaced.
     */
    int Replace(const uchar* find, const uchar* replace);

    /**
     * \brief Splits the string into an array of substrings.
     * 
     * \param seperator The UTF-8 seperator.
     * \param out The output array.
     * \param stringAllocator The allocator to use for each split.
     */
    void Split(const uchar* seperator, Array<String>& out,
               pd::Allocator stringAllocator = pd::Allocator::Persistent);

    /**
     * \brief Converts all characters to uppercase.
     */
    void ToUpper();

    /**
     * \brief Converts all characters to lowercase.
     */
    void ToLower();

#if defined(PD_WINDOWS)

    /**
     * \brief Converts the UTF-8 string to a Windows-specific wide string.
     * 
     * \param allocator The allocator to use for the wide string.
     * \return The converted string.
     */
    inline wchar* ToWide(pd::Allocator allocator = pd::Allocator::Temporary) {
        return UTF8ToWide(Data(), allocator);
    }
#endif

    /**
     * \brief Changes the allocator.
     * If `Allocator::None` is passed, `Delete()` is called.
     * If a different allocator is passed, all memory is copied to a new buffer
     * and the old buffer is freed.
     * 
     * \param allocator The new allocator.
     */
    virtual void ChangeAllocator(pd::Allocator allocator);

    /**
     * \return Whether or not the string is valid UTF-8.
     */
    bool IsValid();

    /**
     * \brief Gets the codepoint at the specified index.
     * This operation is O(n).
     * 
     * \param index The index.
     * \return The codepoint.
     */
    codepoint At(int index) const;

    /**
     * \return The first codepoint.
     */
    codepoint Front();

    /**
     * \return The last codepoint.
     * This location is O(n).
     */
    codepoint Back();

    /**
     * \brief Creates a string view based on this string.
     * 
     * \param offset The offset from the start.
     * \param count How many characters the view should be. Pass -1 for the remaining characters.
     * \return The string view.
     */
    StringView View(int offset = 0, int count = -1) const;

    /**
     * \brief Calculates how many bytes are from the start to `index`.
     * 
     * \param index The index.
     * \return How many bytes are between the start and `index`.
     */
    int ByteOffset(int index) const;

    /**
     * \brief Calculates how many bytes are in the specified range.
     * 
     * \param index The starting index.
     * \param count How many characters to count.
     * \return How many bytes are in the specified range.
     */
    int ByteOffset(int index, int count) const;

    /**
     * \return The raw data pointer of the string.
     */
    virtual uchar* Data() const;

    /**
     * \return The raw data pointer of the string as a byte pointer.
     */
    virtual byte* ByteData() const;

    /**
     * \return The raw data pointer of the string as a `const char` pointer.
     */
    const char* CStr() const;

    /**
     * \return The allocator the string uses.
     */
    Allocator Allocator() const;

    /**
     * \brief Calculates the length of the string.
     * 
     * \return How many codepoints are in the string, exlcuding the null terminator.
     */
    int Count() const;

    /**
     * \brief Calculates the size of the string in bytes.
     * 
     * \return How many bytes are in the string, excluding the null terminator.
     */
    u64 SizeInBytes() const;

    /**
     * \return The size of the entire buffer in bytes.
     */
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

    /**
     * \brief Grows the buffer by `bytes` amount.
     * 
     * \param bytes How many bytes to grow the buffer by.
     */
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
