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

    highp float fac = float(pass - 1)/4.0;

    if(time % 1000 < 500){
        color = last;
        if(pass == 1){
            color = vec4(0.0);
        }
        if(UV.x < fac){
            color += vec4(0.3);
        }
    } else {
        // Should display:
        // first pass (blue) at left
        // second pass (yellow) at right
        if(pass == 1){
            color = vec4(0.0,0.0,1.0,1.0);
        } else if (pass == 2){
            color = vec4(1.0,1.0,0.0,1.0);
        } else if (pass == 3){
            if(UV.x < 0.5){
                color = texture(pass_1,UV);
            } else {
                color = texture(pass_2,UV);
            }
        }
    }
    
}
