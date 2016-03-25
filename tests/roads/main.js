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
    var r = [
        Math.cos(theta),0,Math.sin(theta),
        0,1,0,
        (-Math.sin(theta)),0,Math.cos(theta)
    ];
    multiply_matrix_3d(r,mat);
}

function route(x,y,z,x2,y2,z2,k){
    
    x2 = x + k * (x2 - x);
    y2 = y + k * (y2 - y);
    z2 = z + k * (z2 - z);
    
    console.log([x,y,z]);
    console.log([x2,y2,z2]);
    
    //x = x + x2;
    //y = y + z2;
    //z = z + z2;
    vertex = vertex.concat(
        [
            x,y,z,
            x2,y2,z2,
            x+1,y,z,
            x+1,y,z,
            x2,y2,z2,
            x2+1,y2,z2,
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

rotate_y(0.8,vertex);

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
