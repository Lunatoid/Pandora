#version 330 core

out vec4 fragColor;

in VS_OUT {
    vec3 fragPos;
    vec2 uv;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} fs_in;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {           
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(tex1, fs_in.uv).rgb;
    
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
    
   
    // get diffuse color
    vec3 color = texture(tex0, fs_in.uv).rgb;
    
    // ambient
    vec3 ambient = 0.1 * color;
    
    // diffuse
    vec3 lightDir = normalize(fs_in.tangentLightPos - fs_in.tangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    
    // specular
    vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    fragColor = vec4(ambient + diffuse + specular, 1.0);
}