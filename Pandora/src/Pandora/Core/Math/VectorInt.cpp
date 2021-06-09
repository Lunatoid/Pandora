#include "VectorInt.h"

#include "Pandora/Core/Math/Math.h"
#include "Pandora/Core/Math/Vector.h"

namespace pd {

Vec2i::operator Vec2() const {
    return Vec2((f32)x, (f32)y);
}

Vec3i::operator Vec3() const {
    return Vec3((f32)x, (f32)y, (f32)z);
}

Vec4i::operator Vec4() const {
    return Vec4((f32)x, (f32)y, (f32)z, (f32)w);
}

Vec3i Cross(Vec3i a, Vec3i b) {
    Vec3i result;

    result.x = (a.y * b.z) - (a.z * b.y);
    result.y = (a.z * b.x) - (a.x * b.z);
    result.y = (a.x * b.y) - (a.y * b.x);

    return result;
}

f32 Distance(Vec2i a, Vec2i b) {
    return Magnitude(a - b);
}

f32 Distance(Vec3i a, Vec3i b) {
    return Magnitude(a - b);
}

f32 Distance(Vec4i a, Vec4i b) {
    return Magnitude(a - b);
}

f32 Magnitude(Vec2i v) {
    return Sqrt((f32)SquareMagnitude(v));
}

f32 Magnitude(Vec3i v) {
    return Sqrt((f32)SquareMagnitude(v));
}

f32 Magnitude(Vec4i v) {
    return Sqrt((f32)SquareMagnitude(v));
}

int SquareMagnitude(Vec2i v) {
    return v.x * v.x + v.y * v.y;
}

int SquareMagnitude(Vec3i v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

int SquareMagnitude(Vec4i v) {
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

//
// Operators
//

bool operator==(const Vec2i& left, const Vec2i& right) {
    return left.x == right.x && left.y == right.y;
}

bool operator==(const Vec3i& left, const Vec3i& right) {
    return left.x == right.x && left.y == right.y && left.z == right.z;
}

bool operator==(const Vec4i& left, const Vec4i& right) {
    return left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;
}

bool operator!=(const Vec2i& left, const Vec2i& right) {
    return !(left == right);
}

bool operator!=(const Vec3i& left, const Vec3i& right) {
    return !(left == right);
}

bool operator!=(const Vec4i& left, const Vec4i& right) {
    return !(left == right);
}

Vec2i operator+(const Vec2i& left, const Vec2i& right) {
    return Vec2i(left.x + right.x, left.y + right.y);
}

Vec3i operator+(const Vec3i& left, const Vec3i& right) {
    return Vec3i(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vec4i operator+(const Vec4i& left, const Vec4i& right) {
    return Vec4i(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
}

Vec2i operator+(const Vec2i& left, int right) {
    return left + Vec2i(right);
}

Vec3i operator+(const Vec3i& left, int right) {
    return left + Vec3i(right);
}

Vec4i operator+(const Vec4i& left, int right) {
    return left + Vec4i(right);
}

Vec2i operator-(const Vec2i& left, const Vec2i& right) {
    return Vec2i(left.x - right.x, left.y - right.y);
}

Vec3i operator-(const Vec3i& left, const Vec3i& right) {
    return Vec3i(left.x - right.x, left.y - right.y, left.z - right.z);
}

Vec4i operator-(const Vec4i& left, const Vec4i& right) {
    return Vec4i(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
}

Vec2i operator-(const Vec2i& left, int right) {
    return left - Vec2i(right);
}

Vec3i operator-(const Vec3i& left, int right) {
    return left - Vec3i(right);
}

Vec4i operator-(const Vec4i& left, int right) {
    return left - Vec4i(right);
}

Vec2i operator*(const Vec2i& left, const Vec2i& right) {
    return Vec2i(left.x * right.x, left.y * right.y);
}

Vec3i operator*(const Vec3i& left, const Vec3i& right) {
    return Vec3i(left.x * right.x, left.y * right.y, left.z * right.z);
}

Vec4i operator*(const Vec4i& left, const Vec4i& right) {
    return Vec4i(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
}

Vec2i operator*(const Vec2i& left, int right) {
    return left * Vec2i(right);
}

Vec3i operator*(const Vec3i& left, int right) {
    return left * Vec3i(right);
}

Vec4i operator*(const Vec4i& left, int right) {
    return left * Vec4i(right);
}

Vec2i operator/(const Vec2i& left, const Vec2i& right) {
    return Vec2i(left.x / right.x, left.y / right.y);
}

Vec3i operator/(const Vec3i& left, const Vec3i& right) {
    return Vec3i(left.x / right.x, left.y / right.y, left.z / right.z);
}

Vec4i operator/(const Vec4i& left, const Vec4i& right) {
    return Vec4i(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
}

Vec2i operator/(const Vec2i& left, int right) {
    return left / Vec2i(right);
}

Vec3i operator/(const Vec3i& left, int right) {
    return left / Vec3i(right);
}

Vec4i operator/(const Vec4i& left, int right) {
    return left / Vec4i(right);
}

Vec2i operator%(const Vec2i& left, const Vec2i& right) {
    return Vec2i(left.x % right.x, left.y % right.y);
}

Vec3i operator%(const Vec3i& left, const Vec3i& right) {
    return Vec3i(left.x % right.x, left.y % right.y, left.z % right.z);
}

Vec4i operator%(const Vec4i& left, const Vec4i& right) {
    return Vec4i(left.x % right.x, left.y % right.y, left.z % right.z, left.w % right.w);
}

Vec2i operator%(const Vec2i& left, int right) {
    return left % Vec2i(right);
}

Vec3i operator%(const Vec3i& left, int right) {
    return left % Vec3i(right);
}

Vec4i operator%(const Vec4i& left, int right) {
    return left % Vec4i(right);
}

Vec2i& operator+=(Vec2i& left, Vec2i& right) {
    return (left = left + right);
}

Vec3i& operator+=(Vec3i& left, Vec3i& right) {
    return (left = left + right);
}

Vec4i& operator+=(Vec4i& left, Vec4i& right) {
    return (left = left + right);
}

Vec2i& operator-=(Vec2i& left, Vec2i& right) {
    return (left = left - right);
}

Vec3i& operator-=(Vec3i& left, Vec3i& right) {
    return (left = left - right);
}

Vec4i& operator-=(Vec4i& left, Vec4i& right) {
    return (left = left - right);
}

Vec2i& operator*=(Vec2i& left, Vec2i& right) {
    return (left = left * right);
}

Vec3i& operator*=(Vec3i& left, Vec3i& right) {
    return (left = left * right);
}

Vec4i& operator*=(Vec4i& left, Vec4i& right) {
    return (left = left * right);
}

Vec2i& operator/=(Vec2i& left, Vec2i& right) {
    return (left = left / right);
}

Vec3i& operator/=(Vec3i& left, Vec3i& right) {
    return (left = left / right);
}

Vec4i& operator/=(Vec4i& left, Vec4i& right) {
    return (left = left / right);
}

}
