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

    if(false){
        // Coordinates of the second point for blur
        highp vec2 coord;
        // Distance from first (current) point
        highp float dist_fac;
        // Above 3 is too much for my poor little chromebook
        int radius = 3;
        // Distance we move for each point
        highp float pixel_size = 0.002;
        // Parse a square around current pixel
        for(int i = -radius; i < radius; i++){
            for(int j = -radius; j < radius; j++){
                coord = uv;
                // Create coords of second point
                coord += vec2(pixel_size*float(i),
                              pixel_size*float(j));
                
                // Do not go to the other part of the screen
                // (Limit coords: 0 to 1 )
                if(coord.x > 1.0 || coord.y > 1.0 ||
                   coord.x < 0.0 || coord.y < 0.0){
                    // get out of here
                    continue;
                }
                
                // Find disance between point and (0,0)
                dist_fac = sqrt(pow(float(i),2.0) + pow(float(j),2.0))/3.0;
                
                // Circle shape (Remove for square)
                if(dist_fac > 1.0){
                    continue;
                }
                
                // Add color of this pixel to the current pixel
                color += dist_fac * texture(
                                            renderedTexture,
                                            coord
                                            );
            }
        }
        
        // Divide the color by area of the square
        // (Because we added many different pixels)
        // I could also have divided by the area of the circle
        // But this is shorter
        // And I have to study
        color /= pow(float(radius),2.0);
    }
}
