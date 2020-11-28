#version 330 core

out vec4 color;

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
    color = texture(tex0, fs_in.uv);
}
