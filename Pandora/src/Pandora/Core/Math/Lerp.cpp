#include "Lerp.h"

#include "Pandora/Core/Math/Math.h"

namespace pd {

f32 Lerp(f32 a, f32 b, f32 t) {
    return LerpUnclamped(a, b, Clamp(t, 0.0f, 1.0f));
}

int Lerp(int a, int b, f32 t) {
    return LerpUnclamped(a, b, Clamp(t, 0.0f, 1.0f));
}

Vec2 Lerp(Vec2 a, Vec2 b, f32 t) {
    return LerpUnclamped(a, b, Clamp(t, 0.0f, 1.0f));
}

Vec3 Lerp(Vec3 a, Vec3 b, f32 t) {
    return LerpUnclamped(a, b, Clamp(t, 0.0f, 1.0f));
}

Vec4 Lerp(Vec4 a, Vec4 b, f32 t) {
    return LerpUnclamped(a, b, Clamp(t, 0.0f, 1.0f));
}

f32 LerpUnclamped(f32 a, f32 b, f32 t) {
    return (1.0f - t) * a + t * b;
}

int LerpUnclamped(int a, int b, f32 t) {
    return (int)LerpUnclamped((f32)a, (f32)b, t);
}

Vec2 LerpUnclamped(Vec2 a, Vec2 b, f32 t) {
    f32 x = LerpUnclamped(a.x, b.x, t);
    f32 y = LerpUnclamped(a.y, b.y, t);

    return Vec2(x, y);
}

Vec3 LerpUnclamped(Vec3 a, Vec3 b, f32 t) {
    f32 x = LerpUnclamped(a.x, b.x, t);
    f32 y = LerpUnclamped(a.y, b.y, t);
    f32 z = LerpUnclamped(a.z, b.z, t);

    return Vec3(x, y, z);
}

Vec4 LerpUnclamped(Vec4 a, Vec4 b, f32 t) {
    f32 x = LerpUnclamped(a.x, b.x, t);
    f32 y = LerpUnclamped(a.y, b.y, t);
    f32 z = LerpUnclamped(a.z, b.z, t);
    f32 w = LerpUnclamped(a.w, b.w, t);

    return Vec4(x, y, z, w);
}

f64 LerpLooping(f64 a, f64 b, f32 t, f64 boundary) {
    f64 num = Mod(b - a, boundary);

    if (num > boundary / 2.0) {
        num -= boundary;
    }

    f64 result = a + num * Clamp(t, 0.0f, 1.0f);

    // Handle wrapping below 0 or past the boundary
    if (result < 0.0) {
        result += boundary;
    } else if (result > boundary) {
        result -= boundary;
    }

    return result;
}

f32 LerpLooping(f32 a, f32 b, f32 t, f32 boundary) {
    f32 num = Mod(b - a, boundary);

    if (num > boundary / 2.0f) {
        num -= boundary;
    }

    f32 result = a + num * Clamp(t, 0.0f, 1.0f);

    // Handle wrapping below 0 or past the boundary
    if (result < 0.0f) {
        result += boundary;
    } else if (result > boundary) {
        result -= boundary;
    }

    return result;
}

int LerpLooping(int a, int b, f32 t, int boundary) {
    int num = (b - a) % boundary;

    if (num > boundary / 2) {
        num -= boundary;
    }

    int result = int((f32)a + (f32)num * Clamp(t, 0.0f, 1.0f));

    // Handle wrapping below 0 or past the boundary
    if (result < 0) {
        result += boundary;
    } else if (result > boundary) {
        result -= boundary;
    }

    return result;
}

f32 LerpAngle(f32 a, f32 b, f32 t) {
    return LerpLooping(a, b, t, 360.0f);
}

Color LerpColor(Color a, Color b, f32 t) {
    Color result;

    result.r = LerpLooping(a.r, b.r, t, 1.0f);
    result.g = LerpLooping(a.g, b.g, t, 1.0f);
    result.b = LerpLooping(a.b, b.b, t, 1.0f);
    result.a = LerpLooping(a.a, b.a, t, 1.0f);

    return result;
}

f32 InverseLerp(f32 a, f32 b, f32 v) {
    return f32((v - a) / (b - a));
}

f32 InverseLerp(int a, int b, int v) {
    return f32((v - a) / (b - a));
}

f32 InverseLerp(Vec2 a, Vec2 b, Vec2 v) {
    Vec2 ab = b - a;
    Vec2 av = v - a;

    return Dot(av, ab) / Dot(ab, ab);
}

f32 InverseLerp(Vec3 a, Vec3 b, Vec3 v) {
    Vec3 ab = b - a;
    Vec3 av = v - a;

    return Dot(av, ab) / Dot(ab, ab);
}

f32 InverseLerp(Vec4 a, Vec4 b, Vec4 v) {
    Vec4 ab = b - a;
    Vec4 av = v - a;

    return Dot(av, ab) / Dot(ab, ab);
}

}
