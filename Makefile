PROGRAM=martilad

CC=g++

CFLAGS=-std=c++11 -Wall -pedantic -Wextra -g -O3 -fopenmp

build: $(PROGRAM)

$(PROGRAM): objs/solution.o objs/helpers.o objs/solver.o objs/recursion.o objs/taskParallel.o \
objs/dataParallel.o objs/main.o
	$(CC) $(CFLAGS) $^ -o $@

objs/helpers.o: src/helpers.cpp headers/helpers.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/solution.o: src/solution.cpp headers/solution.h headers/items.h headers/cord.h headers/constants.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/solver.o: src/solvers/solver.cpp headers/solvers/solver.h headers/solution.h headers/helpers.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/recursion.o: src/solvers/recursion.cpp headers/solvers/recursion.h headers/solvers/solver.h \
headers/solution.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/taskParallel.o: src/solvers/taskParallel.cpp headers/solvers/taskParallel.h headers/solvers/solver.h \
headers/solution.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/dataParallel.o: src/solvers/dataParallel.cpp headers/solvers/dataParallel.h headers/solvers/solver.h \
headers/solution.h headers/item.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/main.o: src/main.cpp headers/solution.h headers/solvers/solver.h headers/solvers/recursion.h \
headers/solvers/taskParallel.h headers/solvers/dataParallel.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs:
	mkdir objs

clean:
	rm -fr $(PROGRAM) objs 2>/dev/null