#version 150

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTextureCoord;

out vec4 Position;
out vec4 Normal;
out vec2 TextureCoord;

uniform mat4 ModelView;
uniform mat4 Projection;

void main() {
    Position = Projection * ModelView * vPosition;
    mat4 normalMatrix = transpose(inverse(ModelView));
    Normal = normalize(normalMatrix * vNormal);
    TextureCoord = vTextureCoord;
    gl_Position = Projection * ModelView * vPosition;
}