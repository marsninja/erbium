ARCH = $(shell uname -s)
PROGRAM = molokai

CXXFLAGS = -Wall -g -I../include -L../../lib
LIBS = -lfl -lerbvm -lerbcsg

$(PROGRAM): molokai.tab.o lex.yy.o molokai.o
	g++ molokai.tab.o lex.yy.o molokai.o -o $(PROGRAM) $(CXXFLAGS) $(LIBS) 
	mv $(PROGRAM) ../../bin

molokai.tab.o: molokai.tab.c
	g++ molokai.tab.c -c $(CXXFLAGS)

lex.yy.o: lex.yy.c
	g++ lex.yy.c -c $(CXXFLAGS)

molokai.o: molokai.cpp
	g++ molokai.cpp -c $(CXXFLAGS)

molokai.tab.c: molokai.y
	bison -vd molokai.y

lex.yy.c: molokai.l
	flex molokai.l
  
clean:
	rm -f *.o *.tab.* *.output lex.yy.c *~
