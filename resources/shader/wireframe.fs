#version 150

uniform float NormalLength;

in vec3 weight;

out vec4 fragColor;

const float DISCARD_AT = 0.1;
const float BACKGROUND_AT = 0.05;
const float BACKGROUND = 1.0;

void main() {
    float mindist = min(weight.r, min(weight.g, weight.b));
    if (mindist > DISCARD_AT) discard;
    float intensity = mindist > BACKGROUND_AT ? BACKGROUND : NormalLength;
    fragColor = vec4(vec3(intensity), 1.0);
}