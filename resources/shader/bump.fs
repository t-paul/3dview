#version 150

in vec4 Position;
in vec2 TextureCoord;

out vec4 fragColor;

uniform vec4 DiffuseProduct;
uniform vec4 LightPosition;
uniform sampler2D Texture1;
uniform sampler2D Texture2;

void main() {
    vec3 normal = normalize(texture2D(Texture2, TextureCoord).rgb * 2.0 - 1.0);

    float diffuse = max(dot(normal, normalize(LightPosition.xyz)), 0.0);

    vec3 color = diffuse * texture2D(Texture1, TextureCoord).rgb;

    fragColor = vec4(color, 1.0);
}