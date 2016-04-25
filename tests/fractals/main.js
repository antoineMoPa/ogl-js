
enable_2_pass_pp();

var zoom = 2.5;

function render(){
    shader_var("zoom",zoom);
}

function on_key(key, x, y){
    if(key == "w"){
        zoom *= 0.99;
    } else if (key == "s"){
        zoom *= 1.01;
    }
    console.log(key);
}
