
OBJ = main.o
INC = -I "include"

bmp_draw:$(OBJ)
	g++ -o bmp_draw main.cpp Matrices.cpp model.cpp
	rm -f $(OBJ)

clean:
	rm -f $(OBJ) bmp_draw

