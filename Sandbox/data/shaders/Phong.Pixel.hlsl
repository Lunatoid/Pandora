
cbuffer LightData {
    float3 lightPos;
    float3 materialColor;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
}

Texture2D tex;

SamplerState samp;

float4 main(float3 position : POSITION, float2 uv : UV, float3 normal : NORMAL) : SV_TARGET {
    const float3 vertexLightDir = lightPos - position;
    const float distToLight = length(vertexLightDir);
    const float3 dirToLight = vertexLightDir / distToLight;

    const float att = 1.0f / (attConst + attLin * distToLight + attQuad * (distToLight * distToLight));
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToLight, normal));

    return tex.Sample(samp, uv) * float4(materialColor, 1.0f) * float4(saturate(diffuse + ambient), 1.0f);
}
