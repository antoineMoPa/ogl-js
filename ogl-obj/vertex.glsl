#version 300 es

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec2 vertexUV;

uniform mat4 MVP;

out vec3 pos;
out vec2 UV;

void main(){
    gl_Position = MVP * vec4(v_pos,1);
    pos = gl_Position.xyz;
    UV = vertexUV;
}
