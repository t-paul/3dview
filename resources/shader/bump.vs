#version 150

in vec4 vPosition;
in vec2 vTextureCoord;

out vec4 Position;
out vec2 TextureCoord;

uniform mat4 ModelView;
uniform mat4 Projection;

void main() {
    Position = Projection * ModelView * vPosition;
    TextureCoord = vTextureCoord;
    gl_Position = Projection * ModelView * vPosition;
}