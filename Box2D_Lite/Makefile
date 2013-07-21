
CC = g++
CFLAGS = -Wall -g -I../include/Box2D/
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
LIBDIR  = ../lib/

.PHONY : all clean depend

all: libbox2d.a

libbox2d.a: $(OBJS)
	ar rc $(LIBDIR)libbox2d.a $(OBJS)

.cpp.o:
	$(CC) $(CFLAGS) -c $<

#depend:
#	makedepend -- $(CFLAGS) -- $(SRCS)

clean:
	-rm $(OBJDIR)*.o $(LIBDIR)libbox2d.a

# DO NOT DELETE THIS LINE -- make depend depends on it.


