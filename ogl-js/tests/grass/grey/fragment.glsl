#version 300 es

precision highp float;

out vec4 color;

in vec2 UV;
in vec3 pos;
in vec3 normal;
uniform sampler2D texSampler;

vec3 light_pos = vec3(10.0f,10.0f,0.0f);
vec3 light_color = vec3(1.0f,1.0f,1.0f);
float light_power = 100.0;

void main(){
    //color = texture(texSampler,UV).rgb;
    vec3 pos_m = pos;
    //color = sin(pos_m);
    //color = vec3(UV.x,UV.y,0);

    float dist = distance(pos,light_pos);

    // diffuse part
    vec3 diff_color = light_color * light_power *
        dot(normal,light_pos) / (dist * dist);

    float window =
        sin(pos_m.x * 20.0) *
        sin(pos_m.y * 20.0 + 0.2) *
        sin(pos_m.z * 20.0 + 0.5) * sin((pos_m.x + pos_m.y + pos_m.x)*10.0);

    color = vec4(window+0.1,window+0.1,0.0,1.0);
}
