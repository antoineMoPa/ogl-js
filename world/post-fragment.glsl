#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D renderedTexture;
uniform int time;

void main(){
    highp vec2 uv = UV;
    highp float small_time = float(time%10000000)/10000.0;
    // wavy rendering
    //uv.x += sin(uv.y * 10.0 + small_time * 2.0) * 0.1;
    color = texture(renderedTexture, uv);
    // background
    color += (1.0 - color.a) * vec4(0.1,0.1,uv.y * 0.8,1.0);
}
