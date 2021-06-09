#pragma once
#include "Pandora/Core/Math/Vector.h"

#if defined(PD_LIB)
#include <glm/gtc/quaternion.hpp>
#endif

namespace pd {

struct Quat {
    Quat() :
        w(1.0f), x(0.0f), y(0.0f), z(0.0f) {}

    Quat(f32 eulerX, f32 eulerY, f32 eulerZ);

    Quat(f32 w, f32 x, f32 y, f32 z) :
        w(w), x(x), y(y), z(z) {}

    union {
        struct {
            f32 w;
            f32 x;
            f32 y;
            f32 z;
        };

        Vec4 wxyz;

        f32 elements[4];
    };

};

#if defined(PD_LIB)

// pd::Quat <-> glm::quaternion

glm::quat Q(const Quat& q);

Quat Q(const glm::quat& q);

#endif

/**
 * \brief Creates a quaternion from euler angles as radians.
 * 
 * \param eulerX The euler X angle.
 * \param eulerY The euler Y angle.
 * \param eulerZ The euler Z angle.
 * \return The resulting quaternion.
 */
Quat FromEuler(f32 eulerX, f32 eulerY, f32 eulerZ);

/**
 * \brief Creates a quaternion from euler angles as degrees.
 * 
 * \param eulerX The euler X angle.
 * \param eulerY The euler Y angle.
 * \param eulerZ The euler Z angle.
 * \return The resulting quaternion.
 */
Quat FromEulerDegrees(f32 eulerX, f32 eulerY, f32 eulerZ);

/**
 * \param q The quaternion.
 * \return The quaternion in euler angles as radians.
 * X as pitch. Y as yaw. Z as roll.
 */
Vec3 ToEuler(Quat q);

/**
 * \param q The quaternion.
 * \return The quaternion in euler angles as degrees.
 * X as pitch. Y as yaw. Z as roll.
 */
Vec3 ToEulerDegrees(Quat q);

/**
 * \param q The quaternion.
 * \return The quaternion with a magnitude of 1.
 */
Quat Normalize(Quat q);

/**
 * \param q The quaternion.
 * \return The inverse of the quaternion.
 */
Quat Inverse(Quat q);

/**
 * \param a The first quaternion.
 * \param b The second quaternion.
 * \return The dot product of `a` and `b`.
 */
f32 Dot(Quat a, Quat b);

/**
 * \param a The first quaternion.
 * \param b The second quaternion.
 * \return The cross product of `a` and `b`.
 */
Quat Cross(Quat a, Quat b);

/**
 * \brief Creates a rotation with the specified forward and up directions.
 * 
 * \param view The view direction.
 * \param up The up direction.
 * \return 
 */
Quat LookAt(Vec3 view, Vec3 up = Vec3(0.0f, 1.0f, 0.0f));

/**
 * \brief Rotates a quaternion from a vector of 3 components axis
 * and an angle expressed in degrees.
 * 
 * \param q The quaternion.
 * \param angle The angle in degrees.
 * \param axis The axis.
 * \return The rotated quaternion.
 */
Quat Rotate(Quat q, f32 angle, Vec3 axis);

/**
 * \brief Creates a quaternion from an angle and a normalized axis.
 * 
 * \param angle The angle in degrees.
 * \param axis The normalized axis.
 * \return The resulting quaternion.
 */
Quat AngleAxis(f32 angle, Vec3 axis);

/**
 * \param a The first quaternion.
 * \param b The second quaternion.
 * \param t The interpolated point. Clamps between 0.0 and 1.0.
 * \return The interpolated result.
 */
Quat Lerp(Quat a, Quat b, f32 t);

/**
 * \brief Spherical interpolation between two quaternions.
 * 
 * \param a The first quaternion.
 * \param b The second quaternion/
 * \param t The interpolation point. Clamps between 0.0 and 1.0.
 * \return The spherically interpolated result.
 */
Quat Slerp(Quat a, Quat b, f32 t);

// Operators

bool operator==(const Quat& left, const Quat& right);

bool operator!=(const Quat& left, const Quat& right);

Quat operator*(const Quat& left, const Quat& right);

}
