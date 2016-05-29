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
                         UV + 6.0 * y_offset - x_offset);
        cell_2 = texture(pass_2,
                         UV + 6.0 * y_offset);
        cell_3 = texture(pass_2,
                         UV + 6.0 * y_offset + x_offset);
        
        cell_4 = texture(pass_2,
                         UV - 6.0 * y_offset - x_offset);
        cell_5 = texture(pass_2,
                         UV - 6.0 * y_offset);
        cell_6 = texture(pass_2,
                         UV - 6.0 * y_offset + x_offset);
        
        color = last;

        highp float factor =
            (sin(UV.y * 10.0 + color.g)) *
            (sin(UV.x * 10.0 + color.r));

        if(factor < 0.1 && factor > -0.1){
            factor += 0.1 + color.g;
        }

        factor = 1.0 + sin(UV.x * 100.0);
        
        color.r -=
            (factor) *
            ( cell_1.g - cell_4.b +
              cell_2.g - cell_5.b +
              cell_3.g + cell_6.b );

        color.g -=
            (factor) *
            ( cell_1.b - cell_4.r +
              cell_2.b + cell_5.r +
              cell_3.b - cell_6.r );

        color.b -=
            (factor) *
            ( cell_1.r + cell_4.g +
              cell_2.r - cell_5.g +
              cell_3.r + cell_6.g );

        
        if(abs(UV.y - 0.02) < pixel_heigth){
            color.rgb += vec3(0.1,0.3,0.1);
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
