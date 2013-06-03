#ifdef BROGUE_SDL
#include <stdlib.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "platform.h"

extern playerCharacter rogue;

SDL_Surface *renderSurface;
TTF_Font *face;

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

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Couldn't init SDL.\n");
		return;
	}

	if (TTF_Init() != 0) {
		printf("Couldn't init SDL_ttf.\n");
		return;
	}

	if ((face = TTF_OpenFont("bin/SourceCodePro-Regular.ttf", 12)) == NULL) {
		printf("Couldn't load font face.\n");
		return;
	}

	if ((renderSurface = SDL_SetVideoMode(COLS * CHAR_W, ROWS * CHAR_H, 32, SDL_DOUBLEBUF)) == NULL) {
		printf("Couldn't create video surface.\n");
		return;
	}

	/* go off into game loop */
	rogueMain();
}

static boolean sdl_pauseForMilliseconds(short ms)
{
	// printf("STUB: SDL pauseForMilliseconds(%d)\n", ms);
	SDL_Flip(renderSurface);
	SDL_Delay(ms);

	SDL_Event unused;

	return SDL_PeepEvents(&unused, 1, SDL_PEEKEVENT, 
	 	SDL_KEYDOWN | SDL_KEYUP | SDL_MOUSEMOTION | SDL_MOUSEBUTTONDOWN | SDL_MOUSEBUTTONUP);
}

#define PAUSE_BETWEEN_EVENT_POLLING 36

static void xy2cxcy(int x, int y, int *cx, int *cy) {
	printf("at %d, %d\n", x, y);
	if (cx != NULL) *cx = x / CHAR_W;
	if (cy != NULL) *cy = y / CHAR_H;
}

int last_mouse_cx = -1, last_mouse_cy = -1;

static void sdl_nextKeyOrMouseEvent(rogueEvent *returnEvent, boolean textInput, boolean colorsDance)
{
	printf("STUB: SDL nextKeyOrMouseEvent(0x%x, %d, %d)\n", returnEvent, textInput, colorsDance);

	if (noMenu && rogue.nextGame == NG_NOTHING) rogue.nextGame = NG_NEW_GAME;

	while (true) {
		printf("nextKeyOrMouseEvent loop\n");

		Uint32 theTime = SDL_GetTicks();
		int cx, cy;
				
		// if (!(serverMode || (SDL_GetAppState() & SDL_APPACTIVE))) {
		// 	SDL_Delay(100);
		// } else {
		// 	if (colorsDance) {
		// 		shuffleTerrainColors(3, true);
		// 		commitDraws();
		// 	}
		// }

		SDL_Event evt;
		if (1 == SDL_PollEvent(&evt)) {
			switch (evt.type) {
			case SDL_QUIT:
				rogue.gameHasEnded = true;
				rogue.nextGame = NG_QUIT;
				returnEvent->eventType = KEYSTROKE;
				returnEvent->param1 = ESCAPE_KEY;
				return;

			case SDL_MOUSEBUTTONUP:
				if (evt.button.button == SDL_BUTTON_LEFT) {
					printf("lmb up\n");
					returnEvent->eventType = MOUSE_UP;
				} else if (evt.button.button == SDL_BUTTON_RIGHT) {
					printf("rmb up\n");
					returnEvent->eventType = RIGHT_MOUSE_UP;
				} else {
					// ignore middle mouse button events
					continue;
				}

				xy2cxcy(evt.button.x, evt.button.y, 
					&(returnEvent->param1), &(returnEvent->param2));
				return;

			case SDL_MOUSEBUTTONDOWN:
				if (evt.button.button == SDL_BUTTON_LEFT) {
					printf("lmb down\n");
					returnEvent->eventType = MOUSE_DOWN;
				} else if (evt.button.button == SDL_BUTTON_RIGHT) {
					printf("rmb down\n");
					returnEvent->eventType = RIGHT_MOUSE_DOWN;
				} else {
					// ignore middle mouse button events
					continue;
				}

				xy2cxcy(evt.button.x, evt.button.y, 
					&(returnEvent->param1), &(returnEvent->param2));
				return;

			case SDL_MOUSEMOTION:
				xy2cxcy(evt.motion.x, evt.motion.y, &cx, &cy);

				if (cx != last_mouse_cx || cy != last_mouse_cy) {
					// mouse has changed cells
					returnEvent->eventType = MOUSE_ENTERED_CELL;
					returnEvent->param1 = cx;
					returnEvent->param2 = cy;
					last_mouse_cx = cx;
					last_mouse_cy = cy;
					return;
				} else {
					// mouse hasn't changed cells
					continue;
				}

			default:
				printf("dropped event %d on the floor\n", evt.type);
				continue;
			}
		}

		// Sint32 waitTime = PAUSE_BETWEEN_EVENT_POLLING + theTime - SDL_GetTicks();

		// if (waitTime > 0 && waitTime <= PAUSE_BETWEEN_EVENT_POLLING) {
		// 	SDL_Delay(waitTime);
		// }
	}
}

static void sdl_plotChar(uchar inputChar,
			  short xLoc, short yLoc,
			  short foreRed, short foreGreen, short foreBlue,
			  short backRed, short backGreen, short backBlue)
{
	// printf("STUB: SDL plotChar ch=%c, (%d, %d), fg (%d, %d, %d), bg (%d, %d, %d)\n",
	// 	   inputChar, xLoc, yLoc, foreRed, foreGreen, foreBlue, backRed, backGreen, backBlue);

	// render bg
	SDL_Rect bgRect;
	bgRect.x = CHAR_W * xLoc;
	bgRect.y = CHAR_H * yLoc;
	bgRect.w = CHAR_W;
	bgRect.h = CHAR_H;

	Uint32 col = SDL_MapRGB(renderSurface->format, backRed, backGreen, backBlue);
	SDL_FillRect(renderSurface, &bgRect, col);

	// render character
	SDL_Color color;
	color.r = foreRed;
	color.g = foreGreen;
	color.b = foreBlue;

	SDL_Surface *tempSurface = TTF_RenderGlyph_Blended(face, inputChar, color);

	SDL_Rect fgRect;

	int ascent = TTF_FontAscent(face);
	int maxY = 0;
	TTF_GlyphMetrics(face, inputChar, NULL, NULL, NULL, &maxY, NULL);

	fgRect.x = CHAR_W * xLoc;
	fgRect.y = CHAR_H * yLoc + (ascent - maxY);
	fgRect.w = CHAR_W;
	fgRect.h = CHAR_H;

	SDL_BlitSurface(tempSurface, NULL, renderSurface, &fgRect);

	SDL_FreeSurface(tempSurface);
}

static void sdl_remap(const char *input, const char *output)
{
	printf("STUB: SDL remap \"%s\" -> \"%s\"\n", input, output);
}

static boolean sdl_modifierHeld(int modifier)
{
	// SDL_PumpEvents();

	SDLMod modState = SDL_GetModState();
	boolean state = 0;

	switch (modifier) {
	case 0: 
		state = modState & KMOD_SHIFT; 
		break;
	case 1: 
		state = modState & KMOD_CTRL; 
		break;
	default: 
		state = 0;
		break;
	}

	// printf("STUB: SDL modifierHeld(%d) -> %d (%d)\n", modifier, state, modState);

	return state;
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