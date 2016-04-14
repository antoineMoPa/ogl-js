#version 300 es

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 v_normal;

uniform mat4 MVP;

vec3 light_position = vec3(40.0f,40.0f,10.0f);

out vec3 light_pos;
out vec3 pos_model;
out vec2 UV;
out vec3 normal;
out vec3 normal_model;
out float light_power;

void main(){
    vec3 pos = v_pos;
    gl_Position = MVP * vec4(pos,1.0);
    pos_model = pos;
    normal_model = v_normal;

    normal = v_normal;

    light_power = 300.0;
    light_pos = light_position;
    UV = vertexUV;
}
