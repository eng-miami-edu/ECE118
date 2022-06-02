Windows: main.o library.o clibrary.o gpc.o
	g++ main.o library.o clibrary.o gpc.o -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm -lpthread -o main.exe

Mac: main.o library.o clibrary.o gpc.o
	g++ main.o library.o clibrary.o gpc.o -I include -L lib -lpthread -o main

main.o: main.cpp
	g++ -c main.cpp -I include

library.o: library.cpp
	g++ -c library.cpp -I include

clibrary.o: clibrary.c
	gcc -c clibrary.c -I include

gpc.o: gpc.c
	gcc -c gpc.c -I include

