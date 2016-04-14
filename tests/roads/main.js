
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

// Start random routes at specific points
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
    /*
     I created the coordinates using the default cube in Blender
     Steps:
     
     1 - Save as .obj (triangulate faces, write normals)
     2 - Replace manually faces in .obj file:
         f 1//2 4//2 4//4
         means:
         f <value of first vertex> // <value of 2nd normal>
     3 - Use that to create vertex and normal array here
     */
    var building = {
        vertex: [
            1.0,0.0,-1.0,1.0,0.0,1.0,-1.0,0.0,1.0,
            1.0,0.0,-1.0,-1.0,0.0,1.0,-1.0,0.0,-1.0,
            1.0,2.0,-1.0,-1.0,2.0,-1.0,-1.0,2.0,1.0,
            1.0,2.0,-1.0,-1.0,2.0,1.0,1.0,2.0,1.0,
            1.0,0.0,-1.0,1.0,2.0,-1.0,1.0,2.0,1.0,
            1.0,0.0,-1.0,1.0,2.0,1.0,1.0,0.0,1.0,
            1.0,0.0,1.0,1.0,2.0,1.0,-1.0,2.0,1.0,
            1.0,0.0,1.0,-1.0,2.0,1.0,-1.0,0.0,1.0,
            (-1.0),0.0,1.0,-1.0,2.0,1.0,-1.0,2.0,-1.0,
            (-1.0),0.0,1.0,-1.0,2.0,-1.0,-1.0,0.0,-1.0,
            1.0,2.0,-1.0,1.0,0.0,-1.0,-1.0,0.0,-1.0,
            1.0,2.0,-1.0,-1.0,0.0,-1.0,-1.0,2.0,-1.0,
        ],
        normal: [
            0.0,-1.0,0.0,0.0,-1.0,0.0,0.0,-1.0,0.0,
            0.0,-1.0,0.0,0.0,-1.0,0.0,0.0,-1.0,0.0,
            0.0,1.0,0.0,0.0,1.0,0.0,0.0,1.0,0.0,
            0.0,1.0,0.0,0.0,1.0,0.0,0.0,1.0,0.0,
            1.0,0.0,0.0,1.0,0.0,0.0,1.0,0.0,0.0,
            1.0,0.0,0.0,1.0,0.0,0.0,1.0,0.0,0.0,
            (-0.0),-0.0,1.0,-0.0,-0.0,1.0,-0.0,-0.0,1.0,
            (-0.0),-0.0,1.0,-0.0,-0.0,1.0,-0.0,-0.0,1.0,
            (-1.0),-0.0,-0.0,-1.0,-0.0,-0.0,-1.0,-0.0,-0.0,
            (-1.0),-0.0,-0.0,-1.0,-0.0,-0.0,-1.0,-0.0,-0.0,
            0.0,0.0,-1.0,0.0,0.0,-1.0,0.0,0.0,-1.0,
            0.0,0.0,-1.0,0.0,0.0,-1.0,0.0,0.0,-1.0,
        ],
        uv: [
            
        ]
    };
    return building;
}

// We will concatenate all building vertices & normals here
var buildings = {
    vertex: [
    ],
    normal: [
    ],
    uv: [
    ]
};

// Some small function to translate an array
function translate_3d(x,y,z,arr){
    for(var i = 0; i < arr.length; i+=3){
        arr[i+0] += x;
        arr[i+1] += y;
        arr[i+2] += z;
    }
}

// Create buildings in this loop
for(var i = 0; i < 600; i++){
    var building = new_building();
    // Rotate my a multiple of 90°
    var angle = Math.floor(3*(Math.random())) * Math.PI/2;

    // City surface radius
    var spread = 40;
    // Downtown surface radius
    var downtown_ring = 20;

    // Position "randomly"
    var x = Math.floor((Math.random()-0.5) * 8 * spread)/(4);
    var z = Math.floor((Math.random()-0.5) * 8 * spread)/(4);
    var y = 0;

    // Find distances from city center
    var dist_from_center = Math.sqrt(
        Math.pow(x,2) +
        Math.pow(z,2)
    );

    // Create a factor that is higher at city center
    // And 0 outside of downtown_ring
    function downtown_fac(dist){
        if(dist < downtown_ring){
            return 5*Math.pow(1-(dist/downtown_ring),2);
        } else {
            return 0;
        }
    }

    // Create height using many factors
    var height = Math.random() * 2.0 *
        downtown_fac(dist_from_center) + 1;

    // Scale according to height + random z axis factor
    multiply_matrix_3d([
        1,0,0,
        0,height,0,
        0,0,Math.random() * 0.5,
    ],building.vertex);

    // Rotate that building
    rotate_y(angle,building.vertex);

    // Translate that building
    translate_3d(x,y,z,building.vertex);

    // Add building to final array
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
    translate(0,-4,-30);

    // Rotate stuff according to time
    rotate(Math.sin(new Date().getTime()/8000)*2*Math.PI,0,1,0);
    
    render_triangle_array("roads");
    translate(0,0,-4);
    render_triangle_array("buildings");
}
