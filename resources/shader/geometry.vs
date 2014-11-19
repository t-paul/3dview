#version 150

in vec4 vPosition;
in vec4 vNormal;

out Vertex
{
  vec4 normal;
  vec4 color;
} vertex;

void main()
{
  gl_Position = vPosition;
  vertex.normal = vNormal;
  vertex.color =  vec4(0.0, 0.0, 0.5, 1.0);
}