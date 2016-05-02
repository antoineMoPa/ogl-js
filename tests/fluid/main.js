
enable_2_pass_pp();

var mouse = {x:0,y:0};
var damp = 0;
var wall = 0;
var reset = 0;
var pause = -1;
var source_type = 0;
var shading = 0;
var radius = 0.003;

console.log("\nWelcome to the small 2D fluid playground\n");
console.log("d: (damp) Calm water down");
console.log("w: Toggle walls");
console.log("p: Pause");
console.log("r/R: Change source radius");
console.log("a: Change shading");
console.log("s: Change source type");
console.log("\nHave fun!");

function render(){
    var speed = 0.7;
    var time = speed * new Date().getTime() / 10000;
    //mouse.x = time % 1;
    //mouse.y = 0.5;
    shader_var("mouse_x",mouse.x);
    shader_var("mouse_y",mouse.y);
    shader_var("damp",damp);
    shader_var("wall",wall);
    shader_var("reset",reset);
    shader_var("radius",radius);
    shader_var("pause",pause);
    shader_var("source_type",source_type);
    shader_var("shading",shading);
    damp *= 0.8;
    reset = 0;
}

function on_key_down(key, x, y){
    if(key == "d"){
        damp+=0.1;
    } else if(key == "w"){
        wall++;
        wall = wall % 2;
        console.log("Wall: " + wall);
        reset = 1;
    } else if(key == "s"){
        source_type++;
        source_type = source_type % 2;
        console.log("Source type:" + source_type);
    } else if(key == "a"){
        shading++;
        shading = shading % 4;
        console.log("Shading:" + shading);
    } else if(key == "r"){
        radius += 0.005;
        console.log("Radius: " + radius);
    } else if(key == "R"){
        radius -= 0.005;
        console.log("Radius: " + radius);
    } else if(key == "p"){
        pause *= -1;
    }
}

function on_mouse_move(x, y){
    mouse.x = x;
    mouse.y = y;
}
