#version 300 es

precision highp float;

in vec3 pos;
out vec3 color;

void main(){
    color = pos;
}
