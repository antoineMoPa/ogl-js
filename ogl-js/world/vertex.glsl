#version 300 es

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 v_normal;

uniform mat4 MVP;
out vec3 pos;
out vec2 UV;
out vec3 normal;

void main(){
    gl_Position = MVP * vec4(v_pos,1);
    pos = gl_Position.xyz;
    normal = (MVP * vec4(v_normal,1)).xyz;
    UV = vertexUV;
}
