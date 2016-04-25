#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D last_pass;
uniform sampler2D pass_0;
uniform sampler2D pass_1;
uniform sampler2D pass_2;
uniform highp float zoom;
uniform int time;
uniform int pass;
uniform int frame_count;
highp vec4 rand_var;

void main(){
    // Last post processed screen
    // Not used in this script
    highp vec4 last = texture(last_pass,UV);

    // Complex numbers
    highp vec2 z,c,old_z;

    // Z and C have a real and an imaginary part
    // C is initialized with screen coordinates
    
    z = vec2(0.00,0.00);

    c = zoom * vec2(UV.x - 0.5, UV.y - 0.5);
    // Move that a little
    c.x -= 0.5;

    int current_step = 0;

    int iterations = 10;

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
        // Set color according to current step
        color.r = 1.0 - float(current_step) / float(iterations);
        color.g = 0.0;
        color.b = color.r;
        color.a = 1.0;
    } else if(pass == 2){
        color = last;
    } else if(pass == 3){
        color = last;
    }
}
