#pragma once

#include "Pandora/Core/Data/Allocator.h"
#include "Pandora/Core/Data/Slice.h"

namespace pd {

class String;

#if defined(PD_WINDOWS)

/**
 * \brief Converts the UTF-8 string to a Windows-specific wide string.
 * 
 * \param text The null-terminated UTF-8 string.
 * \param allocator The allocator to use for the wide string.
 * \return The converted string.
 */
wchar* UTF8ToWide(const uchar* text, Allocator allocator = Allocator::Temporary);

/**
 * \brief Converts the UTF-8 string to a Windows-specific wide string.
 * Converts up to `sizeInBytes` bytes.
 * 
 * \param text The UTF-8 string.
 * \param sizeInBytes How mnay bytes to convert.
 * \param allocator The allocator to use for the wide string.
 * \return The converted string.
 */
wchar* UTF8ToWide(const uchar* text, int sizeInBytes,
                  Allocator allocator = Allocator::Temporary);

/**
 * \brief Converts the Windows-specific wide string to a UTF-8 string.
 * 
 * \param text The wide string.
 * \param allocator The allocator to use for the UTF-8 string.
 * \return The converted string.
 */
uchar* WideToUTF8(const wchar* text, Allocator allocator = Allocator::Temporary);

/**
 * \brief Converts the Windows-specific wide string to a UTF-8 string.
 * 
 * \param text The wide string.
 * \param sizeInBytes. How many bytes to convert.
 * \param allocator The allocator to use for the UTF-8 string.
 * \return The converted string.
 */
uchar* WideToUTF8(const wchar* text, int sizeInBytes,
                  Allocator allocator = Allocator::Temporary);

#endif

class StringViewIt;

class StringView {
public:
    StringView() = default;
    StringView(const char* text);
    StringView(const char* text, int count);
    StringView(String& string);
    StringView(const uchar* text, int count, int size);

    /**
     * \return The view as a slice of bytes.
     */
    Slice<byte> ToSlice() const;

    /**
     * \return The raw data pointer of the view.
     */
    const uchar* Data() const;

    /**
     * \return The raw data pointer to the view as a byte pointer.
     */
    const byte* ByteData() const;

    /**
     * \return The raw data pointer as a `const char` pointer.
     */
    const char* CStr() const;

    /**
     * \return How many codepoints are in the view.
     */
    int Count() const;

    /**
     * \return How long the view is in bytes.
     */
    u64 SizeInBytes() const;

    /**
     * \brief Adjusts the view.
     * 
     * \param start The offset relative to the start.
     * \param count The new count.
     */
    void Adjust(int start, int count);


    /**
     * \param index The index.
     * \return The codepoint at the index.
     */
    codepoint At(int index) const;

    /**
     * \brief Duplicates the view into a newly allocated string.
     * 
     * \param allocator The allocator to use.
     * \return The duplicated strong.
     */
    const uchar* ToString(Allocator allocator = Allocator::Temporary);

#if defined(PD_WINDOWS)
    /**
     * \brief Converts the view to a Windows-specific wide string.
     * 
     * \param allocator The allocator to use.
     * \return The Windows-specific wide string.
     */
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
    u64 sizeInBytes = 0;
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
