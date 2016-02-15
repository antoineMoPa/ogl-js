var render;

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

render = function(){
    // Create cube
    // Points
    var P1 = [0,0,1];
    var P2 = [1,0,1];
    var P3 = [0,0,0];
    var P4 = [1,0,0];
    var P5 = [0,1,1];
    var P6 = [1,1,1];
    var P7 = [0,1,0];
    var P8 = [1,1,0];

    var strip = [];

    strip = strip.concat(
        P3,P7,P8,P3,P4,P1,P4,P2
    );
    
    // Draw faces
    color(1,1,0.5,0.2);
    
    triangle_strip(
        strip
    );
};
