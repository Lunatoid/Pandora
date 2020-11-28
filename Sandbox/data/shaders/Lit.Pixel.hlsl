
#include "LitData.hlsl"

Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);

SamplerState samp;

float4 main(in VSOut vso) : SV_TARGET {         
    // obtain normal from normal map in range [0,1]
    float3 normal = (float3)normalTex.Sample(samp, vso.uv);
    
    // Invert green channel because we use OpenGL normal maps
    normal.g = 1.0 - normal.g;
    
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
    
    // get diffuse color
    float3 color = (float3)albedoTex.Sample(samp, vso.uv);
    
    // ambient
    float3 ambient = 0.1 * color;
    
    // diffuse
    float3 lightDir = normalize(vso.tangentLightPos - vso.tangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    float3 diffuse = diff * color;
    
    // specular
    float3 viewDir = normalize(vso.tangentViewPos - vso.tangentFragPos);
    float3 reflectDir = reflect(-lightDir, normal);
    float3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    float3 specular = float3(0.2, 0.2, 0.2) * spec;
    return float4(ambient + diffuse + specular, 1.0);
}
