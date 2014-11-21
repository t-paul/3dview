#version 150

out vec4 fragColor;

uniform vec4 DiffuseProduct;

void main(){
    if (gl_FrontFacing) {
        fragColor = DiffuseProduct;
    } else {
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
}