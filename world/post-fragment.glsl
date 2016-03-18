#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D renderedTexture;
uniform highp float time;

void main(){
    highp vec2 uv = UV;
    highp float small_time = sin(time/1000.0);
    uv.x += sin(uv.y * 10.0 + small_time) * 0.01;
    color = vec4(texture(renderedTexture, uv).rgb,1.0);
}
