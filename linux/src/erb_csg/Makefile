LIBRARY = liberbcsg.a

FILES := erb_engine erb_math
SOURCES := $(FILES:=.cpp)
OBJS := $(FILES:=.o)

CXXFLAGS = -Wall -g -I../include

$(LIBRARY): $(OBJS)
	ar rc $(LIBRARY) $(OBJS)
	ranlib $(LIBRARY)
	mv $(LIBRARY) ../../lib

clean:
	rm -f $(OBJS) *~
