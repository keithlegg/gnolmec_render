

![alt text](https://raw.githubusercontent.com/keithlegg/render_thing/master/images/examples/monkey.jpg) 

simple 3d renderer, procedural model in C++, with GCC using standard libraries. 

The vector and Matrix libraries are written by the legendary Song Ho Ahn.
I probably could have written those myself, but it was a huge time saver to borrow his.

You can find them here: 

http://www.songho.ca/opengl/gl_matrix.html


I took the Bresenham algorithm from here:

http://www.roguebasin.com/index.php?title=Bresenham%27s_Line_Algorithm


And the BMP picture export from these guys:

http://sourceforge.net/projects/rasterrain/
https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/


if you are on Ubuntu or OSX just go to the folder and type "make"

The newer renderer requires a 4X4 camera matrix that it reads from disk.

It is all controlled by example.olm, and cam_matrix.olm, both included. 

To run:
<pre>
./renderthing 512 512 example.olm myrender.bmp
</pre>



The old renderer lives in here as "really_simple_render_model()"
To activate look in main.cpp

The arguments to it are as follows:

<pre>
    ./renderthing 500 500 3d_obj/cone.obj 0 60 0 foo.bmp 50 

    args in order are:
        - X resolution of output image
        - Y resolution of output image
        - obj file to load and render
        - X rotation 
        - Y rotation
        - Z rotation 
        - output bmp file 
        - render overall scale
</pre>







Keith Legg
Updated       - December 6, 2018 
Original code - February 22, 2014




