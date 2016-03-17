var render;

var console = {};
console.log = function(){
    for(i in arguments){
        var arg = arguments[i];
        if(typeof arg == 'object'){
            log(JSON.stringify(arg));
        } else {
            log(""+arg);
        }
    }
}
