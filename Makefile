PROGRAM=martilad_PDP_solution

CC=g++

CFLAGS=-std=c++11 -Wall -pedantic -Wextra -g -O3 -fopenmp

compile: $(PROGRAM)

$(PROGRAM): objs/main.o objs/solution.o objs/POL.o
	$(CC) $(CFLAGS) $^ -o $@

objs/solution.o: src/solution.cpp headers/solution.h headers/items.h headers/cord.h headers/constants.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/POL.o: src/POL.cpp headers/POL.h headers/items.h headers/cord.h headers/constants.h headers/helpers.h headers/stackItem.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/main.o: src/main.cpp headers/solution.h headers/POL.h | objs
	$(CC) $(CFLAGS) -c $< -o $@


objs:
	mkdir objs

clean:
	rm -fr $(PROGRAM) objs 2>/dev/null