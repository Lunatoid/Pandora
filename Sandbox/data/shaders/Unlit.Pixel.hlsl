
#include "LitData.hlsl"

Texture2D albedo : register(t0);
Texture2D normal : register(t1);

SamplerState albedoSamp;
SamplerState normalSamp;

float4 main(in VSOut vso) : SV_TARGET {
    return albedo.Sample(albedoSamp, vso.uv);
}
