OBJ = main.o
INC = -I "./"


bmp_draw:$(OBJ)
	g++ $(OBJ) -o bmp_draw
	rm -f $(OBJ)

main.o:
	g++ -c main.cpp $(INC)

clean:
	rm -f $(OBJ) bmp_draw

