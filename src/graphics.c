/* graphics.c
 * Graphics related functions
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
 

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "font8x8.h"
#include "graphics.h"


unsigned char font8x8[8][8][256];
SDL_Surface *screen;
Uint32 color[256];

SDL_Rect **fs_modes;
SDL_Rect **win_modes;

int Video_fullscreen = 0;
int Video_X = 800;
int Video_Y = 600;


void Init_Font( void )
{
    int a, x, y;
    unsigned char c;

    for( a = 0; a < 256; a++ ) {
	for( y = 0; y < 8; y++ ) {
	    c = raw_font8x8[8*a+y];
	    for( x = 0; x < 8; x++ ) {
		if( (c & 128) == 128 )
		    font8x8[x][y][a] = 1;
		else
		    font8x8[x][y][a] = 0;
		c = c << 1;
	    }
	}
    }
}


void Init_Video( void )
{
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
	printf( "Couldn't initialize SDL: %s\n", SDL_GetError() );
	exit( 1 );
    }
    atexit( SDL_Quit );

    screen = SDL_SetVideoMode( Video_X, Video_Y, 16,
			       SDL_HWSURFACE | Video_fullscreen | SDL_DOUBLEBUF );
    if( screen == NULL ) {
	printf( "Couldn't set video mode %dx%dx16: %s\n", 
		Video_X, Video_Y, SDL_GetError() );
	exit( 1 );
    }

    SDL_WarpMouse( 0, 0 );
    SDL_ShowCursor( SDL_DISABLE );
    SDL_WM_SetCaption( "Tunneler", "Tunneler" );

    /* Black and white */
    color[0]  = SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 );
    color[1]  = SDL_MapRGB( screen->format, 0xff, 0xff, 0xff );
    color[2]  = SDL_MapRGB( screen->format, 0x00, 0x00, 0x88 ); /* Blue background */
    color[3]  = SDL_MapRGB( screen->format, 0x44, 0x44, 0x44 ); /* Dark gray */
    color[4]  = SDL_MapRGB( screen->format, 0x88, 0x88, 0x88 ); /* Medium gray */
    color[5]  = SDL_MapRGB( screen->format, 0xcc, 0xcc, 0xcc ); /* Light gray */
    color[6]  = SDL_MapRGB( screen->format, 0xff, 0xff, 0x00 ); /* Energy */
    color[7]  = SDL_MapRGB( screen->format, 0x00, 0xff, 0xff ); /* Shields */

    /* Field */
    color[8]  = SDL_MapRGB( screen->format, 0x99, 0x66, 0x33 ); /* Light brown */
    color[9]  = SDL_MapRGB( screen->format, 0x66, 0x44, 0x22 ); /* Dark brown */
    color[10] = SDL_MapRGB( screen->format, 0x88, 0x88, 0x88 ); /* Rock */
    color[11] = SDL_MapRGB( screen->format, 0xff, 0xff, 0x00 ); /* Rock2 */

    /* Fire and ammo colors */
    color[12]  = SDL_MapRGB( screen->format, 0xff, 0x00, 0x00 );
    color[13]  = SDL_MapRGB( screen->format, 0x88, 0x00, 0x00 );
    color[14]  = SDL_MapRGB( screen->format, 0xff, 0xff, 0x00 );
    color[15]  = SDL_MapRGB( screen->format, 0x00, 0xff, 0x00 );
    color[16]  = SDL_MapRGB( screen->format, 0x00, 0xff, 0x00 );
    color[17]  = SDL_MapRGB( screen->format, 0x00, 0xff, 0x00 );
    color[18]  = SDL_MapRGB( screen->format, 0x00, 0xff, 0x00 );
    color[19]  = SDL_MapRGB( screen->format, 0x00, 0xff, 0x00 );

    /* Player 0 colors */
    color[30] = SDL_MapRGB( screen->format, 0x00, 0xff, 0x00 );
    color[31] = SDL_MapRGB( screen->format, 0x00, 0x88, 0x00 );
    color[32] = SDL_MapRGB( screen->format, 0xff, 0xff, 0x00 );

    /* Player 1 colors */
    color[40] = SDL_MapRGB( screen->format, 0x00, 0x00, 0xff );
    color[41] = SDL_MapRGB( screen->format, 0x00, 0x00, 0x88 );
    color[42] = SDL_MapRGB( screen->format, 0xff, 0xff, 0x00 );

    /* Noise */
    color[50] = SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 );
    color[51] = SDL_MapRGB( screen->format, 0xaa, 0x22, 0x22 );
    color[52] = SDL_MapRGB( screen->format, 0x00, 0xaa, 0x22 );
    color[53] = SDL_MapRGB( screen->format, 0x22, 0x00, 0xaa );
    color[54] = SDL_MapRGB( screen->format, 0x88, 0x22, 0x22 );
    color[55] = SDL_MapRGB( screen->format, 0x00, 0x88, 0x22 );
    color[56] = SDL_MapRGB( screen->format, 0x22, 0x22, 0x88 );
    color[57] = SDL_MapRGB( screen->format, 0xff, 0xbb, 0xff );
    color[58] = SDL_MapRGB( screen->format, 0x88, 0xbb, 0x88 );
    color[59] = SDL_MapRGB( screen->format, 0x44, 0xbb, 0x44 );
    color[60] = SDL_MapRGB( screen->format, 0x88, 0x88, 0x22 );
    color[61] = SDL_MapRGB( screen->format, 0x22, 0x88, 0x88 );
    color[62] = SDL_MapRGB( screen->format, 0x88, 0x11, 0x88 );
    color[63] = SDL_MapRGB( screen->format, 0xff, 0xff, 0x22 );
    color[64] = SDL_MapRGB( screen->format, 0x22, 0xff, 0xff );
    color[65] = SDL_MapRGB( screen->format, 0xff, 0x22, 0xff );
    color[66] = SDL_MapRGB( screen->format, 0x00, 0x11, 0x00 );
    color[67] = SDL_MapRGB( screen->format, 0xff, 0xcc, 0xff );
    color[68] = SDL_MapRGB( screen->format, 0x22, 0x33, 0x22 );
    color[69] = SDL_MapRGB( screen->format, 0xaa, 0xaa, 0xaa );
	
    SDL_FillRect( screen, NULL, color[0] );
    SDL_Flip( screen );

    win_modes = SDL_ListModes( NULL, SDL_HWSURFACE | SDL_DOUBLEBUF );
    if( win_modes == (SDL_Rect **)-1 ) {
	win_modes       = malloc( 5*sizeof(SDL_Rect *) );
	win_modes[0]    = malloc( sizeof(SDL_Rect) );
	win_modes[0]->w = 1024;
	win_modes[0]->h = 768;
	win_modes[1]    = malloc( sizeof(SDL_Rect) );
	win_modes[1]->w = 800;
	win_modes[1]->h = 600;
	win_modes[2]    = malloc( sizeof(SDL_Rect) );
	win_modes[2]->w = 640;
	win_modes[2]->h = 480;
	win_modes[3]    = malloc( sizeof(SDL_Rect) );
	win_modes[3]->w = 320;
	win_modes[3]->h = 240;
	win_modes[4]    = 0;
    }
		
    fs_modes = SDL_ListModes( NULL, SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF );
    if( fs_modes == (SDL_Rect **)-1 ) {
	fs_modes       = malloc( 5*sizeof(SDL_Rect *) );
	fs_modes[0]    = malloc( sizeof(SDL_Rect) );
	fs_modes[0]->w = 1024;
	fs_modes[0]->h = 768;
	fs_modes[1]    = malloc( sizeof(SDL_Rect) );
	fs_modes[1]->w = 800;
	fs_modes[1]->h = 600;
	fs_modes[2]    = malloc( sizeof(SDL_Rect) );
	fs_modes[2]->w = 640;
	fs_modes[2]->h = 480;
	fs_modes[3]    = malloc( sizeof(SDL_Rect) );
	fs_modes[3]->w = 320;
	fs_modes[3]->h = 240;
	fs_modes[4]    = 0;
    }

    return;
}


