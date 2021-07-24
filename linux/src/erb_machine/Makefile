LIBRARY = liberbvm.a
PROGRAM = erb-vm

#Stuff for library
FILES := erb_machine erb_proc
SOURCES := $(FILES:=.cpp)
OBJS := $(FILES:=.o)

#Stuff for the executablej
LIBS := -lerbcsg
LDFLAGS := -L../../lib
POBJS := $(PROGRAM:=.o)

CXXFLAGS = -Wall -g -I../include

all: $(LIBRARY) $(PROGRAM)

$(LIBRARY): $(OBJS)
	ar rc $(LIBRARY) $(OBJS)
	ranlib $(LIBRARY)
	mv $(LIBRARY) ../../lib

$(PROGRAM): $(OBJS) $(POBJS)
	g++ $(OBJS) $(POBJS) -o $(PROGRAM) $(LDFLAGS) $(LIBS) 
	mv $(PROGRAM) ../../bin
  
clean:
	rm -f $(OBJS) $(POBJS) *~
