GCC = g++
FLAGS = -std=c++17 -O3

main.run: main.o
	${GCC} ${FLAGS} main.o -o main

main.o: main.cpp rb_tree.h rb_tree_other.h node.h adding_class.h erasing_class.h
	${GCC} -c ${FLAGS} main.cpp -o main.o