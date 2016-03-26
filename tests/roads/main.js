var points = [
    0,0,0,
    1,0,-1,
    2,0,-2,
    3,0,-3,
    4,0,-4,
    4,0,-5,
    4,0,-6,
    4,0,-6,
    4,0,-6,
    3,0,-6,
    2,0,-6,
    1,0,-6,
    0,0,-6,
];
/*
var points = [
    1,0,-1,
    1,0,-2,
    1,0,-3,
    1,0,-4,
];

var points = [
    1,0,-2,
    2,0,-2,
    3,0,-2,
    4,0,-2,
];*/


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
    var vertex = 
        [
            0,0,0,
            0,0,1,
            1,0,0,
            1,0,0,
            0,0,1,
            1,0,1,
        ];
    
    // Move 0.5 left
    for(var i = 0; i < vertex.length; i+=3){
        vertex[i] -= 0.5;
    }
    var normal = 
        [
            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0,
            0,1,0,
        ];

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
    
}

// Calculates angle of 2d vector
function angle(dx,dy){
    if(dy == 0){
        return dx > 0 ? 0 : Math.PI;;
    }
    var m = dy/dx;
    var theta = Math.atan(m);
    if(dx < 0){
        theta += Math.PI;
    }
    return theta;
}

var final_shape = {vertex:[],normal:[]};

function routes(points){
    for(var i = 0; i <= points.length - 8; i+=3){
        // fetch points
        var x1 = points[i+0];
        var y1 = points[i+1];
        var z1 = points[i+2];
        var x2 = points[i+3];
        var y2 = points[i+4];
        var z2 = points[i+5];
        var x3 = points[i+6];
        var y3 = points[i+7];
        var z3 = points[i+8];

        // find angle of path ends
        var angle_begin = -angle(x2-x1,z2-z1) - Math.PI/2;
        var angle_end = -angle(x3-x2,z3-z2) - Math.PI/2;;
        
        // Get base model
        var shape = base_route_shape();
        var vertex = shape.vertex;
        var normal = shape.normal;

        // Move parts of model
        // to the right place
        for(var j = 0; j <= vertex.length-3; j += 3){
            if(vertex[j+2] < 0.5){
                // Begining of the path
                // bring back z to 0
                var arr = [vertex[j+0],vertex[j+1],0];

                // rotate
                rotate_y(angle_begin,arr);

                vertex[j+0] = arr[0];
                vertex[j+1] = arr[1];
                vertex[j+2] = arr[2];

                // translate
                vertex[j+0] += x1;
                vertex[j+1] += y1;
                vertex[j+2] += z1;
            } else {
                // End of the path

                var arr = [vertex[j+0],vertex[j+1],0];
                
                // Rotate
                rotate_y(angle_end,arr);
                vertex[j+0] = arr[0];
                vertex[j+1] = arr[1];
                vertex[j+2] = arr[2];

                // translate
                vertex[j+0] += x2;
                vertex[j+1] += y2;
                vertex[j+2] += z2;
            }
        }
        final_shape.vertex = final_shape.vertex
            .concat(vertex);
        final_shape.normal = final_shape.normal
            .concat(normal);
    }
}

routes(points);

create_triangle_array(
    "roads",
    final_shape.vertex,
    final_shape.normal,
    []
);

var it = 0;

function render(){
    bind_shaders("main");
    it++;
    translate(-2,-4 + it/100,-2);
    //rotate(new Date().getTime()/10000 % 2 * Math.PI,0,1,0);
    render_triangle_array("roads");
}
