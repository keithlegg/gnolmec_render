
OBJ = main.o
INC = -I "include"

renderthing:$(OBJ)
	g++ -o renderthing main.cpp Matrices.cpp model.cpp framebuffer.cpp
	rm -f $(OBJ)

clean:
	rm -f $(OBJ) renderthing
	rm -f *.o 
	rm -f *.bmp	

