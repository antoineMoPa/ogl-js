
enable_2_pass_pp();

var boat = {x:0.5,y:0.5,dx:0,dy:0,angle:0,dangle:0};
var pause = -1;

function render(){
    var speed = 0.7;
    var time = speed * new Date().getTime() / 10000;

    boat.x += boat.dx;
    boat.y += boat.dy;
    boat.angle += boat.dangle;
    
    boat.dx *= 0.99;
    boat.dy *= 0.99;
    boat.dangle *= 0.93;
    
    shader_var("boat_x",boat.x);
    shader_var("boat_y",boat.y);
    shader_var("boat_angle",boat.angle);
    shader_var("pause",pause);

    reset = 0;
}

function accelerate(force){
    boat.dx += force * Math.cos(boat.angle);
    boat.dy += force * Math.sin(boat.angle);
}

function on_key(key, x, y){
    if(key == "w"){
        accelerate(0.0001);
    } else if(key == "s"){
        accelerate(-0.0001);
    } else if(key == "a"){
        boat.dangle += 0.01;
    } else if(key == "d"){
        boat.dangle -= 0.01;
    } else if(key == "p"){
        pause *= -1;
    }
}

function on_mouse_move(x, y){
    
}
