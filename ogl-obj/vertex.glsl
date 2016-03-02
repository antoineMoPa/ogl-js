#version 300 es

layout(location = 0) in vec3 vertexPosition_modelspace;
 
uniform mat4 MVP;

out vec3 pos;

void main(){
    gl_Position = MVP * vec4(vertexPosition_modelspace,1);
    pos = gl_Position.xyz;
    gl_PointSize = 3.0;
}
