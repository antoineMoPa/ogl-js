#version 300 es

precision highp float;

in vec3 pos;
out vec3 color;

in vec2 UV;
uniform sampler2D texSampler;

void main(){
    //color = texture(texSampler,UV).rgb;
    //color = vec3(UV,1);
    color = vec3(1,1,0);
}
