#version 300 es

precision highp float;

out vec3 color;

void main(){
    color = vec3(sin(gl_PointCoord.x * 30.0),
                 gl_PointCoord.y,
                 0.2);
}
