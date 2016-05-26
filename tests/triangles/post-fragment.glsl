#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D last_pass;
uniform sampler2D pass_0;
uniform sampler2D pass_1;
uniform sampler2D pass_2;
uniform int time;
uniform int pass;
uniform int screen_w;
uniform int screen_h;
uniform int frame_count;
uniform int js_frame_count;
uniform int reset;
uniform int pause;
uniform int rule;
uniform int source_type;
uniform highp float ratio;
uniform highp float radius;
uniform highp float mouse_x;
uniform highp float mouse_y;
highp vec4 rand_var;

void main(){
    // We'll use this later
    highp vec4 last = texture(last_pass,UV);

    highp float pixel_width = 1.0 / float(screen_w);
    highp float pixel_heigth = 1.0 / float(screen_h);
    
    // Create these values to find neighboring cells
    highp vec2 x_offset = vec2(pixel_width,0.00);
    highp vec2 y_offset = vec2(0.00,pixel_heigth);
    
    if(frame_count == 0 || reset == 1){
        if(distance(UV,vec2(0.5,0.95)) < float(pixel_width)){
            color = vec4(1.0,1.0,1.0,1.0);
        } else {
            color = vec4(0.0,0.0,0.0,1.0);
        }
    } else if(pass == 1) {
        int active_line = js_frame_count % screen_h;
        int this_line = int(
                            (1.0 - UV.y) *
                            float(screen_h));

        last = texture(pass_2,UV);

        if(last.x > 0.5){
            color = last;
            return;
        }
        
        if(pause == 1){
            color = last;
            return;
        }
            
        if(!(this_line == active_line)){
            color = last;
            return;
        }

        bool cell_1 = texture(pass_2,
                              UV + y_offset - x_offset).x > 0.5;
        bool cell_2 = texture(pass_2,
                              UV + y_offset).x > 0.5;
        bool cell_3 = texture(pass_2,
                              UV + y_offset + x_offset).x > 0.5;
        int num = 0;
        
        if(cell_1){
            num += 4;
        }
        if(cell_2){
            num += 2;
        }
        if(cell_3){
            num += 1;
        }
        
        bool next = false;

        // `rule`: 8 bits
        // `num`: 3 bits addressing `rule` bits
        //
        // `rule` indicates which cases of `num` will produce
        // an open pixel
        //
        // bitwise or (&) operator example:
        // 0010 0000 & 0010 0001 == 0010 0000
        //
        // Example with rule 3
        // Rule 3 = 0000 0011
        // So bits 1 and 2 are activated
        // Which means 2^0 and 2^1 is activated
        // 0000 0011 & 0000 0001 != 0 and
        // 0000 0011 & 0000 0010 != 0
        //
        // In these cases, the next state of the pixel is `1`
        //
        next = (rule & int(pow(2,num))) != 0;
        
        if(next){
            color.rgb = vec3(1.0);
        } else {
            color.rgb = vec3(0.0);
        }
        
        color.a = 1.0;
    } else if(pass == 2){
        // We do nothing here
        color = last;
    } else if(pass == 3){
        color = last;
        color.a = 1.0;
    }
}
