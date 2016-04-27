
/*
  
  This test tests:
  
  - keyboard interaction
  - post processing shaders
  - passing values to post processing shaders
  
 */

enable_2_pass_pp();

var zoom = 2.5;
var x_offset = 0.0;
var y_offset = 0.0;
var iterations = 10;
var fractal = 0;

console.log("\n    WELCOME TO THE SMALL MANDELBROT BROWSER    \n");
console.log("wasd to move, +(=)/- to zoom/unzoom, i to iterate more.");

function render(){
    shader_var("zoom",zoom);
    shader_var("x_offset",x_offset);
    shader_var("y_offset",y_offset);
    shader_var("iterations",iterations);
    shader_var("fractal",fractal % 2);
}

function on_key(key, x, y){
    if(key == "="){
        zoom *= 0.95;
    } else if (key == "-"){
        zoom *= 1.05;
    } else if (key == "a"){
        x_offset -= 0.03 * zoom;
    } else if (key == "d"){
        x_offset += 0.03 * zoom;
    } else if (key == "s"){
        y_offset -= 0.03 * zoom;
    } else if (key == "w"){
        y_offset += 0.03 * zoom;
    } else if (key == "i"){
        iterations++;
    } else if (key == "f"){
        fractal++;
    }
}
