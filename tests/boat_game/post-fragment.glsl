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
uniform int reset;
uniform int pause;
uniform highp float ratio;
uniform highp float boat_x;
uniform highp float boat_y;
uniform highp float boat_angle;
uniform highp float boat_acc;
highp vec4 rand_var;

/*
  Method:
  
  Find base of vector in 2 of the triangle's angle.
  
  If all base components are positive, then the point
  is in the triangle.
 */
bool point_in_triangle(
                       highp vec2 p,
                       highp vec2 t1,
                       highp vec2 t2,
                       highp vec2 t3){

    highp vec2 u = t2 - t1;
    highp vec2 v = t3 - t1;
    highp vec2 w = t2 - t3;

    highp mat2 m =
        inverse(mat2(u,v));
    
    highp vec2 pt2 = m * (p - t1);
    
    if(pt2.x > 0.0 && pt2.y > 0.0){
        m = inverse(mat2(-v,w));
        pt2 = m * (p - t3);
        if(pt2.x > 0.0 && pt2.y > 0.0){
            return true;
        }
    }
    return false;
}

bool point_in_rect(
                   highp vec2 pt,
                   highp vec2 p1,
                   highp vec2 p2,
                   highp vec2 p3,
                   highp vec2 p4){
    if( point_in_triangle(pt,p1,p2,p3) ||
        point_in_triangle(pt,p2,p3,p4) ){
        return true;
    }
    return false;
}

void main(){
    highp vec4 last = texture(last_pass,UV);

    highp vec4 data = texture(pass_2,UV);
    highp float height = data.x - 0.5;
    highp float speed = data.y - 0.5;
    highp float resistance = data.z;

    highp vec2 uv_ratio = vec2(1.0,1.0 / ratio);

    highp float boat_dist =
        distance(UV * uv_ratio,
                 vec2(boat_x,boat_y) * uv_ratio);
    

    // Boat data
    highp float b_length = 0.05;
    highp float b_width = 0.05;
    highp float b_nose = 0.05;

    bool is_boat = false;
    bool is_motor = false;
    
    // Not the real pixel size
    // (Can be anything)
    highp float pixel_width = 0.003;
    
    // Create these values to find neighboring cells
    highp vec2 x_offset = vec2(pixel_width,0.00);
    highp vec2 y_offset = vec2(0.00,pixel_width * ratio);
    
    // Wave source

    if( distance(UV,vec2(0.0,0.0)) < 0.04 ||
        distance(UV,vec2(1.0,1.0)) < 0.04 ||
        distance(UV,vec2(0.0,1.0)) < 0.04 ||
        distance(UV,vec2(1.0,0.0)) < 0.04
        ){
        height = 0.5 * sin(float(mod(time,10000))/10.0);
    }
    
    // Enter boat render logic when close enough
    if(boat_dist < 2.0 * b_length){
        highp vec2 point = UV - vec2(boat_x,boat_y);
        point.x *= ratio;
        // Boat rotation
        highp float r = length(point);
        highp float p_angle = atan(-point.y,point.x);
        p_angle += boat_angle;
        
        point = r * vec2(cos(p_angle),sin(p_angle));

        is_boat = point_in_rect(
                                point,
                                vec2(-b_length,-b_width/2.0),
                                vec2(-b_length,b_width/2.0),
                                vec2(b_length,-b_width/2.0),
                                vec2(b_length,b_width/2.0)
                                );
        
        is_boat = is_boat ||
            point_in_triangle(
                              point,
                              vec2(b_length,-b_width/2.0),
                              vec2(b_length,b_width/2.0),
                              vec2(b_length + b_nose,0.0)
                              );

        is_motor =
            point_in_rect
            (
             point,
             vec2(-b_length * 1.3 * boat_acc, -b_width/8.5),
             vec2(-b_length * 1.3 * boat_acc, b_width/8.5),
             vec2(-b_length, -b_width/4.5),
             vec2(-b_length, b_width/4.5));
        
        if (is_boat){
            height *= 0.9;
        }
        if (is_motor){
            height = boat_acc * 0.1;
            if(mod(frame_count,5) < 2.0){
                height *= 0.0;
            }
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
        speed *= 0.99;
        height *= 0.999;

        // Limit too intense values
        if(abs(speed) > 0.5){
            speed *= 0.3;
            height *= 0.3;
        }
        
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
        //color.rgb = last.rgb + vec3(0.5,0.5,0.0);
        color.rgb = vec3(last.x + 0.5 + last.z);
        
        if(!is_boat){

            highp float dx = 0.1;
            
            highp float dyi =
                (texture(pass_2,UV + x_offset).x - 0.5) -
                (texture(pass_2,UV - x_offset).x - 0.5);
            
            highp float dyj =
                (texture(pass_2,UV + y_offset).x - 0.5) -
                (texture(pass_2,UV - y_offset).x - 0.5);

            
            highp vec3 norm_i = vec3(-dyi,0.0,-dx);
            highp vec3 norm_j = vec3(0.0,-dyj,-dx);
            highp vec3 normal = norm_i + norm_j;

            normal = normalize(normal);
            
            highp vec3 sun_direction = vec3(1.0,1.0,1.0);

            highp float diff = dot(-normal,sun_direction);
                
            color.rgb = diff * vec3(0.1,0.4,0.7);

            highp vec3 reflection = sun_direction -
                (2.0 * dot(sun_direction,normal) * normal);

            highp float spec = 0.2 *
                pow(dot(reflection,sun_direction),4.0);
            
            if(spec > 0.0){
                color.rgb += spec * vec3(.3,.4,.9);
            }
            color.rgb -= (height + 0.5) *
                vec3(0.3,0.2,0.4);
        } else {
            color.rgb = vec3(0.3,0.1,0.0);
        }
        color.a = 1.0;
    }
}