void PutPhysPixel( int x, int y, Uint32 color )
{
    int bpp = screen->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    switch( bpp ) {
    case 1:
	*p = color;
	break;
		
    case 2:
	*(Uint16 *)p = color;
	break;
		
    case 3:
	if( SDL_BYTEORDER == SDL_BIG_ENDIAN ) {
	    p[0] = (color >> 16) & 0xff;
	    p[1] = (color >> 8) & 0xff;
	    p[2] = color & 0xff;
	} else {
	    p[0] = color & 0xff;
	    p[1] = (color >> 8) & 0xff;
	    p[2] = (color >> 16) & 0xff;
	}
	break;
		
    case 4:
	*(Uint32 *)p = color;
	break;
    }
}


void PutPixel( int x, int y, Uint32 color )
{
    SDL_Rect rect;

    rect.x = Video_X*x/RES_X;
    rect.y = Video_Y*y/RES_Y;
    rect.w = Video_X*(x+1)/RES_X - Video_X*x/RES_X;
    rect.h = Video_Y*(y+1)/RES_Y - Video_Y*y/RES_Y;

    SDL_FillRect( screen, &rect, color );
}


void PutChar( int x, int y, char ch, Uint32 color )
{
    int i, j;

    for( i = 0; i < 8; i++ ) {
	for( j = 0; j < 8; j++ ) {
	    if( font8x8[j][i][(int)ch] )
		PutPixel( x + j, y + i, color );
	}
    }
}


void PutStr( int x, int y, char *str, Uint32 color )
{
    int i = 0;

    while( *str ) {
	PutChar( x + i, y, *str, color );
	i += 8;
	str++;
    }
}

	
void DrawBox( int x, int y, int w, int h, Uint32 color )
{
    SDL_Rect rect;

    rect.x = Video_X*x/RES_X;
    rect.y = Video_Y*y/RES_Y;
    rect.w = Video_X*(x+w)/RES_X - Video_X*x/RES_X;
    rect.h = Video_Y*(y+h)/RES_Y - Video_Y*y/RES_Y;

    SDL_FillRect( screen, &rect, color );
}


/* End of file graphics.c */
