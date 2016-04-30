
enable_2_pass_pp();

var boat = {x:0.5,y:0.5,dx:0,dy:0,angle:0};
var pause = -1;

function render(){
    var speed = 0.7;
    var time = speed * new Date().getTime() / 10000;

    boat.x += boat.dx;
    boat.y += boat.dy;

    boat.dx *= 0.98;
    boat.dy *= 0.98;
    
    shader_var("boat_x",boat.x);
    shader_var("boat_y",boat.y);
    shader_var("boat_angle",boat.angle);
    shader_var("pause",pause);

    var current_angle = Math.atan2(boat.dy,boat.dx);
    boat.angle = boat.angle * 0.95 + 0.05 * current_angle;
    
    reset = 0;
}

function on_key(key, x, y){
    if(key == "w"){
        boat.dy += 0.001;
    } else if(key == "s"){
        boat.dy -= 0.001;
    } else if(key == "a"){
        boat.dx -= 0.001;
    } else if(key == "d"){
        boat.dx += 0.001;

    } else if(key == "p"){
        pause *= -1;
    }
}

function on_mouse_move(x, y){
    
}
