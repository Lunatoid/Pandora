#include "StringView.h"

#if defined(PD_WINDOWS)
#include <Windows.h>
#endif

#include "Pandora/Core/Data/String.h"

#include "Pandora/Libs/utf8/utf8.h"

namespace pd {

#if defined(PD_WINDOWS)

wchar* UTF8ToWide(const uchar* text, Allocator allocator) {
    int multiByteLength = MultiByteToWideChar(CP_UTF8, NULL, (char*)text,
                                              -1, NULL, 0);

    wchar* multiByte = (wchar*)Alloc(multiByteLength * sizeof(wchar), allocator);
    MultiByteToWideChar(CP_UTF8, NULL, (char*)text, -1,
                        multiByte, multiByteLength);

    return multiByte;
}

wchar* UTF8ToWide(const uchar* text, int sizeInBytes, Allocator allocator) {
    int multiByteLength = MultiByteToWideChar(CP_UTF8, NULL, (char*)text,
                                              sizeInBytes, NULL, 0) + 1;

    wchar* multiByte = (wchar*)Alloc(multiByteLength * sizeof(wchar), allocator);
    MultiByteToWideChar(CP_UTF8, NULL, (char*)text, sizeInBytes,
                        multiByte, multiByteLength);

    multiByte[multiByteLength - 1] = L'\0';

    return multiByte;
}

uchar* WideToUTF8(const wchar* text, Allocator allocator) {
    int utf8Length = WideCharToMultiByte(CP_UTF8, NULL, text,
                                         -1, NULL, 0, NULL, NULL);

    uchar* utf8 = Alloc(utf8Length, allocator);
    WideCharToMultiByte(CP_UTF8, NULL, text, -1,
                        (char*)utf8, utf8Length, NULL, NULL);

    return utf8;
}

uchar* WideToUTF8(const wchar* text, int sizeInBytes, Allocator allocator) {
    int utf8Length = WideCharToMultiByte(CP_UTF8, NULL, text, sizeInBytes,
                                         NULL, 0, NULL, NULL) + 1;

    uchar* utf8 = Alloc(utf8Length, allocator);
    WideCharToMultiByte(CP_UTF8, NULL, text, sizeInBytes,
                        (char*)utf8, utf8Length, NULL, NULL);

    ((byte*)utf8)[utf8Length - 1] = '\0';

    return utf8;
}

#endif

StringView::StringView(const char* text) {
    memory = (byte*)text;
    sizeInBytes = (u64)UTF8Size(text) - 1;
    count = (int)UTF8Count(text);
}

StringView::StringView(const char* text, int count) {
    memory = (byte*)text;
    this->count = count;

    for (int i = 0; i < count; i++) {
        sizeInBytes += CodepointSize(At(i));
    }
}

StringView::StringView(String& string) {
    if (string.SizeInBytes() == 0) {
        memory = nullptr;
        sizeInBytes = 0;
        count = 0;
        return;
    }

    memory = string.ByteData();
    sizeInBytes = string.SizeInBytes() - 1; // Don't include null-terminator
    count = string.Count();
}

StringView::StringView(const uchar* text, int count, int size) {
    memory = (byte*)text;
    this->count = count;
    this->sizeInBytes = size;
}

Slice<byte> StringView::ToSlice() const {
    return Slice<byte>(memory, (int)sizeInBytes);
}

const uchar* StringView::Data() const {
    return memory;
}

const byte* StringView::ByteData() const {
    return memory;
}

const char* StringView::CStr() const {
    return (const char*)memory;
}

int StringView::Count() const {
    return count;
}

u64 StringView::SizeInBytes() const {
    return sizeInBytes;
}

void StringView::Adjust(int start, int count) {
    codepoint point = 0;
    uchar* u = memory;

    // Calculate new starting pointer
    for (int i = 0; i < start; i++) {
        u = GetNextCodepoint(u, &point);
        int pointSize = CodepointSize(point);
        memory += pointSize;
    }

    // Calculate size in bytes
    sizeInBytes = 0;
    u = memory;
    for (int i = 0; i < count; i++) {
        u = GetNextCodepoint(u, &point);
        int pointSize = CodepointSize(point);
        sizeInBytes += pointSize;
    }

    this->count = count;
}

codepoint StringView::At(int index) const {
    codepoint point = 0;
    const uchar* u = Data();
    for (int i = 0; i < index + 1; i++) {
        u = GetNextCodepoint(u, &point);
    }

    return point;
}

const uchar* StringView::ToString(Allocator allocator) {
    int size = (int)SizeInBytes();

    // Copy stringview into null-terminated memory
    uchar* buffer = Alloc(u64(size) + 1, allocator);
    MemoryCopy(buffer, Data(), size);
    ((byte*)buffer)[size] = '\0';

    return buffer;
}

bool StringView::operator==(const StringView& other) const {
    if (SizeInBytes() == other.SizeInBytes()) {
        return utf8ncmp(Data(), other.Data(), SizeInBytes()) == 0;
    } else {
        return false;
    }
}

codepoint StringView::operator[](int index) const {
    return At(index);
}

StringViewIt StringView::begin() const {
    return StringViewIt(*this, 0);
}

StringViewIt StringView::end() const {
    return StringViewIt(*this, Count());
}

}
