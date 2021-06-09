#include "String.h"

#include <cstdio>
#include <cstdarg>

#include "Pandora/Core/Assert.h"
#include "Pandora/Core/Data/Memory.h"

#include "Pandora/Libs/utf8/utf8.h"

namespace pd {

String::String(pd::Allocator allocator) :
    allocator(allocator) {}

String::String(const uchar* string, pd::Allocator allocator) :
    allocator(allocator) {
    Set(string);
}

String::String(const String& other) : String(other.Data(), other.Allocator()) {}

String::~String() {
    Delete();
}

void String::Delete() {
    if (memory) {
        Free(memory, allocator);
    }

    memory = nullptr;
    bufferSize = 0;
}

void String::Set(const uchar* string) {
    if (!string) return;

    int size = (int)UTF8Size(string);

    Grow(size);
    utf8cpy(ByteData(), string);
}

void String::Set(StringView text) {
    Set(text.ToString());
}

void String::FormatF(const uchar* fmt, ...) {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    buffer[0] = '\0';

    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, (char*)fmt, args);
    va_end(args);

    Set(buffer);
}

void String::Append(const uchar* text) {
    if (SizeInBytes() == 0) {
        Set(text);
    } else {
        int subSize = (int)UTF8Size(text) - 1;

        Grow(subSize);
        utf8cat(Data(), text);
    }
}

void String::Append(StringView text) {
    Append(text.ToString());
}

void String::Append(codepoint point) {
    int size = (int)SizeInBytes();

    // If we have no memory yet we allocate a null-terminated memory
    if (size == 0) {
        Set("");
        size = 1;
    }

    int pointSize = CodepointSize(point);

    Grow(pointSize);
    utf8catcodepoint(ByteData() + size - 1, point, bufferSize - (u64)((i64)size - (i64)pointSize));
    ByteData()[size + pointSize - 1] = '\0';
}

void String::Substr(String& out, int index, int count) {
    if (count <= 0) {
        count = this->Count() - index;
    }

    PD_ASSERT_D(index >= 0 && index < Count() && index + count < Count(),
               u8"illegal substring range, valid: 0:%d, given: %d:%d", Count(), index, count);

    if (*this == out) {
        // If we're the target we can just remove all surrounding characters
        Remove(0, index);
        Remove(count, this->Count() - count);
    } else {
        // This feels dirty...
        int offset = ByteOffset(index);
        int subSize = ByteOffset(index, count);

        out.Delete();
        out.Grow(subSize + 1);
        out.ByteData()[subSize] = '\0';
        MemoryCopy(out.ByteData(), ByteData() + offset, subSize);
    }
}

int String::PurgeChars(const uchar* chars) {
    int removed = 0;

    uchar* u = ByteData();
    while ((u = utf8pbrk(u, chars))) {
        removed += 1;

        codepoint point = 0;
        GetNextCodepoint(u, &point);

        int pointSize = CodepointSize(point);
        MemoryCopy(u, (byte*)u + pointSize, (byte*)u - ByteData() - pointSize, true);
    }

    return removed;
}

void String::TrimFront(codepoint toTrim) {
    while (Front() == toTrim) {
        Remove(0);
    }
}

void String::TrimBack(codepoint toTrim) {
    while (Back() == toTrim) {
        Remove(Count() - 1);
    }
}

void String::Trim(codepoint toTrim) {
    TrimFront(toTrim);
    TrimBack(toTrim);
}

Optional<int> String::FindAny(const uchar* chars, int offset) {
    if (offset >= Count()) return Optional<int>();

    if (offset <= 0) {
        offset = 0;
    }

    int startOffset = ByteOffset(offset);

    uchar* u = ByteData() + startOffset;

    int i = offset;
    codepoint point = 0;
    for (u = GetNextCodepoint(u, &point); point; u = GetNextCodepoint(u, &point)) {

        codepoint findPoint = 0;
        for (uchar* c = GetNextCodepoint(chars, &findPoint); findPoint; c = GetNextCodepoint(c, &findPoint)) {
            if (point == findPoint) {
                return i;
            }
        }

        i += 1;
    }

    return Optional<int>();
}

