#version 140

in vec4 vPosition;
in vec4 vNormal;

out vec4 Position;
out vec4 Normal;

uniform mat4 ModelView;
uniform mat4 Projection;

void main()
{
    Normal = vNormal;
    Position = vPosition;
    gl_Position = Projection * ModelView * vPosition;
}
