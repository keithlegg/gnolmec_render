
OBJ = main.o
INC = -I "include"

renderthing:$(OBJ)
	# -std=c++11 added for OSX to handle vectors properly  
	g++ -std=c++11 -o renderthing main.cpp ngc_model.cpp Matrices.cpp model.cpp framebuffer.cpp render.cpp point_ops.cpp math_ops.cpp

	# -g is for debugging flags with GDB 
	#g++ -g -o renderthing main.cpp Matrices.cpp model.cpp framebuffer.cpp render.cpp
	rm -f $(OBJ)

clean:
	rm -f $(OBJ) renderthing
	rm -f *.o 
	rm -f *.bmp	

