#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D renderedTexture;
uniform int time;

void main(){
    highp vec2 uv = UV;
    highp float small_time = float(time%10000000)/10000.0;
    
    // wavy rendering
    uv.x += sin(uv.y * 10.0 + small_time * 10.0) * 0.05;

    // fetch color
    color = texture(renderedTexture, uv);
    
    // Vignette effect (darker corners)
    // Distance from center:
    highp float dist_from_center = distance(vec2(0.5,0.5),uv);
    color *= 1.0 - pow(dist_from_center,2.0);
}
