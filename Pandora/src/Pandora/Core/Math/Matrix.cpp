#include "Matrix.h"

#include "Pandora/Core/Math/Math.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace pd {

glm::mat4 M(const Mat4& m) {
    glm::mat4 m2;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            m2[c][r] = m[c][r];
        }
    }

    return m2;
}


Mat4 M(const glm::mat4& m) {
    Mat4 m2;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            m2[c][r] = m[c][r];
        }
    }

    return m2;
}


Mat4 Transpose(Mat4 mat) {
    return M(glm::transpose(M(mat)));
}

Mat4 Inverse(Mat4 mat) {
    return M(glm::inverse(M(mat)));
}

Mat4 Translate(Vec3 translation) {
    return M(glm::translate(glm::mat4(1.0f), V(translation)));
}

Mat4 Rotate(f32 angle, Vec3 axis) {
    return M(glm::rotate(glm::mat4(1.0f), angle, V(axis)));
}

Mat4 Scale(Vec3 scale) {
    return M(glm::scale(glm::mat4(1.0f), V(scale)));
}

Mat4 Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
    return M(glm::ortho(left, right, bottom, top, near, far));
}

Mat4 Perspective(f32 fov, f32 aspectRatio, f32 near, f32 far) {
    return M(glm::perspective(fov, aspectRatio, near, far));
}

Mat4 LookAt(Vec3 eye, Vec3 center, Vec3 up) {
    return M(glm::lookAt(V(eye), V(center), V(up)));
}

bool Decompose(Mat4 mat, Vec3* scale, Quat* orientation,
               Vec3* translation, Vec3* skew, Vec4* perspective) {
    glm::vec3 scaleVal, transVal, skewVal;
    glm::quat orientVal;
    glm::vec4 perspVal;
    bool result = glm::decompose(M(mat), scaleVal, orientVal, transVal, skewVal, perspVal);

    if (scale) {
        *scale = V(scaleVal);
    }

    if (orientation) {
        *orientation = Q(orientVal);
    }

    if (translation) {
        *translation = V(transVal);
    }

    if (skew) {
        *skew = V(skewVal);
    }

    if (perspective) {
        *perspective = V(perspVal);
    }

    return result;
}

Mat4 ToMat4(Quat q) {
    return M(glm::toMat4(Q(q)));
}

// Operators

Mat4 operator+(Mat4 left, Mat4 right) {
    return M(M(left) + M(right));
}

Mat4 operator-(Mat4 left, Mat4 right) {
    return M(M(left) - M(right));
}

Mat4 operator*(Mat4 left, Mat4 right) {
    return M(M(left) * M(right));
}

Mat4 operator*(Mat4 left, float scalar) {
    return M(M(left) * scalar);
}

Vec4 operator*(Mat4 left, Vec4 vec) {
    return V(M(left) * V(vec));
}

Mat4 operator/(Mat4 left, float scalar) {
    return M(M(left) / scalar);
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
    return M(left) == M(right);
}

bool operator!=(Mat4& left, Mat4& right) {
    return M(left) != M(right);
}

}

