# OGL-JS

This project embeds Spidemonkey 24 into a c++ application.

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

"handle" is an identifier created by you.

* `load_shaders("handle","vertex.glsl","fragment.glsl")` - Load Shaders in current folder. Use "handle" as the name.
* `bind_shaders("handle")` - Bind the shader with name "handle" for the next calls
* `bind_shaders("default")` - Back to the default shader
 
**BONUS**

* `render_model("test.obj");` - Render a 3d model made with blender's obj export. (In export settings: Triangulate faces!)

**OpenGL todo**

* make render to texture work
* default shader that loads textures, uvs, uses Kd, Ks, Ka from MaterialLib
* conquer the world
* Load PNGs in Model, not just bmp - http://lodev.org/lodepng/? other lib with more filetypes?

**General todo**

* Ability to download "cubes" from the internet in .zip format (but renamed .cube)
* user input (started, but not put in javascript)
* links
* move through the world / follow links

## Cubes

Cubes are subdivisions of the world. They will be coded in Javascript and packed into an archive along with images and 3d models they might need.
The archive size will be limited to < 1 MB. The goal is to keep this file small so it can be loaded in a snap.
Cubes will take advantage of the space they have, maximize originality and use procedural stuff.

The initial motivation is to create a big world where people can create links to other cubes (with urls like http://site.com/unicorn.cube). 

## Why Spidemonkey 24 ?
I did not want to compile chrome or a new Spidemonkey release on my chromebook. Spidermonkey 24 is available in debian packages.

## Make your cubes Free Software
(open source, if you prefer)

I encourage you to follow the hacker mindset and share your cubes under an open source licence such as the GPLV3.

## Contributing

fork on github
commit your changes
create a pull request

Talk to me for info!

Rules:

* Indentation is 4 spaces
* Lines longer than 65 characters should be really exceptional
  (So I can split my emacs window and the code still looks good on my chromebook)

## Dependencies

In Debian (and maybe Ubuntu):

    sudo apt-get install libmozjs-24-dev freeglut3-dev

## Building

    make

## Use

### Run

    ./app.o

### Create your own world!

    cp -r world some_folder

Just have fun with main.js and the GLSL files!

### Run something else than world/

    ./app.o some_folder/
