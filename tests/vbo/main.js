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

    var a = mat1[0];
    var b = mat1[1];
    var c = mat1[2];
    
    var d = mat1[3];
    var e = mat1[4];
    var f = mat1[5];

    var g = mat1[6];
    var h = mat1[7];
    var i = mat1[8];

    var l = mat2.length;

    // I hate math
    // Math is nice
    
    for(var j = 0; j <= l-3; j+=3){
        // initialize
        var x = mat2[j + 0];
        var y = mat2[j + 1];
        var z = mat2[j + 2];
        
        mat2[j + 0] = a * x + b * y + c * z;
        mat2[j + 1] = d * x + e * y + f * z;
        mat2[j + 2] = g * x + h * y + i * z;
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

rotate_y(Math.PI*1.25,xy_xz_plane_vert);

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

    translate(-3,2 * Math.sin((new Date().getTime())/500),-2);
    
    render_triangle_array("xzplane");
    
    translate(0,0,-0.1);
    
    render_triangle_array("xyplane");
    
    translate(2,0,0);
    
    render_triangle_array("xyplane+xzplane");

    translate(2,0,0);

    render_triangle_array("xyplane+xzplane_rotated");
    
}
