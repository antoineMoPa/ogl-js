#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D last_pass;
uniform sampler2D pass_0;
uniform sampler2D pass_1;
uniform sampler2D pass_2;
uniform int time;
uniform int pass;
uniform int frame_count;
highp vec4 rand_var;

void main(){
    highp vec4 last = texture(last_pass,UV);

    highp float fac = float(pass - 1)/4.0;

    // a,b = c
    highp vec2 z,c,old_z;

    z = vec2(0.00,0.00);
    c = 5.0 * vec2(UV.x - 0.5, UV.y - 0.5);
    c.x -= 0.5;

    int current_step = 0;

    highp float maximum = 10.0;
    int iterations = 40;
    for(int i = 0; i < iterations; i++){
        old_z = z;
        z.x = pow(z.x,2.0) - pow(z.y,2.0);
        z.y = 2.0 * old_z.x * old_z.y;
        z += c;
        
        if(distance(z,vec2(0.0,0.0)) > maximum){
            break;
        }
        current_step++;
    }

    if(frame_count == 0){
        color.r = float(current_step) / float(iterations);
        color.g = 0.0;
        color.b = color.r;
        color.a = 1.0;
    } else if(pass == 1){
        color = texture(pass_2,UV);
    } else if(pass == 2){
        color = last;
    } else if(pass == 3){
        color = last;
    }
}
