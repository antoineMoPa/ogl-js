#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D renderedTexture;
uniform int time;
uniform int frame_count;

void main(){
    if(frame_count == 0){
        color = vec4(UV.x,UV.y,1.0,1.0);
    } else {
        //color = texture(renderedTexture,UV);
        //color += 0.01 * sin(UV.x);
        //color += 0.01 * cos(UV.y);
    }

    //color = vec4(1.0,1.0,1.0,1.0);
}
