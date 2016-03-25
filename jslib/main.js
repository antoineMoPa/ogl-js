var render;

var console = {};
console.log = function(){
    for(var i in arguments){
        var arg = arguments[i];
        if(typeof arg == 'object'){
            log(JSON.stringify(arg));
        } else {
            log(""+arg);
        }
    }
}
