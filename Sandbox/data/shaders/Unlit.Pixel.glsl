#version 330 core

in vec3 position;
in vec2 texUV;
in vec3 normal;

out vec4 color;

uniform sampler2D tex0;

// Just here so that we don't get "uniform not found" errors
uniform vec3 lightPos;
uniform vec3 materialColor;
uniform vec3 ambient;
uniform vec3 diffuseColor;
uniform float diffuseIntensity;
uniform float attConst;
uniform float attLin;
uniform float attQuad;

void main() {
    color = texture(tex0, texUV);
}
