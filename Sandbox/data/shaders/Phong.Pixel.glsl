#version 330 core

in vec3 position;
in vec2 texUV;
in vec3 normal;

out vec4 color;

uniform sampler2D tex0;

uniform vec3 lightPos;
uniform vec3 materialColor;
uniform vec3 ambient;
uniform vec3 diffuseColor;
uniform float diffuseIntensity;
uniform float attConst;
uniform float attLin;
uniform float attQuad;

void main() {
    vec3 vertexLightDir = lightPos - position;
    float distToLight = length(vertexLightDir);
    vec3 dirToLight = vertexLightDir / distToLight;

    float att = 1.0f / (attConst + attLin * distToLight + attQuad * (distToLight * distToLight));
    vec3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0, dot(dirToLight, normal));

    color = texture(tex0, texUV) * vec4(materialColor, 1.0) * vec4(clamp(diffuse + ambient, 0.0, 1.0), 1.0);
}
