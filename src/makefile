#CFLAGS = -msse3 -ffast-math -g
CFLAGS = -DNDEBUG -msse3 -ffast-math -g -O2

all: tables.elf

tables.elf: main.o tables.o
	g++ tables.o main.o -g -ltbb -o tables.elf

main.o: main.cpp tables.h
	g++ main.cpp -c $(CFLAGS)

tables.o: tables.cpp tables.h
	g++ tables.cpp -c $(CFLAGS)

clean:
	rm -f *.o *.elf

