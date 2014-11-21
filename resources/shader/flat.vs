#version 150

in vec4 vPosition;
in vec4 vNormal;

flat out vec3 Normal;

uniform mat4 ModelView;
uniform mat4 Projection;

void main() {
    mat4 normalMatrix = transpose(inverse(ModelView));
    Normal = normalize(vec3(normalMatrix * vNormal));
    gl_Position = Projection * ModelView * vPosition;
}