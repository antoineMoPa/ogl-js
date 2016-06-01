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
uniform int time;
uniform int pass;
uniform int frame_count;
highp vec4 rand_var;

void main(){
    highp float pixel_width = 1.0 / float(screen_w);
    highp float pixel_height = 1.0 / float(screen_h);
    
    if(pass == 1){
        highp vec2 mod_UV;
        highp float time_fac = float(time % 10000) / 300.0;
        
        highp float modulation = 0.1 *
            sin(1.0 * UV.x + time_fac + 2.0 * UV.y);
        
        mod_UV.x = UV.x +
            modulation;
        mod_UV.y = UV.y;
        
        mod_UV *= 1.5;
        mod_UV -= vec2(0.25,0.25);
        
        color = texture(bg, mod_UV);
        
        if( mod_UV.x < 0.0 || mod_UV.x > 1.0 ||
            mod_UV.y < 0.0 || mod_UV.y > 1.0 ){
            color.rgb = vec3(0.0,0.0,0.0);
        }
    } else {
        color =
            abs(texture(pass_1,UV) -
                texture(pass_1,UV - vec2(pixel_width,0.0))) +
            abs(texture(pass_1,UV) -
                texture(pass_1,UV - vec2(0.0,pixel_height)));

    }
    color.a = 1.0;
}
