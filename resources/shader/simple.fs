// #version 330 core

varying vec3 normal;

void main(){
    if (gl_FrontFacing) {
        gl_FragColor = vec4(normal.x, 0.0, 0.0, 1.0);
    } else {
        gl_FragColor = vec4(0.0, 0.5, 0.0, 1.0);
    }
}