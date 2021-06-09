#include "Vector.h"

#include "Pandora/Core/Math/Math.h"

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace pd {

glm::vec2 V(const Vec2& v) {
    return glm::vec2(v.x, v.y);
}

glm::vec3 V(const Vec3& v) {
    return glm::vec3(v.x, v.y, v.z);
}

glm::vec4 V(const Vec4& v) {
    return glm::vec4(v.x, v.y, v.z, v.w);
}

Vec2 V(const glm::vec2& v) {
    return Vec2(v.x, v.y);
}

Vec3 V(const glm::vec3& v) {
    return Vec3(v.x, v.y, v.z);
}

Vec4 V(const glm::vec4& v) {
    return Vec4(v.x, v.y, v.z, v.w);
}

f32 Dot(Vec2 a, Vec2 b) {
    return glm::dot(V(a), V(b));
}

f32 Dot(Vec3 a, Vec3 b) {
    return glm::dot(V(a), V(b));
}

f32 Dot(Vec4 a, Vec4 b) {
    return glm::dot(V(a), V(b));
}

Vec3 Cross(Vec3 a, Vec3 b) {
    return V(glm::cross(V(a), V(b)));
}

f32 Distance(Vec2 a, Vec2 b) {
    return glm::distance(V(a), V(b));
}

f32 Distance(Vec3 a, Vec3 b) {
    return glm::distance(V(a), V(b));
}

f32 Distance(Vec4 a, Vec4 b) {
    return glm::distance(V(a), V(b));
}

f32 Magnitude(Vec2 v) {
    return glm::length(V(v));
}

f32 Magnitude(Vec3 v) {
    return glm::length(V(v));
}

f32 Magnitude(Vec4 v) {
    return glm::length(V(v));
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
    return V(glm::normalize(V(v)));
}

Vec3 Normalize(Vec3 v) {
    return V(glm::normalize(V(v)));
}

Vec4 Normalize(Vec4 v) {
    return V(glm::normalize(V(v)));
}

Vec2 Reflect(Vec2 inDir, Vec2 inNormal) {
    return V(glm::reflect(V(inDir), V(inNormal)));
}

Vec3 Reflect(Vec3 inDir, Vec3 inNormal) {
    return V(glm::reflect(V(inDir), V(inNormal)));
}

f32 Angle(Vec2 from, Vec2 to) {
    return glm::angle(V(from), V(to));
}

f32 Angle(Vec3 from, Vec3 to) {
    return glm::angle(V(from), V(to));
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

Vec2& operator+=(Vec2& left, const Vec2& right) {
    return (left = left + right);
}

Vec3& operator+=(Vec3& left, const Vec3& right) {
    return (left = left + right);
}

Vec4& operator+=(Vec4& left, const Vec4& right) {
    return (left = left + right);
}

Vec2& operator-=(Vec2& left, const Vec2& right) {
    return (left = left - right);
}

Vec3& operator-=(Vec3& left, const Vec3& right) {
    return (left = left - right);
}

Vec4& operator-=(Vec4& left, const Vec4& right) {
    return (left = left - right);
}

Vec2& operator*=(Vec2& left, const Vec2& right) {
    return (left = left * right);
}

Vec3& operator*=(Vec3& left, const Vec3& right) {
    return (left = left * right);
}

Vec4& operator*=(Vec4& left, const Vec4& right) {
    return (left = left * right);
}

Vec2& operator/=(Vec2& left, const Vec2& right) {
    return (left = left / right);
}

Vec3& operator/=(Vec3& left, const Vec3& right) {
    return (left = left / right);
}

Vec4& operator/=(Vec4& left, const Vec4 right) {
    return (left = left / right);
}

}

