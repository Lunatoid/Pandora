
cbuffer ShaderData {
    matrix u_mvp;
    matrix u_model;
};

struct VSOut {
    float3 position : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float2 uv : UV, float3 norm : NORMAL) {
    VSOut vso;
    vso.position = (float3)mul(u_model, float4(pos, 1.0f));
    vso.uv = uv;
    vso.normal = (float3)mul(u_model, float4(norm, 0.0f));
    vso.pos = mul(mul(u_mvp, u_model), float4(pos, 1.0f));

    return vso;
}
