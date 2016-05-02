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
uniform int shading;
uniform int source_type;
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

    if(source_type == 0){
        // Activate source if close enough to mouse
        if(mouse_dist < radius){
            if(resistance < 0.5){
                // Give this a sine in function of time
                height += 100.0 *
                    sin(float(mod(time,10000))/100.0);
            }
        }
    } else if(source_type == 1){
        if( abs(UV.x - mouse_x) < 0.01 &&
            UV.y < 0.8 &&
            UV.y > 0.2 ){
            height += 100.0 *
                sin(float(mod(time,10000))/100.0);
        }
    }


    // Not the real pixel size
    // (Can be anything)
    highp float pixel_width = 0.002;
    
    // Create these values to find neighboring cells
    highp vec2 x_offset = vec2(pixel_width,0.00);
    highp vec2 y_offset = vec2(0.00,pixel_width * ratio);
    
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
        // Find gradient
        // Arbitrary space between pixels
        highp float dx = 0.1;
        // Height delta in one axis
        highp float dyi =
            (texture(pass_2,UV + x_offset).x - 0.5) -
            (texture(pass_2,UV - x_offset).x - 0.5);
        // Height delta in other axis
        highp float dyj =
            (texture(pass_2,UV + y_offset).x - 0.5) -
            (texture(pass_2,UV - y_offset).x - 0.5);
        
        // Compute normal
        highp vec3 norm_i = vec3(-dyi,0.0,-dx);
        highp vec3 norm_j = vec3(0.0,-dyj,-dx);
        highp vec3 normal = norm_i + norm_j;
        
        // Normalize it
        normal = normalize(normal);
        
        // Scene data
        // Sun:
        highp vec3 sun_direction = vec3(1.0,1.0,1.0);
        // lamp color
        highp vec3 lamp = vec3(0.3,0.4,0.8);
        
        // Reflection vector for spec
        highp vec3 reflection = sun_direction -
            (2.0 * dot(sun_direction,normal) * normal);
        
        // Draw stuff
        if(shading == 0){
            // Diffuse factor
            highp float diff = dot(-normal,sun_direction);
            color.rgb = diff * lamp;

            // Specular factor
            highp float spec = 0.003 *
                pow(dot(reflection,sun_direction),8.0);

            // Use specular factor
            color.rgb += spec * lamp;

            // Modulate with height
            // (I'm so artistic)
            color.rgb -= (height + 0.9) *
                vec3(0.3,0.2,0.4);

            // Ambiant light
            color.rgb += vec3(0.0,0.1,0.1);
            
            color.rgb += last.z * vec3(1.0);
        } else if (shading == 1){
            // last.x = height
            // last.z = wall
            color.rgb = vec3(last.x + last.z);
        } else if (shading == 2){
            color.rgb = normal;
        } else if (shading == 3){
            color.rgb = reflection;
        }
        
        color.a = 1.0;
    }
}
