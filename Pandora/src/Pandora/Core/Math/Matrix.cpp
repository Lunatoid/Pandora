#include "Matrix.h"

#include "Pandora/Core/Math/Math.h"

namespace pd {

Mat4 Transpose(Mat4 mat) {
    Mat4 result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result[r][c] = mat[c][r];
        }
    }

    return result;
}

Mat4 Translate(Vec3 translation) {
    Mat4 result(1.0f);

    result[3][0] = translation.x;
    result[3][1] = translation.y;
    result[3][2] = translation.z;

    return result;
}

Mat4 RotateX(float angle) {
    Mat4 result(1.0f);

    f32 rads = ToRadians(angle);

    f32 sin = Sin(angle);
    f32 cos = Cos(angle);

    result[1][1] = cos;
    result[1][2] = sin;
    result[2][1] = -sin;
    result[2][2] = cos;

    return result;
}

Mat4 RotateY(float angle) {
    Mat4 result(1.0f);

    f32 rads = ToRadians(angle);

    f32 sin = Sin(angle);
    f32 cos = Cos(angle);

    result[0][0] = cos;
    result[0][2] = -sin;
    result[2][0] = sin;
    result[2][2] = cos;

    return result;
}

Mat4 RotateZ(float angle) {
    Mat4 result(1.0f);

    f32 sin = Sin(angle);
    f32 cos = Cos(angle);

    result[0][0] = cos;
    result[0][1] = sin;
    result[1][0] = -sin;
    result[1][1] = cos;

    return result;
}

Mat4 Scale(Vec3 scale) {
    Mat4 result(1.0f);

    result[0][0] = scale.x;
    result[1][1] = scale.y;
    result[2][2] = scale.z;

    return result;
}

Mat4 Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
    Mat4 result(1.0f);

    result[0][0] = 2.0f / (right - left);
    result[1][1] = 2.0f / (top - bottom);
    result[2][2] = 2.0f / (near - far);

    result[3][0] = (right + left) / (right - left);
    result[3][1] = (top + bottom) / (top - bottom);
    result[3][2] = (near + far) / (near - far);

    return result;
}

Mat4 Perspective(f32 fov, f32 aspectRatio, f32 near, f32 far) {
    f32 tanHalf = Tan(fov * 0.5f);

    Mat4 result(0.0f);
    result[0][0] = 1.0f / (aspectRatio * tanHalf);
    result[1][1] = 1.0f / tanHalf;
    result[2][2] = -(far + near) / (far - near);
    result[2][3] = -1.0f;
    result[3][2] = -(2.0f * far * near) / (far - near);

    return result;
}

Mat4 LookAt(Vec3 eye, Vec3 center, Vec3 up) {
    Vec3 f = Normalize(center - eye);
    Vec3 s = Normalize(Cross(f, up));
    Vec3 u = Cross(s, f);

    Mat4 result(1.0f);
    result[0][0] = s.x;
    result[1][0] = s.y;
    result[2][0] = s.z;

    result[0][1] = u.x;
    result[1][1] = u.y;
    result[2][1] = u.z;

    result[0][2] = -f.x;
    result[1][2] = -f.y;
    result[2][2] = -f.z;

    result[3][0] = -Dot(s, eye);
    result[3][1] = -Dot(u, eye);
    result[3][2] =  Dot(f, eye);

    return result;
}

Mat4 operator+(Mat4 left, Mat4 right) {
    Mat4 result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result[c][r] = left[c][r] + right[c][r];
        }
    }

    return result;
}

Mat4 operator-(Mat4 left, Mat4 right) {
    Mat4 result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result[c][r] = left[c][r] - right[c][r];
        }
    }

    return result;
}

Mat4 operator*(Mat4 left, Mat4 right) {
    Mat4 result;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            result[c][r] = 0;

            for (int i = 0; i < 4; i++) {
                result[c][r] += left[i][r] * right[c][i];
            }
        }
    }


    return result;
}

Mat4 operator*(Mat4 left, float scalar) {
    Mat4 result;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            result[c][r] = 0;

            for (int i = 0; i < 4; i++) {
                result[c][r] += left[i][r] * scalar;
            }
        }
    }


    return result;
}

Vec4 operator*(Mat4 left, Vec4 vec) {
    Vec4 result;

    for (int r = 0; r < 4; r++) {
        f32 sum = 0.0f;
        for (int c = 0; c < 4; c++) {
            sum += left[c][r] * vec[c];
        }

        result[r] = sum;
    }

    return result;
}

Mat4 operator/(Mat4 left, float scalar) {
    Mat4 result;

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            result[c][r] = left[c][r] / scalar;
        }
    }

    return result;
}

Mat4& operator+=(Mat4& left, Mat4& right) {
    return (left = left + right);
}

Mat4& operator-=(Mat4& left, Mat4& right) {
    return (left = left - right);
}

Mat4& operator*=(Mat4& left, Mat4 right) {
    return (left = left * right);
}

Mat4& operator*=(Mat4& left, float right) {
    return (left = left * right);
}

Mat4& operator/=(Mat4& left, float right) {
    return (left = left / right);
}

bool operator==(Mat4& left, Mat4& right) {
    return left[0] == right[0] && left[1] == right[1] && left[2] == right[2] && left[3] == right[3];
}

bool operator!=(Mat4& left, Mat4& right) {
    return !(left == right);
}

}
