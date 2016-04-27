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
highp vec4 rand_var;

highp vec2 screen(){
    return zoom * vec2(UV.x * ratio - 0.5, UV.y - 0.5);
}

void main(){
    // Last post processed screen
    // Not used in this script
    highp vec4 last = texture(last_pass,UV);

    // Complex numbers
    highp vec2 z,c,old_z;

    if(fractal == 0){
        // Mandelbrot
        c = screen();
        c.x -= 1.0;
        
        c.x += x_offset;
        c.y += y_offset;
        
        z = vec2(0,0);
        
        //z.y = cos(float(time % 5000)/5000.0 * 2.0 * 3.1415);
    } else if (fractal == 1){
        // Julia
        z = screen();
        
        z.x += x_offset;
        z.y += y_offset;
        
        c = vec2(-0.835,-0.2321);
        
        c.y = cos(float(time % 5000)/5000.0 * 2.0 * 3.1415);
    }

    
    int current_step = 0;

    // Iterate and do math
    for(int i = 0; i < iterations; i++){
        // 'next value of z' = z ^2 + c
        // If you do the math:
        // z = a + bj
        // next z real part = a^2 - b^2
        // next z img part = 2 * a * b * i
        // next z img part = 2 * a * b * j (Engineers)

        // Keep the current (old) value
        old_z = z;

        // Set new values according to math
        z.x = pow(z.x,2.0) - pow(z.y,2.0);
        z.y = 2.0 * old_z.x * old_z.y;
        z += c;

        // If z goes out of some value (10.0), we know
        // it is not in the mandelbrot set
        // (So we'll color this place)
        if(distance(z,vec2(0.0,0.0)) > 10.0){
            break;
        }
        current_step++;
    }

    if(frame_count == 0){

    } else if(pass == 1){
        color = vec4(0.2,0.8,1.0,1.0);
        // Set color according to current step
        color.rgb *= float(current_step) / float(iterations);
    } else if(pass == 2){
        color = last;
    } else if(pass == 3){
        color = last;
    }
}
