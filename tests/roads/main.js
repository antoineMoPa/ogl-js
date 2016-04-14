
var points = [
    1,0,-1,
    1,0,-2,
    1,0,-3,
    1,0,-4,
    2,0,-5,
    3,0,-5,
    4,0.3,-5,
    5,0.3,-5,
    6,0,-5,
    7,0,-5,
    8,0,-5,
    9,0,-5,
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
    var theta = Math.atan2(dy,dx);
    return theta;
}

var final_shape = {vertex:[],normal:[]};

function routes(points){
    for(var i = 0; i < points.length - 9; i+=3){
        // fetch 3 points
        // P1
        var x1 = points[i+0];
        var y1 = points[i+1];
        var z1 = points[i+2];
        // P2
        var x2 = points[i+3];
        var y2 = points[i+4];
        var z2 = points[i+5];
        // P3
        var x3 = points[i+6];
        var y3 = points[i+7];
        var z3 = points[i+8];

        // find angle of path ends
        var angle_begin = angle(x2-x1,z2-z1) - Math.PI/2;
        var angle_end = angle(x3-x2,z3-z2) - Math.PI/2;
        
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

function random_routes(x,y,z){
    var points = [];
    
    // Current point
    var currpoint = [x,y,z];
    // Current direction
    var currdir = [0,0,-1];
    
    // Add points in random directions
    for(var i = 0; i < 80; i++){
        points = points.concat(currpoint);


        if(Math.random() < 0.2){
            if(Math.random() <= 1/4){
                currdir[0] = 1;
                currdir[1] = 0;
                currdir[2] = 0;
            } else if(Math.random() <= 1/3){
                currdir[0] = -1;
                currdir[1] = 0;
                currdir[2] = 0;
            } else if(Math.random() <= 1/2){
                currdir[0] = 0;
                currdir[1] = 0;
                currdir[2] = 1;
            } else {
                currdir[0] = 0;
                currdir[1] = 0;
                currdir[2] = -1;
            }
        }
        
        currpoint[0] += currdir[0];
        currpoint[1] += currdir[1];
        currpoint[2] += currdir[2];
    }
    return points;
}

for(var i = 0; i < 20; i++){
    var x,y,z;
    x = y = z = 0;
    x = Math.random() * 20 - 10;
    z = Math.random() * 20 - 10;
    y = Math.floor(Math.random() * 10 - 5) / 4;
    routes(random_routes(x,y,z));
}

create_triangle_array(
    "roads",
    final_shape.vertex,
    final_shape.normal,
    []
);

function new_building(){
    var building = {
        vertex: [
            0,0,0,
            10,0,0,
            0,10,0,
            0,10,0,
            10,0,0,
            10,10,0
        ],
        normal: [
            0,0,1,
            0,0,1,
            0,0,1,
            0,0,1,
            0,0,1,
            0,0,1
        ],
        uv: [
            0,0,
            0,0,
            0,0,
            0,0,
            0,0,
            0,0
        ]
    };
    return building;
}

var buildings = {
    vertex: [
    ],
    normal: [
    ],
    uv: [
    ]
};

for(var i = 0; i < 10; i++){
    var building = new_building();
    multiply_matrix_3d([
        1,0,0,
        Math.random(),1,0,
        0,Math.random(),1,
    ],building.vertex);
    buildings.vertex = buildings.vertex.concat(building.vertex);
    buildings.normal = buildings.normal.concat(building.normal);
    buildings.uv = buildings.uv.concat(building.uv);
}

create_triangle_array(
    "buildings",
    buildings.vertex,
    buildings.normal,
    buildings.uv
);

var it = 0;

function render(){
    bind_shaders("main");
    it++;
    translate(0,-4,-10);
    //angle = 0 + Math.sin((new Date().getTime())/1000);
    angle = 0;
    rotate(Math.sin(new Date().getTime()/8000)*2*Math.PI,0,1,0);
    
    render_triangle_array("roads");
    translate(0,0,-4);
    render_triangle_array("buildings");
}
