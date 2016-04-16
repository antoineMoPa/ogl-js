#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D renderedTexture;
uniform int time;

void main(){
    highp vec2 uv = UV;
        
    // fetch color
    color = texture(renderedTexture, uv);

    // Vignette effect (darker corners)
    // Distance from center:
    highp float dist_from_center = distance(vec2(0.5,0.5),uv);
    color *= 1.0 - pow(dist_from_center,2.0);

    highp vec2 coord;
    highp float dist_fac;
    // Above 3 is too much for my poor little chromebook

    int radius = 3;
    highp float pixel_size = 0.002;
    for(int i = -radius; i < radius; i++){
        for(int j = -radius; j < radius; j++){
            coord = uv;
            coord += vec2(pixel_size*float(i),
                          pixel_size*float(j));
            
            if(coord.x > 1.0 || coord.y > 1.0 ||
               coord.x < 0.0 || coord.y < 0.0){
                continue;
            }

            dist_fac = sqrt(pow(i,2.0) + pow(j,2.0))/3.0;

            if(dist_fac > 1.0){
                continue;
            }
            
            color += dist_fac * texture(
                             renderedTexture,
                             coord
                             );
        }
    }
    color /= pow(radius,2.0);
}
