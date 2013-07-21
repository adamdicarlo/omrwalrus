#######################################################################
#
# This makefile ripped from Robert Ohannessian's Speed Hack 2006 entry
# and tweaked for my own use. I added a depend target to create/update
# dependencies (had to use a dirty sed hack, unfortunately, not being
# a makefile expert).
#
# -- Adam DiCarlo
#
# Instructions:
#
# make
#   Compiles all .c and .cpp files in the src directory to .o
#   files in the obj directory, and links them into an
#   executable named 'game' or 'game.exe' in the currect directory.
#
# make clean
#   Removes all .o files from the obj directory.
#
# make veryclean
#   Removes all .o files and the game executable.
#
# Optional parameters:
#
# STATICLINK=1
#   Compiles/removes a statically linked version of the game without
#   DLL dependencies. The static object files are put in obj/static
#   and the executable has '_static' appended to the name.
#
# RELEASE=1    *new*   *new*
#   Turns off debugging.
#
# NAME=game_name
#   Sets the name of the game executable. By default the game
#   executable is called 'game' or 'game.exe'.
#
# If you use add-on libraries, add them to the lines starting with
# 'LIBS='. Make sure you enter the libraries in both lines, for the
# normal and static version!
#
#######################################################################

CC = gcc
CXX = g++
LD = g++

ifdef RELEASE
	CFLAGS = -Iinclude -I`freetype-config --prefix`/include/freetype2 -O3 -s -W -Wall
else
	CFLAGS = -Iinclude -I`freetype-config --prefix`/include/freetype2 -ggdb -W -Wall
endif


# Add-on libraries go here
ifdef STATICLINK
	LIBS = -laldmb_s -ldumb_s
else
	LIBS = -laldmb -ldumb
endif

LIBS += -Llib -lbox2d

# Cygwin should build as mingw
ifneq ($(strip $(shell $(CC) -v 2>&1 | grep "cygwin")),)
  CC  := $(CC)  -mno-cygwin
  CXX := $(CXX) -mno-cygwin
  LD  := $(LD)  -mno-cygwin
endif


ifndef NAME
	NAME = game
endif

ifndef WINDOWS
ifdef MINGDIR
	WINDOWS = 1
endif
endif

ifdef WINDOWS
	RM = del /q
	CFLAGS += -D__GTHREAD_HIDE_WIN32API
	LFLAGS = -Wl,--subsystem,windows
	ifdef STATICLINK
		CFLAGS += -DSTATICLINK
		LIBS += -lalleg_s -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -lwinmm -ldsound
		OBJDIR = obj/static
		BIN = $(NAME)_static.exe
	else
		LIBS += -lalleg
		OBJDIR = obj
		BIN = $(NAME).exe
	endif

	LIBS += `openlayer-config --libs`

else
	RM = rm -f
	ifdef STATICLINK
		LIBS += `allegro-config --libs --static` -lXrender
		OBJDIR = obj/static
		BIN = $(NAME)_static
	else
		LIBS += `allegro-config --libs`
		OBJDIR = obj
		BIN = $(NAME)
	endif

	# AGL needs a few more libs
	LIBS += `openlayer-config --libs`

endif

