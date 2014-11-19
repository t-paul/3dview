//http://www.youtube.com/user/thecplusplusguy
//GLSL tutorial - per-fragment (per-pixel lighting)
//vertex.vs
//#version 120
varying vec3 position;
varying vec3 normal;

void main()
{
	gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;	//output position with projection
	position=gl_Vertex;
	normal=gl_Normal;
}