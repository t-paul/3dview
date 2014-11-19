#version 150

layout(triangles) in;
layout(line_strip, max_vertices=5) out;

uniform float NormalLength;
uniform mat4 ModelView;
uniform mat4 Projection;

in Vertex
{
  vec4 normal;
  vec4 color;
} vertex[];

out vec4 vertex_color;

void line(vec3 p0, vec3 p1)
{
    vertex_color = vec4(0, 0, 0, 1);
    gl_Position = Projection * ModelView * vec4(p0, 1.0);
    EmitVertex();
    vertex_color = vec4(0, 0, 0, 1);
    gl_Position = Projection * ModelView * vec4(p1, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    vec3 P0 = gl_in[0].gl_Position.xyz;
    vec3 P1 = gl_in[1].gl_Position.xyz;
    vec3 P2 = gl_in[2].gl_Position.xyz;

    if (NormalLength > 0.001) {
        vec3 V0 = P0 - P1;
        vec3 V1 = P2 - P1;

        vec3 N = cross(V1, V0);
        N = normalize(N);

        vec3 P = (P0+P1+P2) / 3.0;

        gl_Position = Projection * ModelView * vec4(P, 1.0);
        vertex_color = vec4(0, 0, 0, 1);
        EmitVertex();

        gl_Position = Projection * ModelView * vec4(P + N * NormalLength, 1.0);
        vertex_color = vec4(1, 1, 1, 0.5);
        EmitVertex();
        EndPrimitive();
    }

    //line(P0, P1);
    //line(P1, P2);
    //line(P2, P0);
}