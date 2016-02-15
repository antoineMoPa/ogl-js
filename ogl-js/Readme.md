# OGL-JS

This project embeds Spidemonkey 24 into a c++ application.

## Currently available functions

**Log**
* log(str) - logs a string [Protip: log(JSON.stringify(obj)) log(JSON.stringify({a: 1, b: 2}))]

**OpenGL**

* translate(float x,float y,float z) - translates next commands
* scale(float x,float y,float z) - scales next commands
* triangle_strip([x1,y1,z1,x2,y2,z2, ... ]) - creates a triangle strip from a 1-D array of coords
* color(float r,float g, float b, float a) - set color for next vertices

**OpenGL todo**

* textures
* UV maps
* vertex shaders
* fragment shaders
* conquer the world

**General todo**

* Better log function
* Connect with browser for dev UI?
* Ability to download "cubes" from the internet in .zip format (but renamed .cube)
* user input
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

Talk to me!

Rules:

* Indentation is 4 spaces
* Lines longer than 65 characters should be really exceptional
  (So I can split my emacs window and the code still looks good on my chromebook)

## Dependencies

In Debian (and maybe Ubuntu):

    sudo apt-get install libmozjs-24-dev freeglut3-dev

## Building

    make

## Run

    ./app.o
