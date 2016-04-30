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
uniform int wall;
uniform int reset;
uniform int pause;
uniform highp float ratio;
uniform highp float damp;
uniform highp float radius;
uniform highp float mouse_x;
uniform highp float mouse_y;
highp vec4 rand_var;

void main(){
    // We'll use this later
    highp vec4 last = texture(last_pass,UV);

    // Height, speed and resistance is stored in a texture
    // red = x = height
    // green = y = speed
    // etc.
    // Since we can't store a negative value,
    // values are given a 0.5 offset at storage
    // (Also: we cannot store values > 1.0)
    highp vec4 data = texture(pass_2,UV);
    highp float height = data.x - 0.5;
    highp float speed = data.y - 0.5;
    highp float resistance = data.z;

    // Ratio used to find distance between mouse and
    // current position
    highp vec2 uv_ratio = vec2(1.0,1.0 / ratio);

    // Compute mouse distance
    highp float mouse_dist =
        distance(UV * uv_ratio,
                 vec2(mouse_x,mouse_y) * uv_ratio);
    
    // Activate source if close enough to mouse
    if(mouse_dist < radius){
        if(resistance < 0.5){
            // Give this a sine in function of time
            height += 100.0 * sin(float(mod(time,10000))/100.0);
        }
    }
    
    if(frame_count == 0 || reset == 1){
        // Set initial conditions
        color = vec4(0.5,0.5,1.0,1.0);

        // Border width
        highp float border = 0.01;

        // Create border at window edges
        if(UV.x > border&&
           UV.x < 1.0 - border &&
           UV.y > border &&
           UV.y < 1.0 - border
           ){
            // No resistance here
            color.z = 0.0;
        }

        // Create wall
        if(wall == 1){
            // Double slit experiment
            if(UV.x > 0.30 && UV.x < 0.31){
                if(UV.y > 0.42 && UV.y < 0.46 ||
                   UV.y > 0.58 && UV.y < 0.62
                   ){
                    color.z = 0.0;
                } else {
                    color.z = 1.0;
                }
            }
        }
    } else if(pass == 1){
        // In pause, just leave here
        if(pause == 1){
            color = texture(pass_1,UV);
            return;
        }

        // Not the real pixel size
        // (Can be anything)
        highp float pixel_width = 0.003;
        // Create these values to find neighboring cells
        highp vec2 x_offset = vec2(pixel_width,0.00);
        highp vec2 y_offset = vec2(0.00,pixel_width * ratio);

        // Fluid transfer between neighboring cells
        speed -= 0.25 *
            (
             height - (texture(pass_2,UV + x_offset).x - 0.5) +
             height - (texture(pass_2,UV - x_offset).x - 0.5) +
             height - (texture(pass_2,UV + y_offset).x - 0.5) +
             height - (texture(pass_2,UV - y_offset).x - 0.5)
             );

        // Modify height according to speed
        // The factor is adjusted so that the
        // simulation works.
        // Weird things happen when this is to high
        height += 0.2 * speed;

        // Compute resistance to borders / walls / etc.
        speed *= 1.0 - resistance;

        // Damp speed
        // no damping = weird behaviour
        // to much damping = you don't see anything
        speed *= 0.998;
        
        // Damp when user uses the keyboard to damp stuff
        height *= 1.0 - damp;

        // We store data in the color
        color = vec4(
                     height + 0.5,
                     speed + 0.5,
                     resistance,
                     1.0
                     );
        
    } else if(pass == 2){
        // We do nothing here
        color = last;
    } else if(pass == 3){
        // Draw stuff
        // last.x = height
        // last.z = wall
        color.rgb = vec3(last.x + last.z);
        color.a = 1.0;
    }
}
