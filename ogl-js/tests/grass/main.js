var step = 0;

load_shaders("grass","grass/vertex.glsl","grass/fragment.glsl");
load_shaders("ground","ground/vertex.glsl","ground/fragment.glsl");
load_shaders("building",
             "../../world/vertex.glsl",
             "../../world/fragment.glsl");

render = function(){
    ++step;

    
    
    translate(-50,-3,-50.0);
    scale(100,100,100);

    push_matrix();
    
    translate(0.5,-0.02,0.4);
    scale(0.04,0.04,0.04);

    bind_shaders("building");
    rotate(step/100,0,1,0);
    render_model("../../models/test_3d.obj");
    
    pop_matrix();
    
    bind_shaders("ground");
    plane();
    
    bind_shaders("grass");
    for(var i = 0; i < 20; i++){
        translate(0,0.0001,0);
        translate(
            Math.sin(i) * 0.00008,
            0,
            Math.sin(i) * 0.00008
        );
        plane();
    }
};

function plane(){
    var P1 = [0,0,1];
    var P2 = [1,0,0];
    var P3 = [1,0,1];
    var P4 = [0,0,0];

    strips = [];
    
    strips.push(
        P1.concat(P2,P3,P4,P1)
    );

    // Draw faces
    for(var j in strips){
        var strip = strips[j];
        triangle_strip(
            strip
        );
    }
}
