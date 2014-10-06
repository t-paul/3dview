// #version 330 core

varying vec3 normal;

void main(){
    if (normal.z > 0.0) {
        gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        gl_FragColor = vec4(0.0, 0.5, 0.0, 1.0);
    }
}