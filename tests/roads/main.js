create_triangle_array("roads",[1,2,3,2,3,5,2,2]);

function render(){
    translate(-1,0,0);
    scale(0.1,0.1,0.1);
    for(var i = 0; i < 70; i++){
        translate(0.3,0.1,0.1);
        render_triangle_array("roads");
    }
}
