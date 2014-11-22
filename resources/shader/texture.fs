#version 150

in vec4 Position;
in vec4 Normal;
in vec2 TextureCoord;

out vec4 fragColor;

uniform vec4 DiffuseProduct;
uniform vec4 LightPosition;
uniform sampler2D Texture1;

void main() {
    float diffuse = max(dot(Normal.xyz, normalize(-LightPosition.xyz)), 0.0);

    vec3 color = texture2D(Texture1, TextureCoord).rgb;

    fragColor = vec4(color, 1.0);
}