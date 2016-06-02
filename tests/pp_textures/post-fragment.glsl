#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D bg;
uniform sampler2D last_pass;
uniform sampler2D pass_0;
uniform sampler2D pass_1;
uniform sampler2D pass_2;
uniform int screen_w;
uniform int screen_h;
uniform highp float mouse_x;
uniform highp float mouse_y;
uniform int time;
uniform int pass;
uniform int frame_count;
highp vec4 rand_var;

void main(){
    highp float pixel_width = 1.0 / float(screen_w);
    highp float pixel_height = 1.0 / float(screen_h);

    highp vec2 mouse = vec2(mouse_x,mouse_y);

    highp vec2 mouse_diff = mouse - UV;
    
    if(pass == 1){
        highp vec2 mod_UV;
        highp float time_fac = float(time % 20000) / 300.0;

        mod_UV = UV;

        if(length(mouse_diff) < 0.2){
            highp float fac = length(mouse_diff) / 0.2;
            // The magnifying math
            // Some vector thinking here.
            mod_UV =  mouse - fac * 0.3 * mouse_diff;
        } else if (length(mouse_diff) < 0.23){
            // Loop border
            color = vec4(0.0,0.0,0.0,1.0);
            return;
        }

        color = texture(bg, mod_UV);

        if( mod_UV.x < 0.0 || mod_UV.x > 1.0 ||
            mod_UV.y < 0.0 || mod_UV.y > 1.0 ){
            color = vec4(0.0,0.0,0.0,1.0);
        }
        
    } else {
        color = texture(pass_1, UV);
    }
    
    color.a = 1.0;
}
