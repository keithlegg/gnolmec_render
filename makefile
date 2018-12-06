OBJ = main.o
INC = -I "include"

bmp_draw:$(OBJ)
	g++ -o bmp_draw main.cpp Matrices.cpp
	rm -f $(OBJ)
	
#main.o:
#	g++ -c Matrices.cpp main.cpp $(INC)

clean:
	rm -f $(OBJ) bmp_draw




## OBJ = main.o
## INC = -I "include"
## 
## bmp_draw:$(OBJ)
## 	g++ $(OBJ) -o bmp_draw
## 	rm -f $(OBJ)
## 
## main.o:
## 	g++ -c Matrices.cpp main.cpp $(INC)
## 
## clean:
## 	rm -f $(OBJ) bmp_draw



