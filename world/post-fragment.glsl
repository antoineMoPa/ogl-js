#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D renderedTexture;
uniform int time;

void main(){
    highp vec2 uv = UV;
    highp float small_time = float(time%10000000)/10000.0;
    
    // wavy rendering
    //uv.x += sin(uv.y * 10.0 + small_time * 2.0) * 0.01;

    // fetch color
    color = texture(renderedTexture, uv) * vec4(0.0,1.0,0.0,1.0);
    // offset blue on x axis
    color += vec4(0.0,0.0,texture(renderedTexture, uv + vec2(0.004),0.00).b,0.0);
    // offset right on x axis
    color += vec4(texture(renderedTexture, uv - vec2(0.004),0.00).r,0.0,0.0,0.0);

    // "Pixel" lines (create darker spots)
    color.rgb *= 0.5 * pow(sin(uv.y * 300.0),2.0) + 0.5;
    color.rgb *= 0.5 * pow(sin(uv.x * 600.0),2.0) + 0.5;
    
    // Background where alpha is close to 0
    color += (1.0 - color.a) * vec4(0.1,0.2,0.3,1.0);

    // Vignette effect (darker corners)
    // Distance from center:
    highp float dist_from_center = distance(vec2(0.5,0.5),uv);
    color *= 1.0 - pow(dist_from_center,2.0);
}
