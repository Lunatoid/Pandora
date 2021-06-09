#include "Quat.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Pandora/Core/Math/Math.h"

namespace pd {

Quat::Quat(f32 eulerX, f32 eulerY, f32 eulerZ) {
    Quat euler = FromEuler(eulerX, eulerY, eulerZ);

    w = euler.w;
    x = euler.x;
    y = euler.y;
    z = euler.z;
}

glm::quat Q(const Quat& q) {
    return glm::quat(q.w, q.x, q.y, q.z);
}

Quat Q(const glm::quat& q) {
    return Quat(q.w, q.x, q.y, q.z);
}

Quat FromEuler(f32 eulerX, f32 eulerY, f32 eulerZ) {
    return Q(glm::quat(glm::vec3(eulerX, eulerY, eulerZ)));
}

Quat FromEulerDegrees(f32 eulerX, f32 eulerY, f32 eulerZ) {
    return FromEuler(ToRadians(eulerX), ToRadians(eulerY), ToRadians(eulerZ));
}

Vec3 ToEuler(Quat q) {
    return V(glm::eulerAngles(Q(q)));
}

Vec3 ToEulerDegrees(Quat q) {
    Vec3 radians = ToEuler(q);
    return Vec3(ToDegrees(radians.x), ToDegrees(radians.y), ToDegrees(radians.z));
}

Quat Normalize(Quat q) {
    return Q(glm::normalize(Q(q)));
}

Quat Inverse(Quat q) {
    return Q(glm::inverse(Q(q)));
}

f32 Dot(Quat a, Quat b) {
    return glm::dot(Q(a), Q(b));
}

Quat Cross(Quat a, Quat b) {
    return Q(glm::cross(Q(a), Q(b)));
}

Quat LookAt(Vec3 view, Vec3 up) {
    return Q(glm::quatLookAt(V(view), V(up)));
}

Quat Rotate(Quat q, f32 angle, Vec3 axis) {
    return Q(glm::rotate(Q(q), angle, V(axis)));
}

Quat AngleAxis(f32 angle, Vec3 axis) {
    return Q(glm::angleAxis(angle, V(axis)));
}

Quat Lerp(Quat a, Quat b, f32 t) {
    return Q(glm::lerp(Q(a), Q(b), t));
}

Quat Slerp(Quat a, Quat b, f32 t) {
    return Q(glm::slerp(Q(a), Q(b), t));
}

// Operators

bool operator==(const Quat& left, const Quat& right) {
    return Q(left) == Q(right);
}

bool operator!=(const Quat& left, const Quat& right) {
    return Q(left) != Q(right);
}

Quat operator*(const Quat& left, const Quat& right) {
    return Q(Q(left) * Q(right));
}

}
