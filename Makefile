CFLAGS = -Wall -Wextra -W -Wshadow -pedantic -g3

OBJ1 = main1.o list.o entry.o kdTree.o
OBJ2 = main2.o list.o entry.o kdTree.o

all: map1 map2

# Nearest Neighbour Search
map1: $(OBJ1)
	gcc $(CFLAGS) -o map1 $(OBJ1) -lm

# Radius Search
map2: $(OBJ2)
	gcc $(CFLAGS) -o map2 $(OBJ2) -lm

main1.o: main1.c kdTree.h list.h entry.h
	gcc $(CFLAGS) -c -o main1.o main1.c

main2.o: main2.c kdTree.h list.h entry.h
	gcc $(CFLAGS) -c -o main2.o main2.c

list.o: list.c list.h entry.h
	gcc $(CFLAGS) -c -o list.o list.c

entry.o: entry.c entry.h
	gcc $(CFLAGS) -c -o entry.o entry.c

kdTree.o: kdTree.c kdTree.h entry.h list.h
	gcc $(CFLAGS) -c -o kdTree.o kdTree.c


clean:
	rm *.o map1 map2 *output*
