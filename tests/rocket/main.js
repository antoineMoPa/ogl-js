
enable_2_pass_pp();

var floor = 0.05;

var rocket = {
    x:0.5,
    y:floor,
    dx:0,
    dy:0,
    angle:Math.PI/2,
    dangle:0,
    acc:0
};

var pause = -1;
var keys = {};

function render(){
    var speed = 0.7;
    var time = speed * new Date().getTime() / 10000;

    if(pause != 1){
        rocket.x += rocket.dx;
        rocket.y += rocket.dy;
        rocket.angle += rocket.dangle;

        rocket.dy -= 0.00005;
        rocket.dx *= 0.99;
        rocket.dy *= 0.99;

        if(rocket.y < floor ){
            rocket.y = floor;
            if(Math.abs(rocket.dy) > 0.004){
                rocket.dy *= -1;
                if(rocket.angle > 0 && rocket.angle < Math.PI){
                } else {
                    rocket.angle = 2 * Math.PI - rocket.angle;
                }
            }
        }
        rocket.acc *= 0.95;
        rocket.dangle *= 0.93;
        manage_keys();
    }

    shader_var("rocket_x",rocket.x);
    shader_var("rocket_y",rocket.y);
    shader_var("rocket_acc",parseFloat(rocket.acc * 10000.0));
    shader_var("rocket_angle",rocket.angle);
    shader_var("pause",pause);

    reset = 0;
}

function accelerate(force){
    rocket.dx += force * Math.cos(rocket.angle);
    rocket.dy += force * Math.sin(rocket.angle);
    rocket.acc = force;
}

function manage_keys(){
    if(keys["w"] == true){
        accelerate(0.0001);
    }
    if(keys["s"] == true){
        accelerate(-0.00006);
    }
    if(keys["a"] == true){
        rocket.dangle += 0.003;
    }
    if(keys["d"] == true){
        rocket.dangle -= 0.003;
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
