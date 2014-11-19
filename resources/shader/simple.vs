#version 150

in vec4 vPosition;
in vec4 vNormal;

out float diffuse_value;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;

void main() {
    mat4 normalMatrix = transpose(inverse(ModelView));
    vec4 vertex_normal = normalize(normalMatrix * vNormal);
    diffuse_value = max(dot(vertex_normal.xyz, LightPosition.xyz), 0.0);
    gl_Position = Projection * ModelView * vPosition;
}