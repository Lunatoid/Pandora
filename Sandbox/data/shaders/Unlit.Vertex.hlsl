
cbuffer ShaderData {
    matrix u_mvp;
    matrix u_model;
};

struct VSOut {
    float2 uv : UV;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float2 uv : UV, float3 norm : NORMAL) {
    VSOut vso;
    vso.uv = uv;
    vso.pos = mul(mul(u_mvp, u_model), float4(pos, 1.0f));

    return vso;
}
