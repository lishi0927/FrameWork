#version 330 core

uniform sampler2D screenTexture;

in vec2 uv;

out vec4 color;

void main() {
    color = texture(screenTexture, uv);
}