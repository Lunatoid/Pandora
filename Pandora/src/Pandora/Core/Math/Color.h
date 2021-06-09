#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Math/Vector.h"

namespace pd {

struct Color {
    Color(f32 rgb = 1.0f, f32 a = 1.0f) :
        r(rgb), g(rgb), b(rgb), a(a) {}

    Color(f32 r, f32 g, f32 b, f32 a = 1.0f) :
        r(r), g(g), b(b), a(a) {}

    Color(int r, int g, int b, int a = 255) :
        r((f32)r / 255.0f), g((f32)g / 255.0f), b((f32)b / 255.0f), a((f32)a / 255.0f) {}

    Color(Vec2 rg) :
        r(rg.x), g(rg.y), b(0.0f), a(1.0f) {}

    Color(Vec3 rgb) :
        r(rgb.x), g(rgb.y), b(rgb.z), a(1.0f) {}

    Color(Vec4 rgba) :
        r(rgba.x), g(rgba.y), b(rgba.z), a(rgba.w) {}

    union {
        struct {
            f32 r;
            f32 g;
            f32 b;
            f32 a;
        };

        Vec3 rgb;
        Vec4 rgba;

        f32 elements[4];
    };

    inline f32& operator[](const int& i) {
        return elements[i];
    }
};

// Print

template<>
inline void PrintType(Color& type, FormatInfo& info) {
    if (info.base == DisplayBase::Hexadecimal) {
        PrintfToStream(info.output, "0x%X%X%X%X", (int)(type.r * 255.0f), (int)(type.g * 255.0f),
                       (int)(type.b * 255.0f), (int)(type.a * 255.0f));
    } else if (info.pretty) {
        PrintfToStream(info.output, "{\n    R: %g,\n    G: %g\n    B: %g\n    A: %g\n}", type.r, type.g, type.b, type.a);
    } else {
        PrintType(type.rgba, info);
    }
}

// Operators

Color operator+(Color left, Color right);
Color operator-(Color left, Color right);
Color operator*(Color left, Color right);
Color operator/(Color left, Color right);

Color operator+(Color left, f32 right);
Color operator-(Color left, f32 right);
Color operator*(Color left, f32 right);
Color operator/(Color left, f32 right);

Color& operator+=(Color& left, Color& right);
Color& operator-=(Color& left, Color& right);
Color& operator*=(Color& left, Color& right);
Color& operator/=(Color& left, Color& right);

}
