LIBRARY = libk-lib.a

FILES := k-utils scanner seq_engine
SOURCES := $(FILES:=.cpp)
OBJS := $(FILES:=.o)

CXXFLAGS = -O3 #-g

$(LIBRARY): $(OBJS)
	ar rc $(LIBRARY) $(OBJS)
	mv $(LIBRARY) ../lib
	ranlib ../lib/$(LIBRARY)
	cp ../lib/$(LIBRARY) ../../../lib/$(LIBRARY)

clean:
	rm -f $(OBJS) *~
