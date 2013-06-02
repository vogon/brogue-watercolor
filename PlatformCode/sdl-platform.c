#ifdef BROGUE_SDL
#include <stdlib.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "platform.h"

SDL_Surface *renderSurface;

#define CHAR_W 8
#define CHAR_H 16

static void gameLoop()
{
#ifdef WIN32
	// we're running in the Windows subsystem, so open a console the hard way for debugging
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);
#endif /* WIN32 */

	printf("STUB: SDL gameLoop\n");

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) {
		printf("Couldn't init SDL.\n");
		return;
	}

	if (TTF_Init() != 0) {
		printf("Couldn't init SDL_ttf.\n");
		return;
	}

	if ((renderSurface = SDL_SetVideoMode(COLS * CHAR_W, ROWS * CHAR_H, 32, SDL_DOUBLEBUF)) == NULL) {
		printf("Couldn't create video surface.\n");
		return;
	}

	/* go off into game loop */
	rogueMain();

	Sleep(10000);
}

static boolean sdl_pauseForMilliseconds(short ms)
{
	// printf("STUB: SDL pauseForMilliseconds(%d)\n", ms);
	SDL_Flip(renderSurface);
	SDL_Delay(ms);

	return false;
	// return SDL_PeepEvents(NULL, 1, SDL_PEEKEVENT, 
	// 	SDL_KEYDOWN | SDL_KEYUP | SDL_MOUSEMOTION | SDL_MOUSEBUTTONDOWN | SDL_MOUSEBUTTONUP);
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
	// printf("STUB: SDL plotChar ch=%c, (%d, %d), fg (%d, %d, %d), bg (%d, %d, %d)\n",
	// 	   inputChar, xLoc, yLoc, foreRed, foreGreen, foreBlue, backRed, backGreen, backBlue);

	// stub: just render the color rn
	SDL_Rect rect;
	rect.x = CHAR_W * xLoc;
	rect.y = CHAR_H * yLoc;
	rect.w = CHAR_W;
	rect.h = CHAR_H;

	Uint32 col = SDL_MapRGB(renderSurface->format, backRed, backGreen, backBlue);
	SDL_FillRect(renderSurface, &rect, col);
}

static void sdl_remap(const char *input, const char *output)
{
	printf("STUB: SDL remap \"%s\" -> \"%s\"", input, output);
}

static boolean sdl_modifierHeld(int modifier)
{
	SDL_PumpEvents();

	SDLMod modifiers = SDL_GetModState();
	boolean state = 0;

	switch (modifier) {
		case 0: state = modifiers & KMOD_SHIFT;
		case 1: state = modifiers & KMOD_CTRL;
		default: state = 0;
	}

	printf("STUB: SDL modifierHeld(%d) -> %d\n", modifier, state);
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