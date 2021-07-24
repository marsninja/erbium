ARCH = $(shell uname -s)
PROGRAM = erb-asm

FILES := erb_assembler erb-asm
SOURCES := $(FILES:=.cpp)
OBJS := $(FILES:=.o)

CXXFLAGS = -Wall -g -I../include
LDFLAGS = -L../../lib
LIBS = -lk-lib -lerbvm -lerbcsg

$(PROGRAM): $(OBJS)
	g++ $(OBJS) -o $(PROGRAM) $(LDFLAGS) $(LIBS) 
	mv $(PROGRAM) ../../bin

clean:
	rm -f $(OBJS) *~
