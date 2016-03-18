#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D renderedTexture;
uniform highp float time;

void main(){
    //color = vec4(UV,1.0,1.0);;
    color = vec4(texture(renderedTexture, sin(UV)).rgb,1.0);
    //color = vec4(1.0,0.0,0.0,1.0);
}
