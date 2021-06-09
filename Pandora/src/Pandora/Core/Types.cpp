#include "Types.h"

#include "Pandora/Libs/utf8/utf8.h"

namespace pd {

codepoint ToUpper(codepoint point) {
    return utf8uprcodepoint(point);
}

codepoint ToLower(codepoint point) {
    return utf8lwrcodepoint(point);
}

bool IsUpper(codepoint point) {
    return utf8isupper(point) != 0;
}

bool IsLower(codepoint point) {
    return utf8islower(point) != 0;
}

int CodepointSize(codepoint point) {
    return (int)utf8codepointsize(point);
}

uchar* GetNextCodepoint(const uchar* string, codepoint* out) {
    return utf8codepoint(string, out);
}

bool IsValidUTF8(const uchar* string) {
    return utf8valid(string) == nullptr;
}

int UTF8Size(const uchar* string) {
    return (int)utf8size(string);
}

int UTF8Count(const uchar* string) {
    return (int)utf8len(string);
}

}
