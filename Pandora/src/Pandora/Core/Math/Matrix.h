#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Math/Vector.h"
#include "Pandora/Core/Math/Quat.h"
#include "Pandora/Core/Logging/PrintType.h"

namespace pd {

struct Mat4 {
    Mat4(f32 diagonal = 0.0f) {
        elements[0][0] = diagonal;
        elements[1][1] = diagonal;
        elements[2][2] = diagonal;
        elements[3][3] = diagonal;
    }

    Vec4 elements[4];

    Vec4& operator[](const int& index) {
        return elements[index];
    }

    const Vec4& operator[](const int& index) const {
        return elements[index];
    }
};

#if defined(PD_LIB)

// glm::mat4 <-> Mat4

glm::mat4 M(const Mat4& m);

Mat4 M(const glm::mat4& m);

#endif

/**
 * \param mat The matrix.
 * \return The transpose of the matrix.
 */
Mat4 Transpose(Mat4 mat);

/**
 * \param mat The matrix.
 * \return The inverse of the matrix.
 */
Mat4 Inverse(Mat4 mat);

/**
 * \brief Creates a translation matrix.
 * 
 * \param translation The translation.
 * \return The resulting matrix.
 */
Mat4 Translate(Vec3 translation);

/// <summary>
/// Rotates a matrix.
/// </summary>
/// <param name="angle">The angle in radians.</param>
/// <returns>The rotated matrix.</returns>

/**
 * \brief Creates a rotation matrix around the specified axis.
 * 
 * \param angle The angle in radians.
 * \param axis The axis.
 * \return The resulting matrix.
 */
Mat4 Rotate(f32 angle, Vec3 axis);

/**
 * \brief Creates a scale matrix.
 * 
 * \param scale The scale.
 * \return The resulting matrix.
 */
Mat4 Scale(Vec3 scale);

/**
 * \brief Creates an orthogonal projection matrix.
 * 
 * \param left The left-side x-coordinate.
 * \param right The right-side x-coordinate.
 * \param bottom The bottom y-coordinate.
 * \param top The top y-coordinate.
 * \param near The near clipping plane.
 * \param far The far clipping plane.
 * \return The resulting matrix.
 */
Mat4 Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near = 0.01f, f32 far = 10000.0f);

/**
 * \brief Creates a perspective projection matrix.
 * 
 * \param fov The field-of-view in radians.
 * \param aspectRatio The aspect ratio.
 * \param near The near clipping plane.
 * \param far The far clipping plane.
 * \return The resulting matrix.
 */
Mat4 Perspective(f32 fov, f32 aspectRatio, f32 near = 0.01f, f32 far = 10000.0f);

/**
 * \brief Creates a matrix that looks from `eye` to `center`.
 * 
 * \param eye The source point.
 * \param center The target point.
 * \param up The up direction.
 * \return The resulting matrix.
 */
Mat4 LookAt(Vec3 eye, Vec3 center, Vec3 up);

/**
 * \brief Decomposes a matrix into it's seperate components.
 * 
 * \param mat The matrix.
 * \param scale The scale output. Optional.
 * \param orientation The orientation output. Optional.
 * \param translation The translation output. Optional.
 * \param skew The skew output. Optional.
 * \param perspective The perspective output. Optional.
 * \return Whether or not it decomposed successfully.
 */
bool Decompose(Mat4 mat, Vec3* scale = nullptr, Quat* orientation = nullptr,
               Vec3* translation = nullptr, Vec3* skew = nullptr, Vec4* perspective = nullptr);

/**
 * \param q The quaternion.
 * \return The rotation matrix.
 */
Mat4 ToMat4(Quat q);

// Print

template<>
inline void PrintType(Mat4& type, FormatInfo& info) {
    bool wasPretty = info.pretty;
    info.pretty = false;

    PrintfToStream(info.output, "{%s", (wasPretty) ? "\n    " : " ");
    PrintType(type.elements[0], info);
    PrintfToStream(info.output, ",%s", (wasPretty) ? "\n    " : " ");
    PrintType(type.elements[1], info);
    PrintfToStream(info.output, ",%s", (wasPretty) ? "\n    " : " ");
    PrintType(type.elements[2], info);
    PrintfToStream(info.output, ",%s", (wasPretty) ? "\n    " : " ");
    PrintType(type.elements[3], info);
    PrintfToStream(info.output, "%s}", (wasPretty) ? "\n" : " ");

    info.pretty = wasPretty; // We don't really need to do this
}

// Operators

Mat4 operator+(Mat4 left, Mat4 right);
Mat4 operator-(Mat4 left, Mat4 right);

Mat4 operator*(Mat4 left, Mat4 right);
Mat4 operator*(Mat4 left, float scalar);
Vec4 operator*(Mat4 left, Vec4 vec);

Mat4 operator/(Mat4 left, float scalar);

Mat4& operator+=(Mat4& left, Mat4& right);
Mat4& operator-=(Mat4& left, Mat4& right);
Mat4& operator*=(Mat4& left, Mat4 right);
Mat4& operator*=(Mat4& left, float right);
Mat4& operator/=(Mat4& left, float right);

bool operator==(Mat4& left, Mat4& right);
bool operator!=(Mat4& left, Mat4& right);

}
