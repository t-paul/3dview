#version 150

in float diffuse_value;

out vec4 fragColor;

uniform vec4 AmbientProduct;

void main() {
    fragColor = AmbientProduct * diffuse_value;
}