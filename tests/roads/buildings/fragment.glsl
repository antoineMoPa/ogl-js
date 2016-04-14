#version 300 es

precision highp float;

layout(location=0) out vec4 color;

in vec2 UV;
in vec3 pos_model;
in vec3 normal_model;
in vec3 normal;
in vec3 light_pos;
in float light_power;

vec3 light_color = vec3(1.0f,1.0f,1.0f);

void main(){
    //color = texture(texSampler,UV).rgb;
    //color = sin(pos_m);
    //color = vec3(UV.x,UV.y,0);

    float dist = distance(pos_model,light_pos);

    float fac = 0.3 * clamp(dot(normal,light_pos),0.0,1.0);
    
    // diffuse part
    vec3 diff_color = light_color * light_power *
         fac / (dist * dist);

    //vec3 spec_color = light_color * light_power *
    //    pow(3.0,fac) / (dist * dist);
    
    color = vec4(diff_color,1.0);
    //color += vec4(1.0,1.0,1.0,1.0);

    vec4 window_color = vec4(1.0,0.8,0.4,1.0);

    int x_id;
    float scale = 10.0;
    
    // If on x axis
    if(abs(normal_model.x) > 0.1){
        x_id = int(pos_model.z * scale);
    }
    // If on z axis
    else {
        x_id = int(pos_model.x * scale);
    }
    
    int y_id = int(pos_model.y * scale);
    
    // Roof is not pointed towards top
    float is_roof = normal_model.y;
    // Wall is the rest
    float is_wall = 1.0 - is_roof;

    float rand_window = sin(
                         float(x_id) * 4.0 +
                         float(y_id) * 2.0 +
                         float(y_id) * 0.4 * sin(float(x_id))
                         
                         );
    
    if(x_id % 2 == 0 && y_id % 2 == 0 && rand_window > 0.0){
        color += window_color * is_wall * sin(rand_window);
    }
    color.a = 1.0;
}
