
Texture2D tex;

SamplerState samp;

float4 main(float2 uv : UV) : SV_TARGET {
    return tex.Sample(samp, uv);
}
