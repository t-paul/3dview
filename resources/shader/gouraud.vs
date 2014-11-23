#version 150

in vec4 vPosition;
in vec4 vNormal;

out vec4 Color;

uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 AmbientProduct;
uniform vec4 DiffuseProduct;
uniform vec4 SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;

void main()
{
    vec3 pos = vec3(ModelView * vPosition);
    
    vec3 L = normalize(LightPosition.xyz - pos);
    vec3 E = normalize(-pos);
    vec3 H = normalize(L + E);

    vec3 N = normalize(vec3(ModelView * vNormal));

    vec4 ambient = AmbientProduct;

    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd * DiffuseProduct;

    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks * SpecularProduct;

    if (dot(L, N) < 0.0) {
        specular = vec4(0.0, 0.0, 0.0, 1.0);
    }

    Color = ambient + diffuse + specular;;
    gl_Position = Projection * ModelView * vPosition;
}