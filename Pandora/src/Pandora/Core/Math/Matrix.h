#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Math/Vector.h"
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
};

/// <summary>
/// Converts the matrix to colum-major order.
/// </summary>
/// <param name="mat">The matrix.</param>
/// <returns>The column-major matrix.</returns>
Mat4 Transpose(Mat4 mat);

/// <summary>
/// Translates a matrix.
/// </summary>
/// <param name="translation">The translation.</param>
/// <returns>The translated matrix.</returns>
Mat4 Translate(Vec3 translation);

/// <summary>
/// Rotates a matrix.
/// </summary>
/// <param name="angle">The angle in radians.</param>
/// <returns>The rotated matrix.</returns>
Mat4 RotateX(float angle);

/// <summary>
/// Rotates a matrix.
/// </summary>
/// <param name="angle">The angle in radians.</param>
/// <returns>The rotated matrix.</returns>
Mat4 RotateY(float angle);

/// <summary>
/// Rotates a matrix.
/// </summary>
/// <param name="angle">The angle in radians.</param>
/// <returns>The rotated matrix.</returns>
Mat4 RotateZ(float angle);

/// <summary>
/// Scales a matrix.
/// </summary>
/// <param name="scale">The scale.</param>
/// <returns>The scaled matrix.</returns>
Mat4 Scale(Vec3 scale);

/// <summary>
/// Generates an orthogonal projection matrix.
/// </summary>
/// <param name="left">The left-side x-coordinate.</param>
/// <param name="right">The right-side x-coordinate.</param>
/// <param name="bottom">The bottom y-coordinate.</param>
/// <param name="top">The top y-coordinate.</param>
/// <param name="near">The near clipping value.</param>>
/// <param name="top">The far clipping value.</param>
/// <returns>The orthographic matrix.</returns>
Mat4 Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near = 0.01f, f32 far = 10000.0f);

/// <summary>
/// Generates a perspective projection matrix
/// </summary>
/// <param name="fov">The field-of-view in radians.</param>
/// <param name="aspectRatio">The aspect ratio (w/h).</param>
/// <param name="near">The near clipping value.</param>
/// <param name="top">The far clipping value.</param>
/// <returns>The perspective matrix.</returns>
Mat4 Perspective(f32 fov, f32 aspectRatio, f32 near = 0.01f, f32 far = 10000.0f);

/// <summary>
/// Generates a matrix that looks from <c>eye</c> to <c>center</c>.
/// </summary>
/// <param name="eye">The source point.</param>
/// <param name="center">The target point.</param>
/// <param name="up">The up direction.</param>
Mat4 LookAt(Vec3 eye, Vec3 center, Vec3 up);

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
