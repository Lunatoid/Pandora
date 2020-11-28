#include "Vector.h"

#include "Pandora/Core/Math/Math.h"

namespace pd {

f32 Dot(Vec2 a, Vec2 b) {
    return (a.x * b.x) + (a.y * b.y);
}

f32 Dot(Vec3 a, Vec3 b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

f32 Dot(Vec4 a, Vec4 b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

Vec3 Cross(Vec3 a, Vec3 b) {
    Vec3 result;

    result.x = (a.y * b.z) - (b.y * a.z);
    result.y = (a.z * b.x) - (b.z * a.x);
    result.z = (a.x * b.y) - (b.x * a.y);

    return result;
}

f32 Distance(Vec2 a, Vec2 b) {
    f32 x = a.x - b.x;
    f32 y = a.y - b.y;

    return Sqrt(x * x + y * y);
}

f32 Distance(Vec3 a, Vec3 b) {
    f32 x = a.x - b.x;
    f32 y = a.y - b.y;
    f32 z = a.z - b.z;

    return Sqrt(x * x + y * y + z * z);
}

Vec4 Distance(Vec4 a, Vec4 b) {
    return Magnitude(a - b);
}

f32 Magnitude(Vec2 v) {
    return Sqrt(SquareMagnitude(v));
}

f32 Magnitude(Vec3 v) {
    return Sqrt(SquareMagnitude(v));
}

f32 Magnitude(Vec4 v) {
    return Sqrt(SquareMagnitude(v));
}

f32 SquareMagnitude(Vec2 v) {
    return v.x * v.x + v.y * v.y;
}

f32 SquareMagnitude(Vec3 v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

f32 SquareMagnitude(Vec4 v) {
    return Dot(v, v);
}

Vec2 Normalize(Vec2 v) {
    float m = 1.0f / Magnitude(v);
    return v * Vec2((isnan(m) || isinf(m)) ? 0.0f : m);
}

Vec3 Normalize(Vec3 v) {
    float m = 1.0f / Magnitude(v);
    return v * Vec3((isnan(m) || isinf(m)) ? 0.0f : m);
}

Vec4 Normalize(Vec4 v) {
    float m = 1.0f / Magnitude(v);
    return v * Vec4((isnan(m) || isinf(m)) ? 0.0f : m);
}

Vec2 Reflect(Vec2 inDir, Vec2 inNormal) {
    f32 num = -2.0f * Dot(inNormal, inDir);
    return Vec2(num) * inNormal + inDir;
}

Vec3 Reflect(Vec3 inDir, Vec3 inNormal) {
    f32 num = -2.0f * Dot(inNormal, inDir);
    return Vec3(num) * inNormal + inDir;
}

f32 Angle(Vec2 from, Vec2 to) {
    f32 num = Sqrt(SquareMagnitude(from) * SquareMagnitude(to));
    f32 result = Clamp(Dot(from, to) / num, -1.0f, 1.0f);

    result = ToDegrees(Acos(result));

    return result;
}

f32 Angle(Vec3 from, Vec3 to) {
    f32 num = Sqrt(SquareMagnitude(from) * SquareMagnitude(to));
    f32 result = Clamp(Dot(from, to) / num, -1.0f, 1.0f);

    result = ToDegrees(Acos(result));

    return result;
}

//
// Operators
//

bool operator==(const Vec2& left, const Vec2& right) {
    return left.x == right.x && left.y == right.y;
}

bool operator==(const Vec3& left, const Vec3& right) {
    return left.x == right.x && left.y == right.y && left.z == right.z;
}

bool operator==(const Vec4& left, const Vec4& right) {
    return left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;
}

bool operator!=(const Vec2& left, const Vec2& right) {
    return !(left == right);
}

bool operator!=(const Vec3& left, const Vec3& right) {
    return !(left == right);
}

bool operator!=(const Vec4& left, const Vec4& right) {
    return !(left == right);
}

Vec2 operator+(const Vec2& left, const Vec2& right) {
    return Vec2(left.x + right.x, left.y + right.y);
}

Vec3 operator+(const Vec3& left, const Vec3& right) {
    return Vec3(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vec4 operator+(const Vec4& left, const Vec4& right) {
    return Vec4(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
}

Vec2 operator+(const Vec2& left, f32 right) {
    return left + Vec2(right);
}

Vec3 operator+(const Vec3& left, f32 right) {
    return left + Vec3(right);
}

Vec4 operator+(const Vec4& left, f32 right) {
    return left + Vec4(right);
}

Vec2 operator-(const Vec2& left, const Vec2& right) {
    return Vec2(left.x - right.x, left.y - right.y);
}

Vec3 operator-(const Vec3& left, const Vec3& right) {
    return Vec3(left.x - right.x, left.y - right.y, left.z - right.z);
}

Vec4 operator-(const Vec4& left, const Vec4& right) {
    return Vec4(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
}

Vec2 operator-(const Vec2& left, f32 right) {
    return left - Vec2(right);
}

Vec3 operator-(const Vec3& left, f32 right) {
    return left - Vec3(right);
}

Vec4 operator-(const Vec4& left, f32 right) {
    return left - Vec4(right);
}

Vec2 operator*(const Vec2& left, const Vec2& right) {
    return Vec2(left.x * right.x, left.y * right.y);
}

Vec3 operator*(const Vec3& left, const Vec3& right) {
    return Vec3(left.x * right.x, left.y * right.y, left.z * right.z);
}

Vec4 operator*(const Vec4& left, const Vec4& right) {
    return Vec4(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
}

Vec2 operator*(const Vec2& left, f32 right) {
    return left * Vec2(right);
}

Vec3 operator*(const Vec3& left, f32 right) {
    return left * Vec3(right);
}

Vec4 operator*(const Vec4& left, f32 right) {
    return left * Vec4(right);
}

Vec2 operator/(const Vec2& left, const Vec2& right) {
    return Vec2(left.x / right.x, left.y / right.y);
}

Vec3 operator/(const Vec3& left, const Vec3& right) {
    return Vec3(left.x / right.x, left.y / right.y, left.z / right.z);
}

Vec4 operator/(const Vec4& left, const Vec4& right) {
    return Vec4(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
}

Vec2 operator/(const Vec2& left, f32 right) {
    return left / Vec2(right);
}

Vec3 operator/(const Vec3& left, f32 right) {
    return left / Vec2(right);
}

Vec4 operator/(const Vec4& left, f32 right) {
    return left / Vec2(right);
}

Vec2& operator+=(Vec2& left, Vec2& right) {
    return (left = left + right);
}

Vec3& operator+=(Vec3& left, Vec3& right) {
    return (left = left + right);
}

Vec4& operator+=(Vec4& left, Vec4& right) {
    return (left = left + right);
}

Vec2& operator-=(Vec2& left, Vec2& right) {
    return (left = left - right);
}

Vec3& operator-=(Vec3& left, Vec3& right) {
    return (left = left - right);
}

Vec4& operator-=(Vec4& left, Vec4& right) {
    return (left = left - right);
}

Vec2& operator*=(Vec2& left, Vec2& right) {
    return (left = left * right);
}

Vec3& operator*=(Vec3& left, Vec3& right) {
    return (left = left * right);
}

Vec4& operator*=(Vec4& left, Vec4& right) {
    return (left = left * right);
}

Vec2& operator/=(Vec2& left, Vec2& right) {
    return (left = left / right);
}

Vec3& operator/=(Vec3& left, Vec3& right) {
    return (left = left / right);
}

Vec4& operator/=(Vec4& left, Vec4& right) {
    return (left = left / right);
}

}
