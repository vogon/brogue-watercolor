#ifdef BROGUE_SDL
#include <stdlib.h>
#include <windows.h>
#include <SDL/SDL.h>

#include "platform.h"

static void gameLoop()
{
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);

	printf("STUB: SDL gameLoop\n");

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Couldn't init SDL.\n");
		return;
	}

	/* go off into game loop */
	rogueMain();

	Sleep(10000);
}

static boolean sdl_pauseForMilliseconds(short ms)
{
	printf("STUB: SDL pauseForMilliseconds(%d)\n", ms);
	return false;
}

static void sdl_nextKeyOrMouseEvent(rogueEvent *returnEvent, boolean textInput, boolean colorsDance)
{
	printf("STUB: SDL nextKeyOrMouseEvent(0x%x, %d, %d)\n", returnEvent, textInput, colorsDance);
}

static void sdl_plotChar(uchar inputChar,
			  short xLoc, short yLoc,
			  short foreRed, short foreGreen, short foreBlue,
			  short backRed, short backGreen, short backBlue)
{
	printf("STUB: SDL plotChar ch=%c, (%d, %d), fg (%d, %d, %d), bg (%d, %d, %d)\n",
		   inputChar, xLoc, yLoc, foreRed, foreGreen, foreBlue, backRed, backGreen, backBlue);
}

static void sdl_remap(const char *input, const char *output)
{
	printf("STUB: SDL remap \"%s\" -> \"%s\"", input, output);
}

static boolean sdl_modifierHeld(int modifier)
{
	printf("STUB: SDL modifierHeld(%d)", modifier);
	return false;
}

// struct brogueConsole {
// 	void (*gameLoop)();
// 	boolean (*pauseForMilliseconds)(short milliseconds);
// 	void (*nextKeyOrMouseEvent)(rogueEvent *returnEvent, boolean textInput, boolean colorsDance);
// 	void (*plotChar)(uchar, short, short, short, short, short, short, short, short);
// 	void (*remap)(const char *, const char *);
// 	boolean (*modifierHeld)(int modifier);
// };

struct brogueConsole sdlConsole = {
	gameLoop,
	sdl_pauseForMilliseconds,
	sdl_nextKeyOrMouseEvent,
	sdl_plotChar,
	sdl_remap,
	sdl_modifierHeld
};

#endif