CC = g++
CFLAGS = `pkg-config --cflags opencv` -I headers/
LIBS = `pkg-config --libs opencv`

SRC = src/main.cpp src/gvfc.cpp src/common.cpp src/drlse_edge.cpp
OBJ = active_contour_demo
program: 
	$(CC) -o $(OBJ) $(SRC) $(CFLAGS) $(LIBS)