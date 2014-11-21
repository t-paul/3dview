#version 150

flat in vec3 Normal;

out vec4 fragColor;

uniform vec4 DiffuseProduct;
uniform vec4 LightPosition;

void main() {
    float f = dot(normalize(LightPosition.xyz), Normal);
    fragColor = f * DiffuseProduct;
}