#version 300 es

precision highp float;

out vec3 color;

in vec2 UV;
uniform sampler2D texSampler;

void main(){
    color = texture(texSampler,UV).rgb;
    //color = vec3(UV.x,UV.y,0);
}
