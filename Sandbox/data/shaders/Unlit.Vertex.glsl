#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;

out VS_OUT {
    vec3 fragPos;
    vec2 uv;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} vs_out;

uniform mat4 u_view_proj;
uniform mat4 u_model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    vs_out.fragPos = vec3(u_model * vec4(position, 1.0));   
    vs_out.uv = uv;
    
    gl_Position = u_view_proj * u_model * vec4(position, 1.0);
}