Optional<int> String::FindAnyReverse(const uchar* chars, int offset) {
    if (offset <= 0) {
        offset = Count();
    } else {
        offset = Count() - offset;
    }

    Optional<int> finalIndex;
    Optional<int> index = -1;

    while ((index = FindAny(chars, *index + 1)).HasValue()) {
        if (*index > offset) break;

        if (*index > *finalIndex) {
            finalIndex.SetValue(*index);
        }
    }

    return finalIndex;
}

Optional<int> String::Find(const uchar* substring, int offset) {
    if (offset < 0) {
        offset = 0;
    }

    uchar* result = utf8str(ByteData() + ByteOffset(offset), substring);

    if (!result) return Optional<int>();

    int targetOffset = (int)((byte*)result - ByteData());
    int byteOffset = 0;

    // Iterate through each codepoint, counting the bufferSize to find the index of the target string
    int i = 0;
    codepoint point = 0;
    for (uchar* u = GetNextCodepoint(ByteData(), &point); point; u = GetNextCodepoint(u, &point)) {

        if (byteOffset == targetOffset) {
            return i;
        }

        i += 1;
        byteOffset += CodepointSize(point);
    }

    return Optional<int>();
}

Optional<int> String::FindReverse(const uchar* substring, int offset) {
    if (offset <= 0) {
        offset = Count();
    } else {
        offset = Count() - offset;
    }

    Optional<int> finalIndex;
    Optional<int> index = -1;

    int subLength = UTF8Count(substring);

    while ((index = Find(substring, *index + 1)).HasValue()) {
        if (*index + subLength > offset) break;

        if (*index > *finalIndex) {
            finalIndex.SetValue(*index);
        }
    }

    return finalIndex;
}

void String::Insert(int index, const uchar* text) {
    PD_ASSERT_D(index >= 0 && index <= Count(),
               u8"illegal insertion index, valid 0:%d, given: %d", Count(), index);

    if (Count() == 0) {
        Set(text);
        return;
    }

    int textSize = (int)UTF8Size(text) - 1;
    Grow(textSize);

    int offset = ByteOffset(index);

    u64 copySize = SizeInBytes() - textSize - offset;

    MemoryCopy(ByteData() + offset + textSize,
               ByteData() + offset,
               SizeInBytes() - textSize - offset, true);

    MemoryCopy(ByteData() + offset, text, textSize);
}

void String::Remove(int index, int count) {
    PD_ASSERT_D(index >= 0 && index < Count() && count > 0 && index + count <= Count(),
               u8"illegal removal range, valid: 0:%d, given %d:%d", Count(), index, count);

    int removeStart = ByteOffset(index);
    int removeSize = ByteOffset(index, count);

    MemoryCopy(ByteData() + removeStart,
               ByteData() + removeStart + removeSize,
               SizeInBytes() - removeStart - removeSize, true);
}

int String::Replace(const uchar* find, const uchar* replace) {
    int replaceCount = 0;

    int findLength = UTF8Count(find);
    int replaceLength = UTF8Count(replace);

    Optional<int> offset;
    while ((offset = Find(find, *offset)).HasValue()) {
        replaceCount += 1;
        Remove(*offset, findLength);
        Insert(*offset, replace);

        *offset += replaceLength;
        if (*offset > Count()) {
            return replaceCount;
        }
    }

    return replaceCount;
}

void String::Split(const uchar* seperator, Array<String>& out, pd::Allocator stringAllocator) {
    int lastIndex = 0;
    Optional<int> currentIndex = -1;

    int seperatorLen = UTF8Count(seperator);

    while ((currentIndex = Find(seperator, *currentIndex + 1)).HasValue()) {
        out.Reserve(1);
        out.Last().ChangeAllocator(stringAllocator);
        Substr(out.Last(), lastIndex, *currentIndex - lastIndex);

        lastIndex = *currentIndex + seperatorLen;
    }

    out.Reserve(1);
    out.Last().ChangeAllocator(stringAllocator);
    Substr(out.Last(), lastIndex, *currentIndex - lastIndex);
}

void String::ToUpper() {
    if (Data()) {
        utf8upr(Data());
    }
}

void String::ToLower() {
    if (Data()) {
        utf8lwr(Data());
    }
}

