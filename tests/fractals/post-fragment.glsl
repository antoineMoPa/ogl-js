#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D last_pass;
uniform sampler2D pass_0;
uniform sampler2D pass_1;
uniform sampler2D pass_2;
uniform highp float zoom;
uniform highp float x_offset;
uniform highp float y_offset;
uniform highp float ratio;
uniform int iterations;
uniform int time;
uniform int pass;
uniform int frame_count;
uniform int fractal;
uniform int post_proc;
highp vec4 rand_var;

/*
  return coordinates from the screen
 */
highp vec2 screen(){
    return zoom * vec2(UV.x * ratio - 0.5, UV.y - 0.5);
}

/*
  Complex square
 */
highp vec2 to_the_2(highp vec2 z){
    highp vec2 old_z;
    // Keep the current (old) value
    old_z = z;
    
    // Set new values according to math
    z.x = pow(z.x,2.0) - pow(z.y,2.0);
    z.y = 2.0 * old_z.x * old_z.y;

    return z;
}

highp vec4 fractals(){
    // Complex numbers
    highp vec2 z,c;
    
    highp float time_fac = 1.0 *
        cos(float(time % 50000)/50000.0 * 2.0 * 3.1415 );

    // Initial conditions
    if(fractal == 0){
        // Mandelbrot
        c = screen();
        c.x -= 1.0;
        
        c.x += x_offset;
        c.y += y_offset;
        
        z = vec2(0,0);
        
        z.x = time_fac * sqrt(zoom);
    } else if (fractal == 1 || fractal == 2 || fractal == 3 || fractal == 4){
        // Julia
        z = screen();
        
        z.x += x_offset - 0.5;
        z.y += y_offset;
        
        if(fractal != 4){
            c = vec2(0.345,0.003);
        } else {
            c = vec2(-0.8,0.156);
        }
        
        c.y = 0.5 * time_fac * sqrt(zoom);
    }

    int current_step = 0;
    highp vec2 old_z;
    
    // Iterate and do math
    for(int i = 0; i < iterations; i++){
        // 'next value of z' = z ^2 + c
        // If you do the math:
        // z = a + bj
        // next z real part = a^2 - b^2
        // next z img part = 2 * a * b * i
        // next z img part = 2 * a * b * j (Engineers)

        old_z = z;
        
        z = to_the_2(z);
        
        // higher powers
        if(fractal == 2){
            z = to_the_2(z);
            z = to_the_2(z);
        }
        
        if(fractal == 3){
            z += c + old_z / to_the_2(old_z);
        } else {
            z += c;
        }

        // Note: this is cool
        //z *= 2.0 * sin(z);
        
        // If z goes out of some value (10.0), we know
        // it is not in the mandelbrot set
        // (So we'll color this place)
        // To get nice weird results:
        // z.x > 10.0
        // z.y > 10.0
        if(length(z) > 10.0){
            break;
        }
        current_step++;
    }
    
    highp vec4 color = vec4(0.2,0.8,1.0,1.0);
    // Set color according to current step
    color.rgb *= float(current_step) / float(iterations);

    return color;
}

void main(){
    // Last post processed screen
    // Not used in this script
    highp vec4 last = texture(last_pass,UV);
    
    if(frame_count == 0){
    } else if(pass == 1){
        color = fractals();
    } else if(pass == 2){
        if(post_proc == 0){
            color = last;
        } else {
            highp vec3 old = texture(pass_2,UV).rgb;
            
            highp vec2 x_offset = vec2(0.001,0.000);
            highp vec2 y_offset = vec2(0.000,0.001);
            
            color =
                texture(last_pass,UV + x_offset) -
                texture(last_pass,UV - x_offset) +
                texture(last_pass,UV + y_offset) -
                texture(last_pass,UV - y_offset);
            
            color = abs(color);
            color.a = 1.0;
            
            // Fun stuff:
            //color.rgb += old;
        }
    } else if(pass == 3){
        color = last;
    }
}
