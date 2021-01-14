#pragma once

#include "Pandora/Core/Data/Allocator.h"
#include "Pandora/Core/Data/Slice.h"

namespace pd {

class String;

#if defined(PD_WINDOWS)

/// <summary>
/// Converts the UTF-8 string <c>text</c> to a wide string. <c>text</c> must be null-terminated.
/// </summary>
/// <param name="text">The text.</param>
/// <param name="allocator">The allocator to use for the wide string.</param>
/// <returns>The converted wide string.</returns>
wchar* UTF8ToWide(const uchar* text, Allocator allocator = Allocator::Temporary);

/// <summary>
/// Converts the UTF-8 string <c>text</c> to a wide string, converts up to <c>bufferSize</c> bytes.
/// </summary>
/// <param name="text">The text.</param>
/// <param name="bufferSize">How many bytes to convert from <c>text</c>.</param>
/// <param name="allocator">The allocator to use for the wide string.</param>
/// <returns>The converted wide string.</returns>
wchar* UTF8ToWide(const uchar* text, int sizeInBytes, Allocator allocator = Allocator::Temporary);

/// <summary>
/// Converts the wide string <c>text</c> to a UTF-8 string. <c>text</c> must be null-terminated.
/// </summary>
/// <param name="text">The text.</param>
/// <param name="allocator">The allocator to use for the wide string.</param>
/// <returns>The converted UTF-8 string.</returns>
uchar* WideToUTF8(const wchar* text, Allocator allocator = Allocator::Temporary);

/// <summary>
/// Converts the wide string <c>text</c> to a UTF-8 string, converts up to <c>size</c> bytes.
/// </summary>
/// <param name="text">The text.</param>
/// <param name="size">The size in characters.</param>
/// <param name="allocator">The allocator to use for the wide string.</param>
/// <returns>The converted UTF-8 string.</returns>
uchar* WideToUTF8(const wchar* text, int size, Allocator allocator = Allocator::Temporary);

#endif

class StringViewIt;

class StringView {
public:
    StringView() = default;
    StringView(const char* text);
    StringView(const char* text, int count);
    StringView(String& string);
    StringView(const uchar* text, int count, int size);

    /// <summary>
    /// </summary>
    /// <returns>The view as a slice of bytes.</returns>
    Slice<byte> ToSlice() const;

    /// <summary>
    /// </summary>
    /// <returns>The raw data of the string.</returns>
    const uchar* Data() const;

    /// <summary>
    /// </summary>
    /// <returns>The raw data of the string as a byte pointer.</returns>
    const byte* ByteData() const;

    /// <summary>
    /// </summary>
    /// <returns>The raw data of the string as a <c>const char</c> pointer.</returns>
    const char* CStr() const;

    /// <summary>
    /// </summary>
    /// <returns>How many codepoints are in the view.</returns>
    int Count() const;

    /// <summary>
    /// Returns how many bytes the view is, excluding the null terminator.
    /// </summary>
    /// <returns>How big the view is in bytes.</returns>
    u64 SizeInBytes() const;

    /// <summary>
    /// Adjusts the current view.
    /// </summary>
    /// <param name="Start">The Start Start relative to the current view.</param>
    /// <param name="count">The new count.</param>
    void Adjust(int start, int count);

    codepoint At(int index) const;

    const uchar* ToString(Allocator allocator = Allocator::Temporary);

#if defined(PD_WINDOWS)
    /// <summary>
    /// Converts the view to a wide string.
    /// </summary>
    /// <param name="allocator">The allocator to use.</param>
    /// <returns>The wide string.</returns>
    inline wchar* ToWide(Allocator allocator = Allocator::Temporary) {
        return UTF8ToWide(memory, (int)sizeInBytes, allocator);
    }
#endif

    bool operator==(const StringView& other) const;

    codepoint operator[](int index) const;

    StringViewIt begin() const;
    StringViewIt end() const;

private:
    byte* memory = nullptr;

    // The bufferSize of the view
    u64 sizeInBytes = 0;

    // How many codepoints are in the view
    int count = 0;
};

struct StringViewIt {
    StringViewIt(StringView parent, int i) : parent(parent), i(i) {}

    codepoint operator*() const {
        return parent.At(i);
    }

    void operator++() {
        i += 1;
    }

    bool operator==(const StringViewIt& other) {
        return i == other.i && parent.Data() == other.parent.Data();
    }

    bool operator!=(const StringViewIt& other) {
        return !operator==(other);
    }

private:
    int i = 0;
    StringView parent;
};

}
