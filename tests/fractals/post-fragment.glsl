#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D last_pass;
uniform sampler2D pass_0;
uniform sampler2D pass_1;
uniform sampler2D pass_2;
uniform sampler2D pass_3;
uniform int time;
uniform int pass;
uniform int frame_count;
highp vec4 rand_var;

void main(){
    highp vec4 last = texture(last_pass,UV);
    
    if(pass == 0){
        color = vec4(UV.x,UV.y,0.0,1.0);
    } else {
        color = last;
    }
}
