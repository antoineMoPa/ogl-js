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
        if(distance(UV,vec2(0.5,0.05)) < 3.0 * float(pixel_width)){
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
            //color = last;
            //return;
        }
        
        if(pause == 1){
            color = last;
            return;
        }
            
        if(!(this_line == active_line)){
            //color = last;
            //return;
        }

        highp vec4 cell_1, cell_2, cell_3,
            cell_4, cell_5, cell_6;
        
        cell_1 = texture(pass_2,
                         UV - 2.0 * y_offset - 1.0 * x_offset);
        cell_2 = texture(pass_2,
                         UV - 2.0 * y_offset);
        cell_3 = texture(pass_2,
                         UV - 1.0 * y_offset + 1.0 * x_offset);

        color = last;

        int num;

        if (this_line % 30 != 0 || this_line % 20 < 4) {
            color.r += cell_1.r + cell_3.g * 0.1;
            color.g += cell_2.g;
            color.b += cell_3.b;
        } else {
            color.r += cell_1.g;
            color.g += cell_2.b;
            color.b += cell_3.r;
        }

        color *= (sin(UV.y * 30.0 + color.r) + 1.4);
        color *= (sin(UV.x * 30.0 + color.g) + 1.4);
        
        color.a = 1.0;
    } else if(pass == 2){
        // We do nothing here
        color = last;
    } else if(pass == 3){
        color = last;
        color.a = 1.0;
    }
}
