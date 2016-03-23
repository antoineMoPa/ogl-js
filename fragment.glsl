#version 300 es

// Default fragment shader

precision highp float;

out vec4 color;

in vec2 UV;
in vec3 pos_model;
in vec3 normal;
in vec3 light_pos;
in float light_power;

uniform vec3 kd;
uniform vec3 ka;
uniform vec3 ks;
uniform sampler2D texSampler;

vec3 light_color = vec3(1.0f,1.0f,1.0f);

void main(){
    //color = texture(texSampler,UV).rgb;
    //color = vec3(UV.x,UV.y,0);

    float dist = distance(pos_model,light_pos);

    float fac = clamp(dot(normal,light_pos),0.0,1.0);
    
    // diffuse part
    vec3 diff_color = kd * light_color * light_power *
        fac / (dist * dist);

    float ambiant = 0.1;
    vec3 ambi_color = ambiant * ka;
    
    color = vec4(diff_color + ambi_color,1.0);
}
