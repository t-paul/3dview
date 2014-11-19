#version 150

in vec4 vPosition;
in vec4 vNormal;

uniform mat4 ModelView;
uniform mat4 Projection;

void main() {
    gl_Position = Projection * ModelView * vPosition;
}