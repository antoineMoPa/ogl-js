#version 300 es

layout(location = 0) in vec3 vertexPosition_modelspace;
 
uniform mat4 MVP;

void main(){
    gl_Position = MVP * vec4(vertexPosition_modelspace,1);
    gl_PointSize = 3.0;
}
