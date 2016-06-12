# OGL-JS

This project embeds Spidemonkey 24 into a C++ application with JS scripting. The JS code has access a coder-friendly set of 3d functions. You can use GLSL to create everything you can think of. 

## Currently available functions

**Log stuff for debugging**

* `console.log(stuff)` - will log stuff (And help you find the solution to all your problems?)

**OpenGL**

* `translate(float x,float y,float z)` - Translates next commands
* `scale(float x,float y,float z)` - Scales next commands
* `rotate(float angle, float x, float y, float z)` - Rotates `angle` around axis `x,y,z`
* `pushMatrix()` - Used to store translations, scales, rotations
* `popMatrix()` - Used to retrieve the pushed state
* `triangle_strip([x1,y1,z1,x2,y2,z2, ... ])` - Creates a triangle strip from a 1-D array of coords
* `color(float r,float g, float b, float a)` - Set color for next vertices
* `console.log(a,b,c,...)` - Log stuff to standard output (objects are converted to JSON)

**GLSL**

GLSL is an awesome language that allows you to do all sorts of magic things with the vertex and pixels of the screen.

* `load_shaders("some-shaders","vertex.glsl","fragment.glsl")` - Load Shaders in current folder. Use "handle" as the name.
* `bind_shaders("some-shaders")` - Bind the shader with name "handle" for the next calls
* `bind_shaders("default")` - Back to the default shader
 
**BONUS**

* `render_model("test.obj");` - Render a 3d model made with blender's obj export. (In export settings: Triangulate faces!)

# Todo

* More scripting languages with swig
* Remove Spidermonkey dependency (Use other JS engines)
* Build on Windows
* Web library with the same API ?

## Contributing

Fork on github
commit your changes
create a pull request

Talk to me for info!

Rules:

* Indentation is 4 spaces
* Lines longer than 65 characters should be really exceptional
  (So I can split my emacs window and the code still looks good on my chromebook)

## Dependencies

In Debian (and maybe Ubuntu):

    sudo apt-get install libmozjs-24-dev freeglut3-dev libglm-dev libglew-dev libsoil-dev zlib1g-dev

## Building

    make

## Use

### Run

    ./app.o

### Run something else than world/

    ./app.o some_folder/

    ./app.o tests/

### Create your own project!

You can start by playing with the code in the `/world` folder.

    cp -r world some_folder

Just have fun with main.js and the GLSL files!

