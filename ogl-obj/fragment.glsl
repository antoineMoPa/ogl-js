#version 300 es

precision highp float;

out vec3 color;

in vec2 UV;
in vec3 pos;
in vec3 normal;
uniform sampler2D texSampler;

void main(){
    color = texture(texSampler,UV).rgb;
    vec3 pos_m = pos;
    //color = sin(pos_m);
    //color = vec3(UV.x,UV.y,0);
    color = vec3(normal.z);
}
