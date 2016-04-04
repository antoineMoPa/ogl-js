load_shaders("main","vertex.glsl","fragment.glsl");


/*
  Vertex
  Normal
  UV
  
  x
          1,1
  *     *

   0,0
  *     * z
  
  0,0
  1,0
  0,1
  0,1
  1,0
  1,1

*/
create_triangle_array(
    "xzplane",
    [
        0,0,0,
        1,0,0,
        0,0,1,
        0,0,1,
        1,0,0,
        1,0,1
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
        0,0,
        0,0,
        0,0,
        0,0,
        0,0,
        0,0
    ]
);

create_triangle_array(
    "xyplane",
    [
        0,0,0,
        1,0,0,
        0,1,0,
        0,1,0,
        1,0,0,
        1,1,0
    ],
    [
        0,0,1,
        0,0,1,
        0,0,1,
        0,0,1,
        0,0,1,
        0,0,1
    ],
    [
        0,0,
        0,0,
        0,0,
        0,0,
        0,0,
        0,0
    ]
);

create_triangle_array(
    "xyplane+xzplane",
    [
        0,0,0,
        1,0,0,
        0,1,0,
        0,1,0,
        1,0,0,
        1,1,0,

        0,0,0,
        1,0,0,
        0,0,1,
        0,0,1,
        1,0,0,
        1,0,1
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
        0,0,
        0,0,
        0,0,
        0,0,
        0,0,
        0,0
    ]
);



var it = 0;

function render(){
    bind_shaders("main");
    it++;

    translate(-2,-2 + it/100,-2);
    
    render_triangle_array("xzplane");
    
    translate(0,0,-0.1);
    
    render_triangle_array("xyplane");

    translate(2,0,0);
    
    render_triangle_array("xyplane+xzplane");

}
