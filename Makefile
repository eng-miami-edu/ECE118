all: 
	g++ -c library.cpp
	gcc -c clibrary.c
	gcc -c gpc.c
	g++ main.cpp library.o clibrary.o gpc.o -lraylib -o main