void String::ChangeAllocator(pd::Allocator allocator) {
    if (this->allocator == allocator) return;

    if (allocator == pd::Allocator::None) {
        Delete();
    } else if (allocator == pd::Allocator::Persistent) {
        byte* newMemory = (byte*)Alloc(bufferSize, allocator);
        MemoryCopy(newMemory, memory, bufferSize);

        // We know that `memory` doesn't use the persistent allocator so we don't need to free it
        memory = newMemory;
    } else if (allocator == pd::Allocator::Temporary) {
        byte* newMemory = (byte*)Alloc(bufferSize, allocator);
        MemoryCopy(newMemory, memory, bufferSize);

        if (this->allocator == pd::Allocator::Persistent) {
            Free(memory);
        }

        memory = newMemory;
    }

    this->allocator = allocator;
}

bool String::IsValid() {
    return bufferSize > 0 && IsValidUTF8(Data()) == 0;
}

codepoint String::At(int index) const {
    PD_ASSERT_D(index >= 0 && index < Count(),
               u8"index out of range, valid: 0:%d, given: %d", Count(), index);

    codepoint point = 0;
    uchar* u = ByteData();
    for (int i = 0; i < index + 1; i++) {
        u = GetNextCodepoint(u, &point);
    }

    return point;
}

codepoint String::Front() {
    return At(0);
}

codepoint String::Back() {
    return At(Count() - 1);
}

StringView String::View(int offset, int count) const {
    if (count <= 0) {
        count = Count() - offset;
    }

    int byteOffset = ByteOffset(offset);
    int viewSize = ByteOffset(offset, count);

    return StringView(ByteData() + byteOffset, count, viewSize);
}

int String::ByteOffset(int index) const {
    return ByteOffset(0, index);
}

int String::ByteOffset(int index, int count) const {
    if (index == 0 && count == 0 && Count() == 0) return 0;

    PD_ASSERT_D(index >= 0 && index < Count() && index + count <= Count(),
               u8"illegal byte offset range, valid: 0:%d, given: %d:%d", Count(), index, count);

    int offset = 0;

    codepoint point = 0;
    uchar* u = ByteData();
    for (int i = 0; i < index + count; i++) {
        u = GetNextCodepoint(u, &point);

        if (i >= index) {
            offset += CodepointSize(point);
        }
    }

    return offset;
}

uchar* String::Data() const {
    return ByteData();
}

byte* String::ByteData() const {
    return memory;
}

const char* String::CStr() const {
    return (const char*)ByteData();
}

Allocator String::Allocator() const {
    return allocator;
}

int String::Count() const {
    return (Data()) ? UTF8Count(Data()) : 0;
}

u64 String::SizeInBytes() const {
    return (Data()) ? UTF8Size(Data()) : 0;
}

u64 String::BufferSize() const {
    return bufferSize;
}

codepoint String::operator[](int index) const {
    return At(index);
}

String& String::operator=(const uchar* other) {
    Set(other);
    return *this;
}

String& String::operator=(const String& other) {
    Set(other.Data());
    return *this;
}

bool String::operator==(const String& other) const {
    if (SizeInBytes() == other.SizeInBytes()) {
        return utf8cmp(Data(), other.Data()) == 0;
    } else {
        return false;
    }
}

bool String::operator==(StringView other) const {
    // -1 to not include our null terminator
    if (SizeInBytes() - 1 == other.SizeInBytes()) {
        return utf8ncmp(Data(), other.Data(), other.SizeInBytes()) == 0;
    } else {
        return false;
    }
}

bool String::operator==(const char* other) const {
    return operator==(StringView(other));
}

bool String::operator!=(const String& other) const {
    return !operator==(other);
}

bool String::operator!=(StringView other) const {
    return !operator==(other);
}

bool String::operator!=(const char* other) const {
    return !operator==(other);
}

StringViewIt String::begin() const {
    return View().begin();
}

StringViewIt String::end() const {
    return View().end();
}

void String::Grow(int bytes) {
    // Should these be in here?
    const int INITIAL_LENGTH = 64;
    const int GROW_FACTOR = 2;

    if (SizeInBytes() + bytes >= bufferSize) {
        if (!Data()) {
            bufferSize = INITIAL_LENGTH;
        }

        while (bufferSize <= SizeInBytes() + bytes) {
            bufferSize *= GROW_FACTOR;
        }

        memory = (byte*)Realloc(ByteData(), bufferSize, allocator);
    }
}

}
