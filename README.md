brogue-watercolor
=================

this is an early work-in-progress modification to Brogue (https://sites.google.com/site/broguegame/) by
Brian Walker, to redo the UI to look more like the art of Elizabeth Simins' Brogue Stories
(http://bad-at-games.tumblr.com/post/50116104315/brogue-stories-part-1-maybe-go-play-the-game) rather
than an orthodox roguelike UI.

I'm developing this thing on Windows in MinGW, and at least for the next little while I am thoroughly
unconcerned with it building anywhere else.  (but if you get it working elsewhere, great!  send me a pull 
request.)

licensing and dependencies
--------------------------

this repo is a derivative work of Brogue by Brian Walker, licensed under the GNU AGPL, and is licensed 
under the GNU AGPL.

this repo contains binary and source components of the Simple DirectMedia Layer (http://libsdl.org), 
licensed under the GNU LGPL v2.  (currently, it uses unmodified SDL 1.2.15 for MinGW.)

this repo contains binary and source components of libtcod (http://doryen.eptalys.net/libtcod/),
licensed under the BSD license.  (currently, it uses unmodified libtcod 1.5.1 for MinGW.)

this repo contains binary and source components of SDL_ttf (http://libsdl.org/projects/SDL_ttf), licensed as described in SDL_ttf/COPYING.txt.  (currently, it uses unmodified SDL_ttf 2.0.11.)  SDL_ttf contains binary components of FreeType and zlib, licensed as described in bin/LICENSE.freetype.txt and bin/LICENSE.zlib.txt.