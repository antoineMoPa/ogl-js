log(String(plus(2,2)))
log(String(divide(42,2)))

if(typeof XMLHttpRequest == 'undefined'){
    log("XMLHttpRequest is undefined");
}

if(typeof console == 'undefined'){
    log("console is undefined");
}

if(typeof window == 'undefined'){
    log("window is undefined");
}

log(JSON.stringify(this));

log(JSON.stringify(this.parent));

var i = 0;

function render(){
    i++;
    translate(0.2,10 * Math.sin(i/100),0.2);
}
