CC=g++

CFLAGS=$(shell pkg-config --cflags opencv) 
LIBS=$(shell pkg-config --libs opencv) 

OBJS= main.o
DEMOTARGET=main


main.o:	main.cpp
	$(CC) -c $<  -std=c++11	



main:	$(OBJS)
	$(CC) -o $@ $^ -L/usr/lib/x86_64-linux-gnu -ldl -lstdc++  -std=c++11 -lpthread $(LIBS)


clean:
	-rm -r -f   $(DEMOTARGET) *.o 

all:	$(DEMOTARGET)
	make clean  && make main

