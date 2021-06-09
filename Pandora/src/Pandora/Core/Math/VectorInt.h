#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Math/Vector.h"
#include "Pandora/Core/Logging/PrintType.h"

namespace pd {

struct Vec2i {
    Vec2i(int v = 0) :
        x(v), y(v) {}

    Vec2i(int x, int y) :
        x(x), y(y) {}

    union {
        struct {
            int x;
            int y;
        };

        int elements[2];
    };

    inline int& operator[](const int& i) {
        return elements[i];
    }

    inline Vec2i operator-() {
        return Vec2i(-x, -y);
    }

    operator Vec2() const;
};

struct Vec3i {
    Vec3i(int v = 0) :
        x(v), y(v), z(v) {}

    Vec3i(int x, int y, int z) :
        x(x), y(y), z(z) {}

    Vec3i(Vec2i v2) :
        Vec3i(v2.x, v2.y, 0) {}

    union {
        struct {
            int x;
            int y;
            int z;
        };

        Vec2i xy;

        int elements[3];
    };

    inline int& operator[](const int& i) {
        return elements[i];
    }

    inline Vec3i operator-() {
        return Vec3i(-x, -y, -z);
    }

    operator Vec3() const;
};

struct Vec4i {
    Vec4i(int v = 0) :
        x(v), y(v), z(v), w(v) {}

    Vec4i(int x, int y, int z, int w) :
        x(x), y(y), z(z), w(w) {}

    Vec4i(Vec2i v2) :
        Vec4i(v2.x, v2.y, 0, 0) {}

    Vec4i(Vec3i v3) :
        Vec4i(v3.x, v3.y, v3.z, 0) {}

    union {
        struct {
            int x;
            int y;
            int z;
            int w;
        };

        Vec2i xy;
        Vec3i xyz;

        int elements[4];
    };

    inline int& operator[](const int& i) {
        return elements[i];
    }

    inline Vec4i operator-() {
        return Vec4i(-x, -y, -z, w);
    }

    operator Vec4() const;
};

// Operations

/**
 * \param a The first vector.
 * \param b The second vector.
 * \return The distance between `a` and `b`.
 */
f32 Distance(Vec2i a, Vec2i b);

/**
 * \param a The first vector.
 * \param b The second vector.
 * \return The distance between `a` and `b`.
 */
f32 Distance(Vec3i a, Vec3i b);

/**
 * \param a The first vector.
 * \param b The second vector.
 * \return The distance between `a` and `b`.
 */
f32 Distance(Vec4i a, Vec4i b);

/**
 * \param v The vector.
 * \return The length of the vector.
 */
f32 Magnitude(Vec2i v);

/**
 * \param v The vector.
 * \return The length of the vector.
 */
f32 Magnitude(Vec3i v);

/**
 * \param v The vector.
 * \return The length of the vector.
 */
f32 Magnitude(Vec4i v);

/**
 * \param v The vector.
 * \return The squared length of the vector.
 */
int SquareMagnitude(Vec2i v);

/**
 * \param v The vector.
 * \return The squared length of the vector.
 */
int SquareMagnitude(Vec3i v);

/**
 * \param v The vector.
 * \return The squared length of the vector.
 */
int SquareMagnitude(Vec4i v);

// Print

template<>
inline void PrintType(Vec2i& type, FormatInfo& info) {
    PrintfToStream(info.output, "{%s", (info.pretty) ? "\n    " : " ");
    PrintType(type.x, info);
    PrintfToStream(info.output, ",%s", (info.pretty) ? "\n    " : " ");
    PrintType(type.y, info);
    PrintfToStream(info.output, "%s}", (info.pretty) ? "\n" : " ");
}

template<>
inline void PrintType(Vec3i& type, FormatInfo& info) {
    PrintfToStream(info.output, "{%s", (info.pretty) ? "\n    " : " ");
    PrintType(type.x, info);
    PrintfToStream(info.output, ",%s", (info.pretty) ? "\n    " : " ");
    PrintType(type.y, info);
    PrintfToStream(info.output, ",%s", (info.pretty) ? "\n    " : " ");
    PrintType(type.z, info);
    PrintfToStream(info.output, "%s}", (info.pretty) ? "\n" : " ");
}

template<>
inline void PrintType(Vec4i& type, FormatInfo& info) {
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

bool operator==(const Vec2i& left, const Vec2i& right);
bool operator==(const Vec3i& left, const Vec3i& right);
bool operator==(const Vec4i& left, const Vec4i& right);

bool operator!=(const Vec2i& left, const Vec2i& right);
bool operator!=(const Vec3i& left, const Vec3i& right);
bool operator!=(const Vec4i& left, const Vec4i& right);

Vec2i operator+(const Vec2i& left, const Vec2i& right);
Vec3i operator+(const Vec3i& left, const Vec3i& right);
Vec4i operator+(const Vec4i& left, const Vec4i& right);

Vec2i operator+(const Vec2i& left, int right);
Vec3i operator+(const Vec3i& left, int right);
Vec4i operator+(const Vec4i& left, int right);

Vec2i operator-(const Vec2i& left, const Vec2i& right);
Vec3i operator-(const Vec3i& left, const Vec3i& right);
Vec4i operator-(const Vec4i& left, const Vec4i& right);

Vec2i operator-(const Vec2i& left, int right);
Vec3i operator-(const Vec3i& left, int right);
Vec4i operator-(const Vec4i& left, int right);

Vec2i operator*(const Vec2i& left, const Vec2i& right);
Vec3i operator*(const Vec3i& left, const Vec3i& right);
Vec4i operator*(const Vec4i& left, const Vec4i& right);

Vec2i operator*(const Vec2i& left, int right);
Vec3i operator*(const Vec3i& left, int right);
Vec4i operator*(const Vec4i& left, int right);

Vec2i operator/(const Vec2i& left, const Vec2i& right);
Vec3i operator/(const Vec3i& left, const Vec3i& right);
Vec4i operator/(const Vec4i& left, const Vec4i& right);

Vec2i operator/(const Vec2i& left, int right);
Vec3i operator/(const Vec3i& left, int right);
Vec4i operator/(const Vec4i& left, int right);

Vec2i operator%(const Vec2i& left, const Vec2i& right);
Vec3i operator%(const Vec3i& left, const Vec3i& right);
Vec4i operator%(const Vec4i& left, const Vec4i& right);

Vec2i operator%(const Vec2i& left, int right);
Vec3i operator%(const Vec3i& left, int right);
Vec4i operator%(const Vec4i& left, int right);

Vec2i& operator+=(Vec2i& left, Vec2i& right);
Vec3i& operator+=(Vec3i& left, Vec3i& right);
Vec4i& operator+=(Vec4i& left, Vec4i& right);

Vec2i& operator-=(Vec2i& left, Vec2i& right);
Vec3i& operator-=(Vec3i& left, Vec3i& right);
Vec4i& operator-=(Vec4i& left, Vec4i& right);

Vec2i& operator*=(Vec2i& left, Vec2i& right);
Vec3i& operator*=(Vec3i& left, Vec3i& right);
Vec4i& operator*=(Vec4i& left, Vec4i& right);

Vec2i& operator/=(Vec2i& left, Vec2i& right);
Vec3i& operator/=(Vec3i& left, Vec3i& right);
Vec4i& operator/=(Vec4i& left, Vec4i& right);
}
