#include "Math.h"

#include <math.h>

namespace pd {

f64 ToDegrees(f64 radians) {
    return radians * RAD_TO_DEG;
}

f32 ToDegrees(f32 radians) {
    return radians * (f32)RAD_TO_DEG;
}

f64 ToRadians(f64 degrees) {
    return degrees * DEG_TO_RAD;
}

f32 ToRadians(f32 degrees) {
    return degrees * (f32)DEG_TO_RAD;
}

f64 Kilobytes(f64 kb) {
    return kb * 1024.0;
}

f64 Megabytes(f64 mb) {
    return Kilobytes(mb) * 1024.0;
}

f64 Gigabytes(f64 gb) {
    return Megabytes(gb) * 1024.0;
}

f64 ToKilobytes(f64 bytes) {
    return bytes / 1024.0;
}

f64 ToMegabytes(f64 bytes) {
    return ToKilobytes(bytes) / 1024.0;
}

f64 ToGigabytes(f64 bytes) {
    return ToMegabytes(bytes) / 1024.0;
}

f64 Ceil(f64 v) {
    return ceil(v);
}

f32 Ceil(f32 v) {
    return ceilf(v);
}

int Ceil(int v) {
    return (int)ceil((f64)v);
}

f64 Floor(f64 v) {
    return floor(v);
}

f32 Floor(f32 v) {
    return floorf(v);
}

int Floor(int v) {
    return (int)floor((f64)v);
}

f64 Round(f64 v) {
    return round(v);
}

f32 Round(f32 v) {
    return roundf(v);
}

int Round(int v) {
    return (int)round((f64)v);
}

int RoundToPow2(int v) {
    v -= 1;

    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v += 1;

    return v;
}

bool IsPowerOf2(int v) {
    return (v != 0) && ((v & (v - 1)) == 0);
}

f64 Sin(f64 v) {
    return sin(v);
}

f32 Sin(f32 v) {
    return sinf(v);
}

f64 Cos(f64 v) {
    return cos(v);
}

f32 Cos(f32 v) {
    return cosf(v);
}

f64 Tan(f64 v) {
    return tan(v);
}

f32 Tan(f32 v) {
    return tanf(v);
}

f64 Asin(f64 v) {
    return asin(v);
}

f32 Asin(f32 v) {
    return asinf(v);
}

f64 Acos(f64 v) {
    return acos(v);
}

f32 Acos(f32 v) {
    return acosf(v);
}

f64 Atan(f64 v) {
    return atan(v);
}

f32 Atan(f32 v) {
    return atanf(v);
}

f64 Atan2(f64 y, f64 x) {
    return atan2(y, x);
}

f32 Atan2(f32 y, f32 x) {
    return atan2f(y, x);
}

f64 Log(f64 v) {
    return log(v);
}

f32 Log(f32 v) {
    return logf(v);
}

f64 Log(f64 v, f64 base) {
    return Log(v) / Log(base);
}

f32 Log(f32 v, f32 base) {
    return Log(v) / Log(base);
}

f64 Log10(f64 v) {
    return log10(v);
}

f32 Log10(f32 v) {
    return log10f(v);
}

f64 Mod(f64 n, f64 denom) {
    return Clamp(n - Floor(n / denom) * denom, 0.0, denom);
}

f32 Mod(f32 n, f32 denom) {
    return Clamp(n - (f32)Floor(n / denom) * denom, 0.0f, denom);
}

int Sign(int v) {
    return (v < 0) ? -1 : 1;
}

f64 Sign(f64 v) {
    return (v < 0.0) ? -1.0 : 1.0;
}

f32 Sign(f32 v) {
    return (v < 0.0f) ? -1.0f : 1.0f;
}

int CopySign(int x, int y) {
    return x * Sign(y);
}

f32 CopySign(f32 x, f32 y) {
    return x * Sign(y);
}

f64 CopySign(f64 x, f64 y) {
    return x * Sign(y);
}

f64 DeltaLooping(f64 current, f64 target, f64 boundary) {
    f64 num = Mod(target - current, boundary);
    if (num > boundary / 2.0) {
        num -= boundary;
    }
    return num;
}

f32 DeltaLooping(f32 current, f32 target, f32 boundary) {
    f32 num = Mod(target - current, boundary);
    if (num > boundary / 2.0f) {
        num -= boundary;
    }
    return num;
}

int DeltaLooping(int current, int target, int boundary) {
    int num = (target - current) % boundary;
    if (num > boundary / 2) {
        num -= boundary;
    }
    return num;
}

f64 DeltaAngle(f64 current, f64 target) {
    return DeltaLooping(current, target, 360.0);
}

f32 DeltaAngle(f32 current, f32 target) {
    return DeltaLooping(current, target, 360.0f);
}

f64 MoveTowards(f64 current, f64 target, f64 maxDelta) {
    f64 result;

    if (Abs(target - current) <= maxDelta) {
        result = target;
    } else {
        result = current + Sign(target - current) * maxDelta;
    }

    return result;
}

f32 MoveTowards(f32 current, f32 target, f32 maxDelta) {
    f32 result;

    if (Abs(target - current) <= maxDelta) {
        result = target;
    } else {
        result = current + Sign(target - current) * maxDelta;
    }

    return result;
}

int MoveTowards(int current, int target, int maxDelta) {
    int result;

    if (Abs(target - current) <= maxDelta) {
        result = target;
    } else {
        result = current + Sign(target - current) * maxDelta;
    }

    return result;
}

f64 MoveTowardsLooping(f64 current, f64 target, f64 maxDelta, f64 boundary) {
    f64 num = DeltaLooping(current, target, boundary);

    f64 result;
    if (-maxDelta < num && num < maxDelta) {
        result = target;
    } else {
        target = current + num;
        result = MoveTowards(current, target, maxDelta);

        if (result < 0.0) {
            result += boundary;
        } else if (result > boundary) {
            result -= boundary;
        }
    }

    return result;
}

f32 MoveTowardsLooping(f32 current, f32 target, f32 maxDelta, f32 boundary) {
    f32 num = DeltaLooping(current, target, boundary);

    f32 result;
    if (-maxDelta < num && num < maxDelta) {
        result = target;
    } else {
        target = current + num;
        result = MoveTowards(current, target, maxDelta);

        if (result < 0.0f) {
            result += boundary;
        } else if (result > boundary) {
            result -= boundary;
        }
    }

    return result;
}

int MoveTowardsLooping(int current, int target, int maxDelta, int boundary) {
    int num = DeltaLooping(current, target, boundary);

    int result;
    if (-maxDelta < num && num < maxDelta) {
        result = target;
    } else {
        target = current + num;
        result = MoveTowards(current, target, maxDelta);

        if (result < 0) {
            result += boundary;
        } else if (result > boundary) {
            result -= boundary;
        }
    }

    return result;
}

f64 MoveTowardsAngle(f64 current, f64 target, f64 maxDelta) {
    return MoveTowardsLooping(current, target, maxDelta, 360.0);
}

f32 MoveTowardsAngle(f32 current, f32 target, f32 maxDelta) {
    return MoveTowardsLooping(current, target, maxDelta, 360.0f);
}

f64 Exp(f64 power) {
    return exp(power);
}

f32 Exp(f32 power) {
    return expf(power);
}

int Exp(int power) {
    return (int)exp((f64)power);
}

f64 Pow(f64 base, f64 exp) {
    return pow(base, exp);
}

f32 Pow(f32 base, f32 exp) {
    return powf(base, exp);
}

int Pow(int base, int exp) {
    return (int)pow((f64)base, (f64)exp);
}

f64 Sqrt(f64 v) {
    return sqrt(v);
}

f32 Sqrt(f32 v) {
    return sqrtf(v);
}

f64 InverseSqrt(f64 v) {
    return 1.0 / Sqrt(v);
}

f32 InverseSqrt(f32 v) {
    return 1.0f / Sqrt(v);
}

f32 FastInverseSqrt(f32 v) {
    union {
        f32 f;
        u32 i;
    } conv = {v};

    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f *= 1.5f - (v * 0.5f * conv.f * conv.f);

    return conv.f;

}

}
