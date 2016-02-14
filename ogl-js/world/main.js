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
// causes segfault
log(JSON.stringify(this.parent));
