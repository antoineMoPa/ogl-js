#version 300 es

precision highp float;

out vec4 color;

in vec2 UV;
in vec3 pos_model;
in vec3 normal;
in vec3 light_pos;
in float light_power;

uniform sampler2D texSampler;


vec3 light_color = vec3(1.0f,1.0f,1.0f);

float grass_fac(float x, float y, float size){
    float grass = 1.0;
    x += 10.0 * floor(sin(y * 0.32 + y * 10.3));
    y += 13.033 * floor(sin(y * 0.52 + y * 14.3));
    
    grass *= sin(x * size);
    grass *= sin(y * size);
    grass = clamp(grass,0,1.0);
    grass = pow(grass,10.0);
    return grass;
}

void main(){
    //color = texture(texSampler,UV).rgb;
    //color = sin(pos_m);
    //color = vec3(UV.x,UV.y,0);

    float dist = distance(pos_model,light_pos);

    float fac = clamp(dot(normal,light_pos),0.0,1.0);
    
    // diffuse part
    vec3 diff_color = light_color * light_power *
         fac / (dist * dist);

    vec3 spec_color = light_color * light_power *
        pow(3,fac) / (dist * dist);
    
    vec4 grass_color = vec4(0.0,0.0,0.0,1.0);

    float x = pos_model.x;
    float y = pos_model.z;
    
    x -= 0.11;
    y -= 3.7;
    float grass = grass_fac(x,y,4000.0);
    grass_color = vec4(0.1,0.6,0.1,grass);
    
    color = vec4(diff_color,1.0);
    color += vec4(spec_color,1.0);
    color *= grass_color;
}
