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
uniform highp float ratio;
uniform highp float mouse_x;
uniform highp float mouse_y;
highp vec4 rand_var;

void main(){
    highp vec4 last = texture(last_pass,UV);

    highp vec4 data = texture(pass_2,UV);
    highp float height = data.x - 0.5;
    highp float speed = data.y - 0.5;
    highp float resistance = data.z;

    if(distance(UV,vec2(mouse_x,mouse_y)) < 0.01){
        if(resistance < 0.5){
            speed += 0.8 * sin(float(time%100000)/100.0);
        }
    }
    
    if(frame_count == 0){
        // here we store height
        color = vec4(0.5,0.5,1.0,1.0);
        highp float border = 0.01;
        if(UV.x > border&&
           UV.x < 1.0 - border &&
           UV.y > border &&
           UV.y < 1.0 - border
           ){
            // No resistance here
            color.z = 0.0;
        }
    } else if(pass == 1){
        // Compute speed
        highp float pixel_width = 0.001;
        highp vec2 x_offset = vec2(pixel_width,0.00);
        highp vec2 y_offset = vec2(0.00,pixel_width * ratio);
        
        highp float speed_fac = 0.24;
        
        speed -= speed_fac *
            (
             height - (texture(pass_2,UV + x_offset).x - 0.5) +
             height - (texture(pass_2,UV - x_offset).x - 0.5) +
             height - (texture(pass_2,UV + y_offset).x - 0.5) +
             height - (texture(pass_2,UV - y_offset).x - 0.5)
             );
        
        // Damp speed
        speed *= 0.9999;
                
        color = vec4(
                     height + 0.5,
                     speed + 0.5,
                     resistance,
                     1.0
                     );

    } else if(pass == 2){
        color = last;
        speed = color.y - 0.5;
        height = color.x - 0.5;
        resistance = color.z;
        height += (0.3 * speed * (1.0 - resistance));

        color = vec4(
             height + 0.5,
             speed + 0.5,
             resistance,
             1.0 );
        
    } else if(pass == 3){
        // Draw stuff
        color = last;
        color.a = 1.0;
    }
}
