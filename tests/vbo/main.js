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

xy_xz_plane_vert = [
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
];

xy_xz_plane_normal = [
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0
];

xy_xz_plane_uv = [
    0,0,
    0,0,
    0,0,
    0,0,
    0,0,
    0,0
];

create_triangle_array(
    "xyplane+xzplane",
    xy_xz_plane_vert,
    xy_xz_plane_normal,
    xy_xz_plane_uv
);

/*
  Multiply matrices of 3d coords 
  multiply_matrix_3d(mat1,mat2) =
  [a,b,c,a2,b2,c2,a3,b3,c3] * [x,y,z,x2,y2,z2,...]
 */
function multiply_matrix_3d(mat1,mat2){
    var num;
    for(var i = 0; i < mat2.length / 3; i++){
        for(var j = 0; j < 3; j++){
            // initialize
            num = 0;
            for(var k = 0; k < 3; k++){
                num += mat1[3*j + k] * mat2[3*i+k];
            }
            mat2[3 * i + j] = num;
        }
    }
}

function rotate_y(theta,mat){
    // Build a rotation matrix
    var r = [
        Math.cos(theta),0,Math.sin(theta),
        0,1,0,
        (-Math.sin(theta)),0,Math.cos(theta)
    ];
    // Rotate
    multiply_matrix_3d(r,mat);
}

create_triangle_array(
    "xyplane+xzplane_rotated",
    xy_xz_plane_vert,
    xy_xz_plane_normal,
    xy_xz_plane_uv
);

var it = 0;

function render(){
    bind_shaders("main");
    it++;

    translate(-3,-2 + it/100,-2);
    
    render_triangle_array("xzplane");
    
    translate(0,0,-0.1);
    
    render_triangle_array("xyplane");
    
    translate(2,0,0);
    
    render_triangle_array("xyplane+xzplane");

    translate(2,0,0);

    render_triangle_array("xyplane+xzplane_rotated");
    
}
