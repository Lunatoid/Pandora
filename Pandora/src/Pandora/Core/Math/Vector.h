#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Logging/PrintType.h"

namespace pd {

struct Vec2 {
    Vec2(f32 v = 0.0f) :
        x(v), y(v) {}

    Vec2(f32 x, f32 y) :
        x(x), y(y) {}

    union {
        struct {
            f32 x;
            f32 y;
        };

        f32 elements[2];
    };

    inline f32& operator[](const int& i) {
        return elements[i];
    }

    inline Vec2 operator-() {
        return Vec2(-x, -y);
    }
};

struct Vec3 {
    Vec3(f32 v = 0.0f) :
        x(v), y(v), z(v) {}

    Vec3(f32 x, f32 y, f32 z) :
        x(x), y(y), z(z) {}

    Vec3(Vec2 v2) :
        Vec3(v2.x, v2.y, 0.0f) {}

    union {
        struct {
            f32 x;
            f32 y;
            f32 z;
        };

        Vec2 xy;

        struct {
            f32 _padding;
            Vec2 yz;
        };

        f32 elements[3];
    };

    inline f32& operator[](const int& i) {
        return elements[i];
    }

    inline Vec3 operator-() {
        return Vec3(-x, -y, -z);
    }
};

struct Vec4 {
    Vec4(f32 v = 0.0f) :
        x(v), y(v), z(v), w(v) {}

    Vec4(f32 x, f32 y, f32 z, f32 w) :
        x(x), y(y), z(z), w(w) {}

    Vec4(Vec2 v2) :
        Vec4(v2.x, v2.y, 0.0f, 0.0f) {}

    Vec4(Vec3 v3) :
        Vec4(v3.x, v3.y, v3.z, 0.0f) {}

    union {
        struct {
            f32 x;
            f32 y;
            f32 z;
            f32 w;
        };

        struct {
            Vec2 xy;
            Vec2 zw;
        };

        struct {
            f32 _padding;
            union {
                Vec2 yz;
                Vec3 yzw;
            };
        };

        Vec3 xyz;
        f32 elements[4];
    };

    inline f32& operator[](const int& i) {
        return elements[i];
    }

