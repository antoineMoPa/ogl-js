
enable_2_pass_pp();

var mouse = {x:0,y:0};
var reset = 0;
var pause = -1;
var source_type = 0;
var radius = 0.003;

console.log("p: Pause");
console.log("r/R: Change source radius");
console.log("s: Change source type");
console.log("\nHave fun!");

function render(){
    var speed = 0.7;
    var time = speed * new Date().getTime() / 10000;
    //mouse.x = time % 1;
    //mouse.y = 0.5;
    shader_var("mouse_x",mouse.x);
    shader_var("mouse_y",mouse.y);
    shader_var("reset",reset);
    shader_var("radius",radius);
    shader_var("pause",pause);
    shader_var("source_type",source_type);
    reset = 0;
}

function on_key_down(key, x, y){
    if(key == "s"){
        source_type++;
        source_type = source_type % 2;
        console.log("Source type:" + source_type);
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
