create_triangle_array(
    "roads",
    [
        0,1,0,
        0,1,1,
        1,1,0,
        1,1,0,
        0,1,1,
        1,1,1
    ],
    [
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0,
        0,1,0
    ],
    [
        0,1,0,
        0,1,1,
        1,1,0,
        1,1,0,
        0,1,1,
        1,1,1
    ]
);

load_shaders("main","vertex.glsl","fragment.glsl");

function render(){
    bind_shaders("main");
    translate(-3,-2,2);
    //scale(0.1,0.1,0.1);
    for(var j = 0; j < 10; j++){
        translate(2,0,-2);
        push_matrix();
        for(var i = 0; i < 20; i++){
            translate(0,0.1,0);
            render_triangle_array("roads");
        }
        pop_matrix();
    }
}
