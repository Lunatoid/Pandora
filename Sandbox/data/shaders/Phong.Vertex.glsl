#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

out vec3 position;
out vec2 texUV;
out vec3 normal;

uniform mat4 u_mvp;
uniform mat4 u_model;

void main() {
    gl_Position = u_mvp * u_model * vec4(aPosition, 1.0);

    position = (u_model * vec4(aPosition, 1.0)).xyz;
    texUV = aUV;
    normal = (u_model * vec4(aNormal, 0.0)).xyz;
}
