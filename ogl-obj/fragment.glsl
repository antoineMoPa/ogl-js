#version 300 es

precision highp float;

out vec3 color;

in vec2 UV;
in vec3 pos;
in vec3 normal;
uniform sampler2D texSampler;

vec3 light_pos = vec3(-10.0f,70.0f,10.0f);
vec3 light_color = vec3(1.0f,1.0f,0.8f);
float light_power = 1000.0;

void main(){
    color = texture(texSampler,UV).rgb;
    vec3 pos_m = pos;
    //color = sin(pos_m);
    //color = vec3(UV.x,UV.y,0);

    float dist = distance(pos,light_pos);
    // diffuse part
    vec3 diff_color = light_color * light_power *
        clamp(dot(normal,light_pos),0.0,1.0) / (dist * dist);

    // Ambiant part
    vec3 ambiant_color = light_color * 0.4;
    color = ambiant_color + diff_color;
}