SRCS := $(wildcard src/*.cpp)
OBJ_CPP := $(addprefix $(OBJDIR)/, $(subst src/,,$(patsubst %.cpp,%.o,$(wildcard src/*.cpp))))
OBJ_C := $(addprefix $(OBJDIR)/, $(subst src/,,$(patsubst %.c,%.o,$(wildcard src/*.c))))

all: game

$(OBJDIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

game: $(OBJ_C) $(OBJ_CPP)
	$(LD) -o $(BIN) $(OBJ_C) $(OBJ_CPP) $(LIBS) $(LFLAGS)

clean:
ifdef WINDOWS
ifneq ($(OBJ_C),)
	-$(RM) $(subst /,\,$(OBJ_C))
endif
ifneq ($(OBJ_CPP),)
	-$(RM) $(subst /,\,$(OBJ_CPP))
endif
else
ifneq ($(OBJ_C),)
	-$(RM) $(OBJ_C)
endif
ifneq ($(OBJ_CPP),)
	-$(RM) $(OBJ_CPP)
endif
endif

veryclean: clean
	-$(RM) $(BIN)


# uses sed to fix up all the dependencies makedepend puts in
depend:
	-makedepend -- $(CFLAGS) -- $(SRCS)
	sed "s/^src\/\(.*\.o\)/$(OBJDIR)\/\1/" Makefile > Makefile.new
	mv Makefile Makefile.makedepend
	mv Makefile.new Makefile

# DO NOT DELETE THIS LINE -- make depend depends on it.

obj/display.o: include/basictypes.hpp include/Box2D/MathUtils.h
obj/display.o: /usr/include/math.h /usr/include/features.h
obj/display.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
obj/display.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
obj/display.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
obj/display.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/display.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/display.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/display.o: /usr/include/time.h /usr/include/endian.h
obj/display.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/display.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/display.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/display.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/display.o: include/counted_ptr.hpp include/display.hpp include/layer.hpp
obj/display.o: include/fixed-array.hpp include/image.hpp include/system.hpp
obj/display.o: include/input.hpp include/event.hpp include/input-device.hpp
obj/display.o: include/peterson-mutex.hpp include/input-event.hpp
obj/entity.o: include/basictypes.hpp include/Box2D/MathUtils.h
obj/entity.o: /usr/include/math.h /usr/include/features.h
obj/entity.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
obj/entity.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
obj/entity.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
obj/entity.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/entity.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/entity.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/entity.o: /usr/include/time.h /usr/include/endian.h
obj/entity.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/entity.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/entity.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/entity.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/entity.o: include/counted_ptr.hpp include/entity.hpp include/Box2D/Body.h
obj/entity.o: include/Box2D/MathUtils.h include/system.hpp
obj/entity.o: include/display.hpp include/layer.hpp include/fixed-array.hpp
obj/entity.o: include/image.hpp include/input.hpp include/event.hpp
obj/entity.o: include/input-device.hpp include/peterson-mutex.hpp
obj/entity.o: include/input-event.hpp
obj/event.o: include/basictypes.hpp include/Box2D/MathUtils.h
obj/event.o: /usr/include/math.h /usr/include/features.h
obj/event.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
obj/event.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
obj/event.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
obj/event.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/event.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/event.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/event.o: /usr/include/time.h /usr/include/endian.h
obj/event.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/event.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/event.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/event.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/event.o: include/counted_ptr.hpp include/system.hpp include/display.hpp
obj/event.o: include/layer.hpp include/fixed-array.hpp include/image.hpp
obj/event.o: include/input.hpp include/event.hpp include/input-device.hpp
obj/event.o: include/peterson-mutex.hpp include/input-event.hpp
obj/game.o: include/basictypes.hpp include/Box2D/MathUtils.h
obj/game.o: /usr/include/math.h /usr/include/features.h
obj/game.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
obj/game.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
obj/game.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
obj/game.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/game.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/game.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/game.o: /usr/include/time.h /usr/include/endian.h
obj/game.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/game.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/game.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/game.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/game.o: include/counted_ptr.hpp include/system.hpp include/display.hpp
obj/game.o: include/layer.hpp include/fixed-array.hpp include/image.hpp
obj/game.o: include/input.hpp include/event.hpp include/input-device.hpp
obj/game.o: include/peterson-mutex.hpp include/input-event.hpp
obj/game.o: include/game.hpp include/scheduler.hpp include/process.hpp
obj/game.o: include/game-world.hpp include/Box2D/World.h
obj/game.o: include/Box2D/MathUtils.h include/Box2D/Arbiter.h
obj/game.o: include/entity.hpp include/Box2D/Body.h include/goal.hpp
obj/game.o: include/wall.hpp include/game-view.hpp
obj/game-view.o: include/basictypes.hpp include/Box2D/MathUtils.h
obj/game-view.o: /usr/include/math.h /usr/include/features.h
obj/game-view.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
obj/game-view.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
obj/game-view.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
obj/game-view.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/game-view.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/game-view.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/game-view.o: /usr/include/time.h /usr/include/endian.h
obj/game-view.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/game-view.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/game-view.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/game-view.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/game-view.o: include/counted_ptr.hpp include/scheduler.hpp
obj/game-view.o: include/process.hpp include/commands.hpp
obj/game-view.o: include/game-view.hpp include/input.hpp include/event.hpp
obj/game-view.o: include/input-device.hpp include/peterson-mutex.hpp
obj/game-view.o: include/input-event.hpp include/game-world.hpp
obj/game-view.o: include/Box2D/World.h include/Box2D/MathUtils.h
obj/game-view.o: include/Box2D/Arbiter.h include/entity.hpp
obj/game-view.o: include/Box2D/Body.h include/goal.hpp include/wall.hpp
obj/game-view.o: include/system.hpp include/display.hpp include/layer.hpp
obj/game-view.o: include/fixed-array.hpp include/image.hpp
obj/game-world.o: include/basictypes.hpp include/Box2D/MathUtils.h
obj/game-world.o: /usr/include/math.h /usr/include/features.h
obj/game-world.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
obj/game-world.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
obj/game-world.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
obj/game-world.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/game-world.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/game-world.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/game-world.o: /usr/include/time.h /usr/include/endian.h
obj/game-world.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/game-world.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/game-world.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/game-world.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/game-world.o: include/counted_ptr.hpp include/system.hpp
obj/game-world.o: include/display.hpp include/layer.hpp
obj/game-world.o: include/fixed-array.hpp include/image.hpp include/input.hpp
obj/game-world.o: include/event.hpp include/input-device.hpp
obj/game-world.o: include/peterson-mutex.hpp include/input-event.hpp
obj/game-world.o: include/entity.hpp include/Box2D/Body.h
obj/game-world.o: include/Box2D/MathUtils.h include/goal.hpp
obj/game-world.o: include/eball.hpp include/game-world.hpp
obj/game-world.o: include/Box2D/World.h include/Box2D/Arbiter.h
obj/game-world.o: include/scheduler.hpp include/process.hpp include/wall.hpp
obj/game-world.o: include/ecat.hpp include/commands.hpp include/ebukkit.hpp
obj/game-world.o: include/emat.hpp include/epusher.hpp include/ewalrus.hpp
obj/game-world.o: include/walrus-generator.hpp
obj/goal.o: include/goal.hpp include/basictypes.hpp include/Box2D/MathUtils.h
obj/goal.o: /usr/include/math.h /usr/include/features.h
obj/goal.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
obj/goal.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
obj/goal.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
obj/goal.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/goal.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/goal.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/goal.o: /usr/include/time.h /usr/include/endian.h
obj/goal.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/goal.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/goal.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/goal.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/goal.o: include/counted_ptr.hpp include/display.hpp include/layer.hpp
obj/goal.o: include/fixed-array.hpp include/image.hpp include/system.hpp
obj/goal.o: include/input.hpp include/event.hpp include/input-device.hpp
obj/goal.o: include/peterson-mutex.hpp include/input-event.hpp
obj/image.o: include/basictypes.hpp include/Box2D/MathUtils.h
obj/image.o: /usr/include/math.h /usr/include/features.h
obj/image.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
obj/image.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
obj/image.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
obj/image.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/image.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/image.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/image.o: /usr/include/time.h /usr/include/endian.h
obj/image.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/image.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/image.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/image.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/image.o: include/counted_ptr.hpp include/display.hpp include/layer.hpp
obj/image.o: include/fixed-array.hpp include/image.hpp include/system.hpp
obj/image.o: include/input.hpp include/event.hpp include/input-device.hpp
obj/image.o: include/peterson-mutex.hpp include/input-event.hpp
obj/input.o: include/basictypes.hpp include/Box2D/MathUtils.h
obj/input.o: /usr/include/math.h /usr/include/features.h
obj/input.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
obj/input.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
obj/input.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
obj/input.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/input.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/input.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/input.o: /usr/include/time.h /usr/include/endian.h
obj/input.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/input.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/input.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/input.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/input.o: include/counted_ptr.hpp include/input.hpp include/event.hpp
obj/input.o: include/input-device.hpp include/peterson-mutex.hpp
obj/input.o: include/input-event.hpp include/system.hpp include/display.hpp
obj/input.o: include/layer.hpp include/fixed-array.hpp include/image.hpp
obj/input-device.o: include/basictypes.hpp include/Box2D/MathUtils.h
obj/input-device.o: /usr/include/math.h /usr/include/features.h
obj/input-device.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
obj/input-device.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
obj/input-device.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
obj/input-device.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/input-device.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/input-device.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/input-device.o: /usr/include/time.h /usr/include/endian.h
obj/input-device.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/input-device.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/input-device.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/input-device.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/input-device.o: include/counted_ptr.hpp include/input.hpp
obj/input-device.o: include/event.hpp include/input-device.hpp
obj/input-device.o: include/peterson-mutex.hpp include/input-event.hpp
obj/input-device.o: include/system.hpp include/display.hpp include/layer.hpp
obj/input-device.o: include/fixed-array.hpp include/image.hpp
obj/layer.o: include/basictypes.hpp include/Box2D/MathUtils.h
obj/layer.o: /usr/include/math.h /usr/include/features.h
obj/layer.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
obj/layer.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
obj/layer.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
obj/layer.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/layer.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/layer.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/layer.o: /usr/include/time.h /usr/include/endian.h
obj/layer.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/layer.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/layer.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/layer.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/layer.o: include/counted_ptr.hpp include/display.hpp include/layer.hpp
obj/layer.o: include/fixed-array.hpp include/image.hpp include/system.hpp
obj/layer.o: include/input.hpp include/event.hpp include/input-device.hpp
obj/layer.o: include/peterson-mutex.hpp include/input-event.hpp
obj/main.o: include/basictypes.hpp include/Box2D/MathUtils.h
obj/main.o: /usr/include/math.h /usr/include/features.h
obj/main.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
obj/main.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
obj/main.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
obj/main.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/main.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/main.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/main.o: /usr/include/time.h /usr/include/endian.h
obj/main.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/main.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/main.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/main.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/main.o: include/counted_ptr.hpp include/system.hpp include/display.hpp
obj/main.o: include/layer.hpp include/fixed-array.hpp include/image.hpp
obj/main.o: include/input.hpp include/event.hpp include/input-device.hpp
obj/main.o: include/peterson-mutex.hpp include/input-event.hpp
obj/peterson-mutex.o: include/peterson-mutex.hpp include/basictypes.hpp
obj/peterson-mutex.o: include/Box2D/MathUtils.h /usr/include/math.h
obj/peterson-mutex.o: /usr/include/features.h /usr/include/sys/cdefs.h
obj/peterson-mutex.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
obj/peterson-mutex.o: /usr/include/gnu/stubs-64.h
obj/peterson-mutex.o: /usr/include/bits/huge_val.h
obj/peterson-mutex.o: /usr/include/bits/mathdef.h
obj/peterson-mutex.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/peterson-mutex.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/peterson-mutex.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/peterson-mutex.o: /usr/include/time.h /usr/include/endian.h
obj/peterson-mutex.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/peterson-mutex.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/peterson-mutex.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/peterson-mutex.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/peterson-mutex.o: include/counted_ptr.hpp
obj/scheduler.o: include/basictypes.hpp include/Box2D/MathUtils.h
obj/scheduler.o: /usr/include/math.h /usr/include/features.h
obj/scheduler.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
obj/scheduler.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
obj/scheduler.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
obj/scheduler.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/scheduler.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/scheduler.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/scheduler.o: /usr/include/time.h /usr/include/endian.h
obj/scheduler.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/scheduler.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/scheduler.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/scheduler.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/scheduler.o: include/counted_ptr.hpp include/scheduler.hpp
obj/scheduler.o: include/process.hpp include/system.hpp include/display.hpp
obj/scheduler.o: include/layer.hpp include/fixed-array.hpp include/image.hpp
obj/scheduler.o: include/input.hpp include/event.hpp include/input-device.hpp
obj/scheduler.o: include/peterson-mutex.hpp include/input-event.hpp
obj/system.o: include/basictypes.hpp include/Box2D/MathUtils.h
obj/system.o: /usr/include/math.h /usr/include/features.h
obj/system.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
obj/system.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
obj/system.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
obj/system.o: /usr/include/bits/mathcalls.h /usr/include/assert.h
obj/system.o: /usr/include/stdlib.h /usr/include/sys/types.h
obj/system.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
obj/system.o: /usr/include/time.h /usr/include/endian.h
obj/system.o: /usr/include/bits/endian.h /usr/include/sys/select.h
obj/system.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
obj/system.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
obj/system.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
obj/system.o: include/counted_ptr.hpp include/display.hpp include/layer.hpp
obj/system.o: include/fixed-array.hpp include/image.hpp include/game.hpp
obj/system.o: include/scheduler.hpp include/process.hpp
obj/system.o: include/game-world.hpp include/Box2D/World.h
obj/system.o: include/Box2D/MathUtils.h include/Box2D/Arbiter.h
obj/system.o: include/entity.hpp include/Box2D/Body.h include/goal.hpp
obj/system.o: include/wall.hpp include/system.hpp include/input.hpp
obj/system.o: include/event.hpp include/input-device.hpp
obj/system.o: include/peterson-mutex.hpp include/input-event.hpp
obj/wall.o: include/game-world.hpp include/Box2D/World.h
obj/wall.o: include/Box2D/MathUtils.h include/Box2D/Arbiter.h
obj/wall.o: include/entity.hpp include/Box2D/Body.h include/basictypes.hpp
obj/wall.o: include/Box2D/MathUtils.h /usr/include/math.h
obj/wall.o: /usr/include/features.h /usr/include/sys/cdefs.h
obj/wall.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
obj/wall.o: /usr/include/gnu/stubs-64.h /usr/include/bits/huge_val.h
obj/wall.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
obj/wall.o: /usr/include/assert.h /usr/include/stdlib.h
obj/wall.o: /usr/include/sys/types.h /usr/include/bits/types.h
obj/wall.o: /usr/include/bits/typesizes.h /usr/include/time.h
obj/wall.o: /usr/include/endian.h /usr/include/bits/endian.h
obj/wall.o: /usr/include/sys/select.h /usr/include/bits/select.h
obj/wall.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
obj/wall.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
obj/wall.o: /usr/include/alloca.h include/counted_ptr.hpp include/goal.hpp
obj/wall.o: include/scheduler.hpp include/process.hpp include/wall.hpp
obj/wall.o: include/system.hpp include/display.hpp include/layer.hpp
obj/wall.o: include/fixed-array.hpp include/image.hpp include/input.hpp
obj/wall.o: include/event.hpp include/input-device.hpp
obj/wall.o: include/peterson-mutex.hpp include/input-event.hpp
