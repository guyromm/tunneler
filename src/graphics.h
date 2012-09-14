/* graphics.h
 * Graphics related function headers
 * Last modified 30 May 2007
 *
 * Copyright (c) 2004,2007 Taneli Kalvas
 *
 * This file is part of SDL Tunneler
 *
 * SDL Tunneler is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 * 
 * SDL Tunneler is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SDL Tunneler ; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 *
 * Questions, comments and bug reports should be sent to the author 
 * directly via email at tvkalvas@cc.jyu.fi
 */


#ifndef TUNNELER_GRAPHICS_H
#define TUNNELER_GRAPHICS_H


#include <SDL/SDL.h>


#define RES_X 160
#define RES_Y 120

extern SDL_Surface *screen;
extern Uint32 color[256];

extern int Video_fullscreen;
extern int Video_X;
extern int Video_Y;

extern SDL_Rect **fs_modes;
extern SDL_Rect **win_modes;


void Init_Font( void );
void Init_Video( void );

void PutPhysPixel( int x, int y, Uint32 color );
void PutPixel( int x, int y, Uint32 color );
void PutChar( int x, int y, char ch, Uint32 color );
void PutStr( int x, int y, char *str, Uint32 color );
void DrawBox( int x, int y, int w, int h, Uint32 color );


#endif /* End of file graphics.h */