    inline Vec4 operator-() {
        return Vec4(-x, -y, -z, w);
    }
};

/// <summary>
/// Calculates the dot product of <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The first vector.</param>
/// <param name="b">The second vector.</param>
/// <returns>The dot product.</returns>
f32 Dot(Vec2 a, Vec2 b);

/// <summary>
/// Calculates the dot product of <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The first vector.</param>
/// <param name="b">The second vector.</param>
/// <returns>The dot product.</returns>
f32 Dot(Vec3 a, Vec3 b);

/// <summary>
/// Calculates the dot product of <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The first vector.</param>
/// <param name="b">The second vector.</param>
/// <returns>The dot product.</returns>
f32 Dot(Vec4 a, Vec4 b);

/// <summary>
/// Calculates the cross product of <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The first vector.</param>
/// <param name="b">The second vector.</param>
/// <returns>The cross product.</returns>
Vec3 Cross(Vec3 a, Vec3 b);

/// <summary>
/// Calculates the distance between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The first vector.</param>
/// <param name="b">The second vector.</param>
/// <returns>The distance.</returns>
f32 Distance(Vec2 a, Vec2 b);

/// <summary>
/// Calculates the distance between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The first vector.</param>
/// <param name="b">The second vector.</param>
/// <returns>The distance.</returns>
f32 Distance(Vec3 a, Vec3 b);

/// <summary>
/// Calculates the distance between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The first vector.</param>
/// <param name="b">The second vector.</param>
/// <returns>The distance.</returns>
Vec4 Distance(Vec4 a, Vec4 b);

/// <summary>
/// Calculates the length of the vector.
/// </summary>
/// <param name="v">The vector.</param>
/// <returns>The magnitude.</returns>
f32 Magnitude(Vec2 v);

/// <summary>
/// Calculates the length of the vector.
/// </summary>
/// <param name="v">The vector.</param>
/// <returns>The magnitude.</returns>
f32 Magnitude(Vec3 v);

/// <summary>
/// Calculates the length of the vector.
/// </summary>
/// <param name="v">The vector.</param>
/// <returns>The magnitude.</returns>
f32 Magnitude(Vec4 v);

/// <summary>
/// Calculates the squared length of the vector.
/// </summary>
/// <param name="v">The vector.</param>
/// <returns>The square magnitude.</returns>
f32 SquareMagnitude(Vec2 v);

/// <summary>
/// Calculates the squared length of the vector.
/// </summary>
/// <param name="v">The vector.</param>
/// <returns>The square magnitude.</returns>
f32 SquareMagnitude(Vec3 v);

/// <summary>
/// Calculates the squared length of the vector.
/// </summary>
/// <param name="v">The vector.</param>
/// <returns>The square magnitude.</returns>
f32 SquareMagnitude(Vec4 v);

/// <summary>
/// Calculates the vector with a magnitude of 1.
/// </summary>
/// <param name="v">The vector.</param>
/// <returns>The normalized vector.</returns>
Vec2 Normalize(Vec2 v);

/// <summary>
/// Calculates the vector with a magnitude of 1.
/// </summary>
/// <param name="v">The vector.</param>
/// <returns>The normalized vector.</returns>
Vec3 Normalize(Vec3 v);

/// <summary>
/// Calculates the vector with a magnitude of 1.
/// </summary>
/// <param name="v">The vector.</param>
/// <returns>The normalized vector.</returns>
Vec4 Normalize(Vec4 v);

/// <summary>
/// Reflects the vector off the plane by the specified normal.
/// </summary>
/// <param name="inDir">The incoming direction.</param>
/// <param name="inNormal">The normal of the reflection plane.</param>
/// <returns>The reflected vector.</returns>
Vec2 Reflect(Vec2 inDir, Vec2 inNormal);

/// <summary>
/// Reflects the vector off the plane by the specified normal.
/// </summary>
/// <param name="inDir">The incoming direction.</param>
/// <param name="inNormal">The normal of the reflection plane.</param>
/// <returns>The reflected vector.</returns>
Vec3 Reflect(Vec3 inDir, Vec3 inNormal);

/// <summary>
/// Calculates the angle between <c>from</c> and <c>to</c> in degrees.
/// </summary>
/// <param name="from">The from vector.</param>
/// <param name="from">The to vector.</param>
/// <returns>The angle in degrees.</returns>
f32 Angle(Vec2 from, Vec2 to);

/// <summary>
/// Calculates the angle between <c>from</c> and <c>to</c> in degrees.
/// </summary>
/// <param name="from">The from vector.</param>
/// <param name="from">The to vector.</param>
/// <returns>The angle in degrees.</returns>
f32 Angle(Vec3 from, Vec3 to);

// Print

template<>
inline void PrintType(Vec2& type, FormatInfo& info) {
    PrintfToStream(info.output, "{%s", (info.pretty) ? "\n    " : " ");
    PrintType(type.x, info);
    PrintfToStream(info.output, ",%s", (info.pretty) ? "\n    " : " ");
    PrintType(type.y, info);
    PrintfToStream(info.output, "%s}", (info.pretty) ? "\n" : " ");
}

template<>
inline void PrintType(Vec3& type, FormatInfo& info) {
    PrintfToStream(info.output, "{%s", (info.pretty) ? "\n    " : " ");
    PrintType(type.x, info);
    PrintfToStream(info.output, ",%s", (info.pretty) ? "\n    " : " ");
    PrintType(type.y, info);
    PrintfToStream(info.output, ",%s", (info.pretty) ? "\n    " : " ");
    PrintType(type.z, info);
    PrintfToStream(info.output, "%s}", (info.pretty) ? "\n" : " ");
}

template<>
inline void PrintType(Vec4& type, FormatInfo& info) {
    PrintfToStream(info.output, "{%s", (info.pretty) ? "\n    " : " ");
    PrintType(type.x, info);
    PrintfToStream(info.output, ",%s", (info.pretty) ? "\n    " : " ");
    PrintType(type.y, info);
    PrintfToStream(info.output, ",%s", (info.pretty) ? "\n    " : " ");
    PrintType(type.z, info);
    PrintfToStream(info.output, ",%s", (info.pretty) ? "\n    " : " ");
    PrintType(type.z, info);
    PrintfToStream(info.output, "%s}", (info.pretty) ? "\n" : " ");
}

// Operators

bool operator==(const Vec2& left, const Vec2& right);
bool operator==(const Vec3& left, const Vec3& right);
bool operator==(const Vec4& left, const Vec4& right);

bool operator!=(const Vec2& left, const Vec2& right);
bool operator!=(const Vec3& left, const Vec3& right);
bool operator!=(const Vec4& left, const Vec4& right);

Vec2 operator+(const Vec2& left, const Vec2& right);
Vec3 operator+(const Vec3& left, const Vec3& right);
Vec4 operator+(const Vec4& left, const Vec4& right);

Vec2 operator+(const Vec2& left, f32 right);
Vec3 operator+(const Vec3& left, f32 right);
Vec4 operator+(const Vec4& left, f32 right);

Vec2 operator-(const Vec2& left, const Vec2& right);
Vec3 operator-(const Vec3& left, const Vec3& right);
Vec4 operator-(const Vec4& left, const Vec4& right);

Vec2 operator-(const Vec2& left, f32 right);
Vec3 operator-(const Vec3& left, f32 right);
Vec4 operator-(const Vec4& left, f32 right);

Vec2 operator*(const Vec2& left, const Vec2& right);
Vec3 operator*(const Vec3& left, const Vec3& right);
Vec4 operator*(const Vec4& left, const Vec4& right);

Vec2 operator*(const Vec2& left, f32 right);
Vec3 operator*(const Vec3& left, f32 right);
Vec4 operator*(const Vec4& left, f32 right);

Vec2 operator/(const Vec2& left, const Vec2& right);
Vec3 operator/(const Vec3& left, const Vec3& right);
Vec4 operator/(const Vec4& left, const Vec4& right);

Vec2 operator/(const Vec2& left, f32 right);
Vec3 operator/(const Vec3& left, f32 right);
Vec4 operator/(const Vec4& left, f32 right);

Vec2& operator+=(Vec2& left, Vec2& right);
Vec3& operator+=(Vec3& left, Vec3& right);
Vec4& operator+=(Vec4& left, Vec4& right);

Vec2& operator-=(Vec2& left, Vec2& right);
Vec3& operator-=(Vec3& left, Vec3& right);
Vec4& operator-=(Vec4& left, Vec4& right);

Vec2& operator*=(Vec2& left, Vec2& right);
Vec3& operator*=(Vec3& left, Vec3& right);
Vec4& operator*=(Vec4& left, Vec4& right);

Vec2& operator/=(Vec2& left, Vec2& right);
Vec3& operator/=(Vec3& left, Vec3& right);
Vec4& operator/=(Vec4& left, Vec4& right);

}
