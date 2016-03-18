#version 330 core

in vec2 UV;
out vec4 color;
uniform sampler2D renderedTexture;
uniform float time;

void main(){
    //color = texture(renderedTexture, UV + 0.005*vec2( sin(time+1024.0*UV.x),cos(time+768.0*UV.y)) ).xyz;
    color = vec4(1.0,0.0,0.0,1.0);
}
