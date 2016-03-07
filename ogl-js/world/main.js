
console.log("meow",1,2,3,{a:3,b:2});

//test();

function test(){
    // First things I tried to integrate
    // Should disapear soon
    log(String(plus(2,2)))
    log(String(divide(42,2)))

    // Make sure we can't use this ever
    if(typeof XMLHttpRequest == 'undefined'){
        log("XMLHttpRequest is undefined");
    }

    // What if... nope
    if(typeof console == 'undefined'){
        log("console is undefined");
    }

    // Is there a window?... nope
    if(typeof window == 'undefined'){
        log("window is undefined");
    }

    // Testing the log function
    log(JSON.stringify(this));
    log(JSON.stringify(this.parent));

    // Test render function
    var i = 0;
    render = function(){
        i++;
        translate(0.2,10 * Math.sin(i/100),0.2);
        scale(Math.sin(i/40),Math.sin(i/40),Math.sin(i%40));
        color(1,0.2,Math.sin(i/100),1);
        triangle_strip(
            [0.0,1.0,0.0,
             1.0,1.0,0.0,
             1.0,0.0,0.0,
             3.0,1.0,0.0,
             3.0,0.0,0.0,
             4.0,1.0,0.0]
        );
    }
}

var w = 10;
var h = 10;
var grid = new Array(w);
var colors = new Array(w);
for(var i = 0; i < w; i++){
    grid[i] = [];
    colors[i] = []
    for(var j = 0; j < h; j++){
        grid[i][j] = Math.random() * 4 + 1;
        colors[i][j] = Math.random();
    }
}

var step = 0;

render = function(){
    ++step;
    
    translate(-4,-1.2,-5.0);
    scale(0.1,0.1,0.1);

    for(var i = 0; i < w; i++){
        pushMatrix();
        translate(i*10,0,0);
        for(var j = 0; j < h; j++){
            translate(0,0,5.0);
            color(colors[i][j],0.1,0.4,0.4);
            grid[i][j] += Math.random() * 0.2 - 0.1;
            pushMatrix();
            rotate(0.1 * step,0,1,0);
            var scaleY = grid[i][j] + Math.sin((i+step)/4);
            scale(1,scaleY,1);
            render_model("../models/world.obj");
            popMatrix();
        }
        popMatrix();
    }
};

function cube(){
    var P1 = [0,0,1];
    var P2 = [1,0,1];
    var P3 = [0,0,0];
    var P4 = [1,0,0];
    var P5 = [0,1,1];
    var P6 = [1,1,1];
    var P7 = [0,1,0];
    var P8 = [1,1,0];

    var strips = [];

    strips.push(
        P3.concat(P1,P4,P2)
    );

    strips.push(
        P1.concat(P5,P2,P6)
    );

    strips.push(
        P8.concat(P6,P7,P5)
    );

    strips.push(
        P1.concat(P3,P5,P7)
    );

    strips.push(
        P4.concat(P8,P3,P7)
    );

    strips.push(
        P4.concat(P2,P8,P6)
    );

    // Draw faces
    for(var j in strips){
        var strip = strips[j];
        triangle_strip(
            strip
        );
    }
}


/*
triangles = [];
triangles_speed = [];

var step = 0;
render = function(){
    ++step;

    if(Math.random() < 0.1){
        triangles.push(0);
        triangles_speed.push((Math.random()-0.5) * 0.1);
    }

    translate(5,8,0);
    color(1,1,1,0.2);
    //scale(4,4,4)
    triangle_strip(triangles);

    for(var i = 0; i < triangles.length; i++){
        triangles_speed[i] += (Math.random()-0.5) * 0.00001;
        triangles[i] *= 1.0001;
        triangles[i] += triangles_speed[i];
    }
    log("i: "+i);
}
*/
//render = function(){
//    model_test();
//}
