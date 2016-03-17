#version 300 es
layout(location = 0) in vec4 v_pos;

void main(){
    gl_Position = v_pos;
}
