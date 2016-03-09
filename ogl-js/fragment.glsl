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
    
    color = vec4(diff_color,1.0) + vec4(spec_color,1.0);
}
