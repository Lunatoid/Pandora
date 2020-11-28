
#include "LitData.hlsl"

cbuffer ShaderData : register(b0) {
    matrix u_view_proj;
    matrix u_model;
};

cbuffer LightData : register(b1) {
    float3 lightPos;
    float3 viewPos;
};

VSOut main(VSIn vsi) {
    VSOut vso;
    vso.fragPos = (float3)mul(u_model, float4(vsi.position, 1.0));
    vso.pos = mul(mul(u_view_proj, u_model), float4(vsi.position, 1.0));
    vso.uv = vsi.uv;
    
    float3x3 normalMatrix = (float3x3)transpose(u_model);
    float3 T = normalize(mul(vsi.tangent, normalMatrix));
    float3 N = normalize(mul(vsi.normal, normalMatrix));
    T = normalize(T - dot(T, N) * N);
    float3 B = cross(N, T);
    
    float3x3 TBN = transpose(float3x3(T, B, N));    
    vso.tangentLightPos = mul(lightPos, TBN);
    vso.tangentViewPos = mul(viewPos, TBN);
    vso.tangentFragPos = mul(vso.fragPos, TBN);

    return vso;
}
