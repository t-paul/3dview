//http://www.youtube.com/user/thecplusplusguy
//GLSL tutorial - per-fragment (per-pixel lighting)
//fragment.frag
//#version 120
varying vec3 position;
varying vec3 normal;

// uniform vec3 lightPos;

//uniform vec3 mambient;
//uniform vec3 mdiffuse;

//uniform vec3 lambient;
//uniform vec3 ldiffuse;

//uniform vec3 mspecular;
//uniform vec3 lspecular;

//uniform float shininess;

void main()
{
        float shininess = 16;
        vec3 mspecular = vec3(1.0, 1.0, 1.0);
        vec3 lspecular = vec3(1.0, 1.0, 1.0);

        vec3 lightPos = vec3(2.0, 2.0, 20.0);

        vec3 mambient = vec3(0.3, 0.6, 0.2);
        vec3 mdiffuse = vec3(0.6, 0.6, 0.6);

        vec3 lambient = vec3(0.3, 0.3, 0.3);
        vec3 ldiffuse = vec3(0.6, 0.6, 0.6);

	float dist=length(position-lightPos);	//distance from light-source to surface
	//float att=1.0/(1.0+0.1*dist+0.01*dist*dist);	//attenuation (constant,linear,quadric)
        float att = 1;
	vec3 ambient=mambient*lambient;	//the ambient light
	
	vec3 surf2light=normalize(lightPos-position);
	vec3 norm=normalize(normal);
	float dcont=max(0.0,dot(norm,surf2light));
	vec3 diffuse=dcont*(mdiffuse*ldiffuse);
	
	vec3 surf2view=normalize(-position);
	vec3 reflection=reflect(-surf2light,norm);
	
	float scont=pow(max(0.0,dot(surf2view,reflection)),shininess);
	vec3 specular=scont*lspecular*mspecular;
	
	gl_FragColor=vec4((ambient+diffuse+specular)*att,1.0);	//<- don't forget the paranthesis (ambient+diffuse+specular)
}