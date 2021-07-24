LIBRARY = liberbgame.a
PROGRAM = erb-play

#Stuff for libarary
FILES := erb_game 
SOURCES := $(FILES:=.cpp)
OBJS := $(FILES:=.o)

#Stuff for executable
LDFLAGS = -L../../lib
LIBS = -lk-lib -lerbvm -lerbcsg -lerbgame -framework GLUT -framework OPENGL
POBJS = $(PROGRAM:=.o)

CXXFLAGS = -Wall -g -I../include

all: $(LIBRARY) $(PROGRAM)

$(LIBRARY): $(OBJS)
	ar rc $(LIBRARY) $(OBJS)
	ranlib $(LIBRARY)
	mv $(LIBRARY) ../../lib

$(PROGRAM): $(POBJS)
	g++ $(POBJS) -o $(PROGRAM) $(LDFLAGS) $(LIBS) 
	mv $(PROGRAM) ../../bin

clean:
	rm -f $(OBJS) *~
