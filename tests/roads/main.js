var points = [
    (-2),0,0,
    0,0,0,
    0,0,-2,
    0,1,-4,
    0,1,-8,
    0,1,-12,
    0,0,-18,
    0,0,-22,
    2,0,-22,
    4,0,-22,
];

load_shaders("main","vertex.glsl","fragment.glsl");

var vertex = [];
var normal = [];

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

/*
  Route profile:
  
  Example:
  
         ^   /^^^^/|_________/^^^^/|  ^
  y axis |  /    /          /    //  /  z axis
         |  |^^^^|_________|^^^^|/  /

                 ----------->
                   x axis
                   
  Part with z < 0.5 is used as one end
  Part with z > 0.5 is used as the other end

  route() rotates and moves each end to fit the
  direction of the path.
*/

function base_route_shape(){
    vertex = vertex.concat(
        [
            0,0,0,
            0,0,1,
            1,0,0,
            1,0,0,
            0,0,1,
            1,0,1,
        ]
    );
    normal = normal.concat(
        [
            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0,
        ]
    );
    return {
        vertex: vertex,
        normal: normal
    };
}

function route(
    x1,y1,z1,
    x2,y2,z2,
    x3,y3,z3,
    x4,y4,z4,
    k
){
    var shape = base_route_shape();
    vertex = shape.vertex;
    normal = shape.normal;
}

function routes(points){
    for(var i = 0; i <= points.length - 6; i+=3){
        var x,y,z,x2,y2,z2;
        x = points[i+0];
        y = points[i+1];
        z = points[i+2];
        x2 = points[i+3];
        y2 = points[i+4];
        z2 = points[i+5];
        route(x,y,z,x2,y2,z2,0.99);
    }
}

routes(points);
rotate_y(0.2,vertex);

create_triangle_array(
    "roads",
    vertex,
    normal,
    []
);

function render(){
    bind_shaders("main");
    translate(0,-2,-5);
    //rotate(new Date().getTime()/10000 % 2 * Math.PI,0,1,0);
    render_triangle_array("roads");
}
