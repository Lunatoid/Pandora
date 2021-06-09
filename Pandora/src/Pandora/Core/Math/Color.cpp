#include "Color.h"

#include "Pandora/Core/Math/Math.h"

namespace pd {

Color operator+(Color left, Color right) {
    Color result;

    result.r = Clamp(left.r + right.r, 0.0f, 1.0f);
    result.g = Clamp(left.g + right.g, 0.0f, 1.0f);
    result.b = Clamp(left.b + right.b, 0.0f, 1.0f);
    result.a = Clamp(left.a + right.a, 0.0f, 1.0f);

    return result;
}

Color operator-(Color left, Color right) {
    Color result;

    result.r = Clamp(left.r - right.r, 0.0f, 1.0f);
    result.g = Clamp(left.g - right.g, 0.0f, 1.0f);
    result.b = Clamp(left.b - right.b, 0.0f, 1.0f);
    result.a = Clamp(left.a - right.a, 0.0f, 1.0f);

    return result;
}

Color operator*(Color left, Color right) {
    Color result;

    result.r = Clamp(left.r * right.r, 0.0f, 1.0f);
    result.g = Clamp(left.g * right.g, 0.0f, 1.0f);
    result.b = Clamp(left.b * right.b, 0.0f, 1.0f);
    result.a = Clamp(left.a * right.a, 0.0f, 1.0f);

    return result;
}

Color operator/(Color left, Color right) {
    Color result;

    result.r = Clamp(left.r / right.r, 0.0f, 1.0f);
    result.g = Clamp(left.g / right.g, 0.0f, 1.0f);
    result.b = Clamp(left.b / right.b, 0.0f, 1.0f);
    result.a = Clamp(left.a / right.a, 0.0f, 1.0f);

    return result;
}

Color operator+(Color left, f32 right) {
    Color result;

    result.r = Clamp(left.r + right, 0.0f, 1.0f);
    result.g = Clamp(left.g + right, 0.0f, 1.0f);
    result.b = Clamp(left.b + right, 0.0f, 1.0f);
    result.a = Clamp(left.a + right, 0.0f, 1.0f);

    return result;
}

Color operator-(Color left, f32 right) {
    Color result;

    result.r = Clamp(left.r - right, 0.0f, 1.0f);
    result.g = Clamp(left.g - right, 0.0f, 1.0f);
    result.b = Clamp(left.b - right, 0.0f, 1.0f);
    result.a = Clamp(left.a - right, 0.0f, 1.0f);

    return result;
}

Color operator*(Color left, f32 right) {
    Color result;

    result.r = Clamp(left.r * right, 0.0f, 1.0f);
    result.g = Clamp(left.g * right, 0.0f, 1.0f);
    result.b = Clamp(left.b * right, 0.0f, 1.0f);
    result.a = Clamp(left.a * right, 0.0f, 1.0f);

    return result;
}

Color operator/(Color left, f32 right) {
    Color result;

    result.r = Clamp(left.r / right, 0.0f, 1.0f);
    result.g = Clamp(left.g / right, 0.0f, 1.0f);
    result.b = Clamp(left.b / right, 0.0f, 1.0f);
    result.a = Clamp(left.a / right, 0.0f, 1.0f);

    return result;
}

Color& operator+=(Color& left, Color& right) {
    return (left = left + right);
}

Color& operator-=(Color& left, Color& right) {
    return (left = left - right);
}

Color& operator*=(Color& left, Color& right) {
    return (left = left * right);
}

Color& operator/=(Color& left, Color& right) {
    return (left = left / right);
}

}
