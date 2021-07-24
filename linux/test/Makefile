ARCH = $(shell uname -s)
PROGRAM = test
ERBPROG = test.ebx

FILES := test
SOURCES := $(FILES:=.cpp)
OBJS := $(FILES:=.o)

CXXFLAGS = -Wall -g -I../src/include
LDFLAGS = -L../lib
LIBS = -lerbcsg -lerbvm 

ifeq ($(ARCH), Darwin)
LIBS += -framework OpenGL -framework GLUT
else
ifeq ($(TERM), cygwin)
LIBS += -lglu32 -lopengl32 -lglut
else
LIBS += -lGL -lGLU -lglut
endif
endif

All: $(PROGRAM) $(ERBPROG)

$(PROGRAM): $(OBJS)
	g++ $(OBJS) -o $(PROGRAM) $(LDFLAGS) $(LIBS) 

world.erb: beauty.mol
	../bin/molokai test.mol > world.erb 

$(ERBPROG): world.erb
	../bin/erb-asm world.erb test.ebx 

clean:
	rm -f $(OBJS) $(ERBPROG) world.erb *~ 
