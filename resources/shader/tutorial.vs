#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

out vec4 Color;
out vec3 Normal0;
out vec3 WorldPos0;

uniform mat4 gWorld;

void main()
{
    gl_Position = gWorld * vec4(Position, 1.0);
    Color = vec4(clamp(Position, 0.0, 1.0), 1.0);
    Normal0 = (gWorld * vec4(Normal, 0.0)).xyz;
    WorldPos0 = (gWorld * vec4(Position, 1.0)).xyz;
}