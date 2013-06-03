LIBTCODDIR=libtcod
SDLDIR=SDL
SDLTTFDIR=SDL_ttf
BINDIR=bin
CFLAGS=$(FLAGS) -I$(LIBTCODDIR)/include -I$(SDLDIR)/include/SDL -I$(SDLTTFDIR)/include -IBrogueCode -IPlatformCode -DBROGUE_SDL -Wall

OBJS=BrogueCode/Architect.o \
	BrogueCode/Combat.o \
	BrogueCode/Dijkstra.o \
	BrogueCode/Globals.o \
	BrogueCode/Grid.o \
	BrogueCode/IO.o \
	BrogueCode/Buttons.o \
	BrogueCode/MainMenu.o \
	BrogueCode/Items.o \
	BrogueCode/Light.o \
	BrogueCode/Monsters.o \
	BrogueCode/Movement.o \
	BrogueCode/RogueMain.o \
	BrogueCode/Random.o \
	BrogueCode/Recordings.o \
	PlatformCode/main.o \
	PlatformCode/platformdependent.o \
	PlatformCode/curses-platform.o \
	PlatformCode/tcod-platform.o \
	PlatformCode/sdl-platform.o

%.o: %.c
	gcc $(CFLAGS) -ggdb -s -o $@ -c $< 

.PHONY: all clean

all: $(BINDIR)/brogue

clean:
	rm -f $(BINDIR)/brogue.exe $(BINDIR)/brogue-icon.o $(OBJS)

$(BINDIR)/brogue-icon.o: brogue-icon.ico icon.rc
	windres icon.rc $(BINDIR)/brogue-icon.o

$(BINDIR)/brogue: ${OBJS} $(BINDIR)/brogue-icon.o 
	g++ -o $(BINDIR)/brogue.exe ${OBJS} $(BINDIR)/brogue-icon.o -L. -ltcod-mingw-debug -lSDL_ttf -lSDL -L$(BINDIR)/ -static-libgcc -static-libstdc++ -mwindows -ggdb