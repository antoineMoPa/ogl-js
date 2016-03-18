#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D renderedTexture;
uniform int time;

void main(){
    highp vec2 uv = UV;
    highp float small_time = float(time%10000000)/1000.0;
    uv.x += sin(uv.y * 10.0 + small_time * 2.0) * 0.01;
    color = vec4(texture(renderedTexture, uv).rgb,1.0);
    color += vec4(0.0,0.1,0.1,0.0);
}
