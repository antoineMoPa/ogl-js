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
    "plane",
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

var it = 0;

function render(){
    bind_shaders("main");
    it++;

    translate(-2,-2 + it/100,-2);
    
    render_triangle_array("plane");
}
