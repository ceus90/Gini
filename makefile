# this is my giniTest makefile

CC = g++
T1 = giniTree
T2 = giniNtest
.SUFFIXES: .cpp .hpp .o

all:  $(T1) $(T2)

$(T1): giniTree.o
	$(CC) -o giniTree giniTree.o

giniTree.o: giniTree.cpp giniTree.hpp
	$(CC) -c giniTree.cpp

$(T2): giniNtest.o
	$(CC) -o giniNtest giniNtest.o

giniNtest.o: giniNtest.cpp
	$(CC) -c giniNtest.cpp

clean:
	/bin/rm -f *.o core $(T1)
