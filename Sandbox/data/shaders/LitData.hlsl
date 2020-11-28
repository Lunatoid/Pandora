
struct VSOut {
    float4 pos : SV_POSITION;
    float3 fragPos : FRAG_POS;
    float2 uv : UV;
    float3 tangentLightPos : TLP;
    float3 tangentViewPos : TVP;
    float3 tangentFragPos : TFP;
};

struct VSIn {
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float3 tangent : TANGENT;
};
