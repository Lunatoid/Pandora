#include "Perlin.h"

#include "Pandora/Core/Math/Math.h"

#define STB_PERLIN_IMPLEMENTATION
#include "Pandora/Libs/stb/stb_perlin.h"

namespace pd {

f32 Perlin3D(f32 x, f32 y, f32 z) {
    // Standard perlin ranges from -1 to 1
    f32 value = stb_perlin_noise3(x, y, z, 0, 0, 0);
    return (value + 1.0f) / 2.0f;
}

f32 Perlin3D(Vec3 position) {
    return Perlin3D(position.x, position.y, position.z);
}

f32 RidgedPerlin3D(f32 x, f32 y, f32 z, int octaves, f32 persistence, f32 lacunarity, f32 offset) {
    f32 value = stb_perlin_ridge_noise3(x, y, z, lacunarity, persistence, offset, octaves);
    return value;
}

f32 RidgedPerlin3D(Vec3 position, int octaves, f32 persistence, f32 lacunarity) {
    return RidgedPerlin3D(position.x, position.y, position.z, octaves, persistence, lacunarity);
}

f32 FBMPerlin3D(f32 x, f32 y, f32 z, int octaves, f32 persistence, f32 lacunarity) {
    f32 value = stb_perlin_fbm_noise3(x, y, z, lacunarity, persistence, octaves);
    return value;
}

f32 FBMPerlin3D(Vec3 position, int octaves, f32 persistence, f32 lacunarity) {
    return FBMPerlin3D(position.x, position.y, position.z, octaves, persistence, lacunarity);
}

f32 TurbulencePerlin3D(f32 x, f32 y, f32 z, int octaves, f32 persistence, f32 lacunarity) {
    f32 value = stb_perlin_turbulence_noise3(x, y, z, lacunarity, persistence, octaves);
    return value;
}

f32 TurbulencePerlin3D(Vec3 position, int octaves, f32 persistence, f32 lacunarity) {
    return TurbulencePerlin3D(position.x, position.y, position.z, octaves, persistence, lacunarity);
}

}
