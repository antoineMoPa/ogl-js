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
    float dist = distance(pos_model,light_pos);

    float fac = clamp(dot(normal,light_pos),0.0,1.0);
    
    // diffuse part
    vec3 diff_color = light_color * light_power *
        fac / (dist * dist);
    
    // Streetlamp light
    vec3 lamp_color = vec3(0.8,0.4,0.2);

    // Less intensity towards the center of the road
    lamp_color *= pow(UV.x - 0.5,2.0);
    // Less intensity at edges of road part
    lamp_color *= pow(UV.y - 0.5,1.1);
    lamp_color *= 10.0;

    // Street marks

    vec3 yellow_marks = vec3(0.9,0.7,0.0);
    vec3 white_marks = vec3(0.9,0.9,0.9);

    // Divide the road at 40
    // Put marks at desired places
    int x_division = int(UV.x * 80.0);
    int y_division = int(UV.y * 6.0);

    // 2 Yellow marks at the middle of the road 
    if(x_division != 39 && x_division != 41){
        // Remove what's left
        yellow_marks *= 0.0;
    }

    // White marks at 2 sides of the road.
    if(x_division % 20 != 0 || x_division == 40 || x_division == 0){
        // Remove what's left
        white_marks *= 0.0;
    }

    // Alternate white marks
    // Like that:
    //
    //  --  --  --  --  --
    //
    if(y_division % 2 == 0){
        white_marks *= 0.0;
    }

    // Darker in lanes
    float bump_color = 0.1 * sin(UV.x * 30.0);
    
    // Put all the colors together
    color = vec4(diff_color +
                 lamp_color +
                 yellow_marks +
                 vec3(bump_color)+
                 white_marks
                 ,1.0);
}
