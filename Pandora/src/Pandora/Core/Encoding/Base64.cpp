#include "Base64.h"

namespace pd {

void Base64Encode(Slice<byte> input, String& out, const char* table, char padding) {
    for (u64 i = 0; i < input.Count(); i += 3) {
        u32 n = u32(input[int(i)]) << 16;

        if (i + 1 < input.Count()) {
            n += u32(input[int(i) + 1]) << 8;
        }

        if (i + 2 < input.Count()) {
            n += u32(input[int(i) + 2]);
        }

        out.Append(codepoint(table[n >> 18 & 63]));
        out.Append(codepoint(table[n >> 12 & 63]));
        out.Append((i + 1) < input.Count() ? table[n >> 6 & 63] : padding);
        out.Append((i + 2) < input.Count() ? table[n & 63] : padding);
    }
}

void Base64Encode(Slice<byte> input, Stream& out, const char* table, char padding) {
    if (!out.CanWrite()) return;
    
    for (u64 i = 0; i < input.Count(); i += 3) {
        u32 n = u32(input[int(i)]) << 16;

        if (i + 1 < input.Count()) {
            n += u32(input[int(i) + 1]) << 8;
        }

        if (i + 2 < input.Count()) {
            n += u32(input[int(i) + 2]);
        }

        out.Write(codepoint(table[n >> 18 & 63]));
        out.Write(codepoint(table[n >> 12 & 63]));
        out.Write((i + 1) < input.Count() ? table[n >> 6 & 63] : padding);
        out.Write((i + 2) < input.Count() ? table[n & 63] : padding);
    }
}

void Base64Decode(const char* input, Array<byte>& out, const int* table, char padding) {
    int inputLength = (int)strlen(input);
    int padLength = input[inputLength - 1] == padding ? input[inputLength - 2] == padding ? 2 : 1 : 0;

    int outLength = ((inputLength * 6) >> 3) - padLength;

    out.Resize(outLength);

    for (int i = 0; i < inputLength; i += 4) {
        // Construct out 24-bit number
        u32 n = 0;
        n = table[input[i]] << 18;
        n += table[input[i + 1]] << 12;

        if (input[i + 2] != '=') {
            n += table[input[i + 2]] << 6;
        }

        if (input[i + 3] != '=') {
            n += table[input[i + 3]];
        }

        // Split it into bytes
        out.Add(byte(n >> 16 & 0xFF));

        if (input[i + 2] != '=') {
            out.Add(byte(n >> 8 & 0xFF));
        }

        if (input[i + 3] != '=') {
            out.Add(byte(n & 0xFF));
        }
    }
}

void Base64Decode(const char* input, Stream& out, const int* table, char padding) {
    if (!out.CanWrite()) return;
    
    int inputLength = (int)strlen(input);
    
    for (int i = 0; i < inputLength; i += 4) {
        // Construct out 24-bit number
        u32 n = 0;
        n = table[input[i]] << 18;
        n += table[input[i + 1]] << 12;

        if (input[i + 2] != '=') {
            n += table[input[i + 2]] << 6;
        }

        if (input[i + 3] != '=') {
            n += table[input[i + 3]];
        }

        // Split it into bytes
        out.WriteByte(byte(n >> 16 & 0xFF));

        if (input[i + 2] != '=') {
            out.WriteByte(byte(n >> 8 & 0xFF));
        }

        if (input[i + 3] != '=') {
            out.WriteByte(byte(n & 0xFF));
        }
    }
}

}
