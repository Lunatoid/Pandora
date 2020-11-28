
#include "LitData.hlsl"

cbuffer ShaderData : register(b0) {
    matrix u_view_proj;
    matrix u_model;
};

VSOut main(VSIn vsi) {
    VSOut vso;
    vso.pos = mul(mul(u_view_proj, u_model), float4(vsi.position, 1.0f));
    vso.fragPos = (float3)vso.pos;
    vso.uv = vsi.uv;
    vso.tangentLightPos = float3(0.0, 0.0, 0.0);
    vso.tangentViewPos = float3(0.0, 0.0, 0.0);
    vso.tangentFragPos = float3(0.0, 0.0, 0.0);

    return vso;
}
