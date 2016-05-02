
enable_2_pass_pp();

var boat = {x:0.5,y:0.5,dx:0,dy:0,angle:0,dangle:0,acc:0};
var pause = -1;
var keys = {};

function render(){
    var speed = 0.7;
    var time = speed * new Date().getTime() / 10000;

    if(pause != 1){
        boat.x += boat.dx;
        boat.y += boat.dy;
        boat.angle += boat.dangle;
        
        boat.dx *= 0.99;
        boat.dy *= 0.99;
        boat.acc *= 0.95;
        boat.dangle *= 0.93;
        manage_keys();
    }

    shader_var("boat_x",boat.x);
    shader_var("boat_y",boat.y);
    shader_var("boat_acc",boat.acc * 10000);
    shader_var("boat_angle",boat.angle);
    shader_var("pause",pause);

    reset = 0;
}

function accelerate(force){
    boat.dx += force * Math.cos(boat.angle);
    boat.dy += force * Math.sin(boat.angle);
    boat.acc = force;
}

function manage_keys(){
    if(keys["w"] == true){
        accelerate(0.00004);
    }
    if(keys["s"] == true){
        accelerate(-0.00003);
    }
    if(keys["a"] == true){
        boat.dangle += 0.003;
    }
    if(keys["d"] == true){
        boat.dangle -= 0.003;
    }
}

function on_key_down(key, x, y){
    on_key(key,x,y,true);
}

function on_key_up(key, x, y){
    on_key(key,x,y,false);
}

function on_key(key, x, y, down){
    keys[key] = down;

    if(key == "p"){
        if(down){
            pause *= -1;
        }
    }
}

function on_mouse_move(x, y){
    
}
