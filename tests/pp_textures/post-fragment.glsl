#version 300 es

in highp vec2 UV;
out highp vec4 color;
uniform sampler2D bg;
uniform sampler2D last_pass;
uniform sampler2D pass_0;
uniform sampler2D pass_1;
uniform sampler2D pass_2;
uniform int time;
uniform int pass;
uniform int frame_count;
highp vec4 rand_var;

void main(){
    color = texture(bg,UV);
    color.a = 1.0;
}
