
This is pretty much the only thing I have written in C++


The vector and Matrix libraries are written by the legendary Song Ho Ahn.
I probably could have written those myself, but it was a huge time saver to borrow his.

You can find them here: 

http://www.songho.ca/opengl/gl_matrix.html


I took the Bresenham algorithm from here:

http://www.roguebasin.com/index.php?title=Bresenham%27s_Line_Algorithm


And the BMP picture export from rasterrain:

http://sourceforge.net/projects/rasterrain/


if you are on Ubuntu or OSX just go to the folder and type "make"

to run type:

./renderthing 500 500 3d_obj/cone.obj 0 60 0 foo.bmp


args in order are 
	X resolution of outoput image
	Y resolution of outoput image
	obj file to load and render
	X rotation 
	Y rotation
	Z rotation 
	output bmp file 




Keith Legg


Updated       - December 6, 2018 
Original code - February 22, 2014




