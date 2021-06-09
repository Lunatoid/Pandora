#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Logging/PrintType.h"

#if defined(PD_LIB)
#include <glm/glm.hpp>
#endif

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

    inline const f32& operator[](const int& i) const {
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

        f32 elements[3];
    };

    inline f32& operator[](const int& i) {
        return elements[i];
    }

    inline const f32& operator[](const int& i) const {
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

        Vec2 xy;
        Vec3 xyz;
        f32 elements[4];
    };

    inline f32& operator[](const int& i) {
        return elements[i];
    }


    inline const f32& operator[](const int& i) const {
        return elements[i];
    }

    inline Vec4 operator-() {
        return Vec4(-x, -y, -z, w);
    }
};

#if defined(PD_LIB)

// pd::Vec* <-> glm::vec*

glm::vec2 V(const Vec2& v);
glm::vec3 V(const Vec3& v);
glm::vec4 V(const Vec4& v);

Vec2 V(const glm::vec2& v);
Vec3 V(const glm::vec3& v);
Vec4 V(const glm::vec4& v);

#endif

/**
 * \param a The first vector.
 * \param b The second vector.
 * \return The dot product of `a` and `b`.
 */
f32 Dot(Vec2 a, Vec2 b);

/**
 * \param a The first vector.
 * \param b The second vector.
 * \return The dot product of `a` and `b`.
 */
f32 Dot(Vec3 a, Vec3 b);

/**
 * \param a The first vector.
 * \param b The second vector.
 * \return The dot product of `a` and `b`.
 */
f32 Dot(Vec4 a, Vec4 b);

/**
 * \param a The first vector.
 * \param b The second vector.
 * \return The cross product of `a` and `b`.
 */
Vec3 Cross(Vec3 a, Vec3 b);

/**
 * \param a The first vector.
 * \param b The second vector.
 * \return The distance between `a` and `b`.
 */
f32 Distance(Vec2 a, Vec2 b);

/**
 * \param a The first vector.
 * \param b The second vector.
 * \return The distance between `a` and `b`.
 */
f32 Distance(Vec3 a, Vec3 b);

/**
 * \param a The first vector.
 * \param b The second vector.
 * \return The distance between `a` and `b`.
 */
f32 Distance(Vec4 a, Vec4 b);

/**
 * \param v The vector.
 * \return The length of the vector.
 */
f32 Magnitude(Vec2 v);

/**
 * \param v The vector.
 * \return The length of the vector.
 */
f32 Magnitude(Vec3 v);

/**
 * \param v The vector.
 * \return The length of the vector.
 */
f32 Magnitude(Vec4 v);

/**
 * \param v The vector.
 * \return The squared length of the vector.
 */
f32 SquareMagnitude(Vec2 v);

/**
 * \param v The vector.
 * \return The squared length of the vector.
 */
f32 SquareMagnitude(Vec3 v);

/**
 * \param v The vector.
 * \return The squared length of the vector.
 */
f32 SquareMagnitude(Vec4 v);

/**
 * \param v The vector.
 * \return The vector as a unit vector.
 */
Vec2 Normalize(Vec2 v);

/**
 * \param v The vector.
 * \return The vector as a unit vector.
 */
Vec3 Normalize(Vec3 v);

/**
 * \param v The vector.
 * \return The vector as a unit vector.
 */
Vec4 Normalize(Vec4 v);

/**
 * \param inDir The incoming direction.
 * \param inNormal The normal plane for the reflection.
 * \return The reflected vector.
 */
Vec2 Reflect(Vec2 inDir, Vec2 inNormal);

/**
 * \param inDir The incoming direction.
 * \param inNormal The normal plane for the reflection.
 * \return The reflected vector.
 */
Vec3 Reflect(Vec3 inDir, Vec3 inNormal);

/**
 * \brief Calculates the angle between `from` and `to`.
 * 
 * \param from The first vector.
 * \param to The second vector.
 * \return The angle in between.
 */
f32 Angle(Vec2 from, Vec2 to);

/**
 * \brief Calculates the angle between `from` and `to`.
 * 
 * \param from The first vector.
 * \param to The second vector.
 * \return The angle in between.
 */
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

Vec2& operator+=(Vec2& left, const Vec2& right);
Vec3& operator+=(Vec3& left, const Vec3& right);
Vec4& operator+=(Vec4& left, const Vec4& right);

Vec2& operator-=(Vec2& left, const Vec2& right);
Vec3& operator-=(Vec3& left, const Vec3& right);
Vec4& operator-=(Vec4& left, const Vec4& right);

Vec2& operator*=(Vec2& left, const Vec2& right);
Vec3& operator*=(Vec3& left, const Vec3& right);
Vec4& operator*=(Vec4& left, const Vec4& right);

Vec2& operator/=(Vec2& left, const Vec2& right);
Vec3& operator/=(Vec3& left, const Vec3& right);
Vec4& operator/=(Vec4& left, const Vec4& right);

}

