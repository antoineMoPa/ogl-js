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

var step = 0;

render = function(){
    ++step;
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
    
    rotate(step*2,1,1,0);
    
    scale(2,2,2);
    
    color(1,0,0,0.6);
    
    // Draw faces
    for(var j in strips){
        switch(parseInt(j)){
        case 0:
            color(1,1,1,1);
            break;
        case 1:
            color(0,1,1,1);
            break;
        case 2:
            color(0,0,1,1);
            break;
        case 3:
            color(1,0,0,1);
            break;
        case 4:
            color(0,1,0,1);
            break;
        case 5:
            color(1,0,1,1);
            break;
        default:
            color(0,0,1,1);
            break;
        }
        var strip = strips[j];
        triangle_strip(
            strip
        );
    }
};
