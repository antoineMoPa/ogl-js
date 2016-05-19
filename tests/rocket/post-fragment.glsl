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
uniform highp float rocket_x;
uniform highp float rocket_y;
uniform highp float rocket_angle;
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

highp float p(highp float x){
    if(x > 0.0){
        return x;
    } else {
        return 0.0;
    }
}

/* 
   Method:
   
   Return true if in 2 triangles formed by rectangle.
 */
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

highp vec2 u_for(highp vec4 data){
    highp float flow_velocity_x = data.g - 0.5;
    highp float flow_velocity_y = data.b - 0.5;
    highp vec2 u = vec2(flow_velocity_x,flow_velocity_y);
    return u;
}

highp float p_for(highp vec4 data){
    highp float pressure = data.r;
    return pressure;
}


void main(){
    highp vec4 last = texture(last_pass,UV);

    // Extract data
    highp vec4 data = texture(pass_2,UV);
    highp vec2 u = u_for(data);
    highp float p = p_for(data);
    
    highp float flow_velocity_x = data.g - 0.5;
    highp float flow_velocity_y = data.b - 0.5;

    // Not the real pixel size
    // (Can be anything)
    highp float pixel_width = 0.003;
    
    // Create these values to find neighboring cells
    highp vec2 x_offset = vec2(pixel_width,0.00);
    highp vec2 y_offset = vec2(0.00,-pixel_width * ratio);
    
    highp vec2 u_top = u_for(texture(pass_1,UV + y_offset));
    highp vec2 u_bottom = u_for(texture(pass_1,UV - y_offset));
    highp vec2 u_right = u_for(texture(pass_1,UV + x_offset));
    highp vec2 u_left = u_for(texture(pass_1,UV - x_offset));

    highp float p_top = p_for(texture(pass_1,UV + y_offset));
    highp float p_bottom = p_for(texture(pass_1,UV - y_offset));
    highp float p_right = p_for(texture(pass_1,UV + x_offset));
    highp float p_left = p_for(texture(pass_1,UV - x_offset));

    // Take screen ratio into account
    highp vec2 uv_ratio = vec2(1.0,1.0 / ratio);
    
    // Find distance between rocket an current UV
    highp float rocket_dist =
        distance(UV * uv_ratio,
                 vec2(rocket_x,rocket_y) * uv_ratio);
    
    // Rocket measurements
    highp float b_length = 0.05;
    highp float b_width = 0.05;
    highp float b_nose = 0.05;

    // Is pixel in rocket/motor area?
    bool is_rocket = false;
    bool is_motor = false;

    highp vec2 point = UV - vec2(rocket_x,rocket_y);
    // Screen is not a square so:
    point.x *= ratio;
    
    // Rocket rotation
    highp float r = length(point);
    highp float p_angle = atan(-point.y,point.x);
    p_angle += rocket_angle;
    
    highp vec2 rocket_vec = vec2(cos(p_angle),sin(p_angle));
    
    // Enter rocket render logic when close enough
    if(rocket_dist < 2.0 * b_length){
        point = r * rocket_vec;

        // This is where I drew the rocket
        // Rectangle part
        is_rocket = point_in_rect(
                                point,
                                vec2(-b_length,-b_width/2.0),
                                vec2(-b_length,b_width/2.0),
                                vec2(b_length,-b_width/2.0),
                                vec2(b_length,b_width/2.0)
                                );
        // Triangle (nose) part
        is_rocket = is_rocket ||
            point_in_triangle(
                              point,
                              vec2(b_length,-b_width/2.0),
                              vec2(b_length,b_width/2.0),
                              vec2(b_length + b_nose,0.0)
                              );

        // Motor area
        is_motor =
            point_in_rect
            (
             point,
             vec2(-b_length * 1.2, -b_width/8.5),
             vec2(-b_length * 1.2, b_width/8.5),
             vec2(-b_length, -b_width/4.5),
             vec2(-b_length, b_width/4.5));
        
        if(is_rocket){
            p = 0.4;
            u = -0.5 * rocket_vec;
        }
        
        // In motor area: oscillate
        if (is_motor){
            p = 0.4;
            u = -0.5 * rocket_vec;
        }
    }

    // Source
    if(distance(UV,vec2(0.25,0.75)) < 0.01){
        //u = vec2(0.0,-0.2);
        //p = 0.5;
    }
    if(distance(UV,vec2(0.8,0.25)) < 0.01){
        //u = vec2(0.0,0.2);
        //p = 0.5;
    }
    
    if(frame_count == 0 || reset == 1){
        // Set initial conditions
        color = vec4(0.5,0.5,0.5,1.0);
    } else if(pass == 1){
        color = texture(pass_2,UV);
    } else if(pass == 2){
        // In pause, just leave here
        if(pause == 1){
            color = texture(pass_2,UV);
            return;
        }
        
        highp vec2 du, dp;
        highp float p0;
        highp vec2 g = vec2(0.0,0.0);
        highp vec2 dudx, dudy;
        highp float dpdx, dpdy;
        highp vec2 nabla_u, nabla_p;
  
        dudx = u_right - u_left;
        dudy = u_top - u_bottom;
        
        p0 = 1.0;
        
        dpdx = p_right - p_left;
        dpdy = p_top - p_bottom;

        highp float time_fac = 0.1;

        // du = -u . del u + del w - g
        // del w = 1/p0 . del p
        du = -u * vec2(dudx.x,dudy.y) +
            1.0/p0 * vec2(dpdx, dpdy) - g;

        u += du * time_fac;

        // -u . del p - p del . u
        dp = -u * vec2(dpdx,dpdy) - p * vec2(dudy.x,dudy.y);
        
        p += length(dp) * time_fac;

        u = (u + u_top + u_bottom + u_left + u_right) / 5.0;
        
        // Border
        if( UV.y < 0.02 ||
            UV.y > 0.98 ||
            UV.x < 0.02 ||
            UV.x > 0.98 ){
            u = vec2(0.0,0.0);
        } else {
            
        }

        flow_velocity_x = u.x;
        flow_velocity_y = u.y;

        // We store data in the color
        color = vec4(
                     p,
                     flow_velocity_x + 0.5,
                     flow_velocity_y + 0.5,
                     1.0
                     );        
        
    } else if(pass == 3){
        if(!is_rocket){
            highp float fire = pow(p,1.3);
            highp float fire_red = pow(p,2.0);
            highp vec3 yellow = vec3(1.0, 1.0, 0.0);
            highp vec3 red = vec3(1.0, 0.0, 0.0);
            highp vec3 white = vec3(1.0, 1.0, 1.0);
            color.rgb = 3.0 * p * white;
            color.rgb -= fire_red * (1.0 - red);
            color.rgb -= fire * (1.0 - yellow);
            color.rgb = vec3(last.r);
            color = last;
        } else {
            // Rocket color
            color.rgb = vec3(0.3,0.1,0.0);
        }
        // Set alpha to 1
        color.a = 1.0;
    }
}
