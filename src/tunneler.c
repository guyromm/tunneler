/* tunneler.c
 * Tunneler main
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
#include <string.h>
#include <math.h>
#include <SDL/SDL.h>
#include "ai.h"
#include "timer.h"
#include "types.h"
#include "keys.h"
#include "game.h"
#include "graphics.h"
#include "terrain.h"


#define CTEST_MACRO(Y,X) 
#define TUNNEL_MACRO(X) if( X == 8 || X == 9 ) X = 0


int noise0 = 0;
int noise1 = 0;
int max;

double rot_xtable[8] = { 1.000,  0.707,  0.000, -0.707, -1.000, -0.707,  0.000,  0.707};
double rot_ytable[8] = { 0.000,  0.707,  1.000,  0.707,  0.000, -0.707, -1.000, -0.707};


struct AMMO Ammo[2][128];
struct EXPL Expl[128];
struct TANK Tank[2];
int tank_spr[2][7][7] = { TANK_SPRITE };


int Round( double a )
{
    if( a - floor(a) < 0.5 )
	return( (int)floor(a) );
    else
	return( (int)ceil(a) );
}


void DrawTank( int x, int y, int rot, int player )
{
    int i, j;
    player = 30 + 10*player - 1;

    if( rot == 0 || rot == 1 ) {
	for( j = -3; j <= 3; j++ ) {
	    for( i = -3; i <= 3; i++ ) {
		if( tank_spr[rot][j+3][i+3] != 0 )
		    PutPixel( x+i, y+j, color[player+tank_spr[rot][j+3][i+3]] );
	    }
	}
    } else if( rot == 2 ) {
	rot = 0;

	for( j = -3; j <= 3; j++ ) {
	    for( i = -3; i <= 3; i++ ) {
		if( tank_spr[rot][j+3][i+3] != 0 )
		    PutPixel( x+j, y+i, color[player+tank_spr[rot][j+3][i+3]] );
	    }
	}
    } else if( rot == 3 || rot == 4 ) {
	if( rot == 4 )
	    rot = 0;
	else
	    rot = 1;

	for( j = -3; j <= 3; j++ ) {
	    for( i = -3; i <= 3; i++ ) {
		if( tank_spr[rot][j+3][i+3] != 0 )
		    PutPixel( x-i, y+j, color[player+tank_spr[rot][j+3][i+3]] );
	    }
	}
    } else if( rot == 5 ) {
	rot = 1;

	for( j = -3; j <= 3; j++ ) {
	    for( i = -3; i <= 3; i++ ) {
		if( tank_spr[rot][j+3][i+3] != 0 )
		    PutPixel( x-i, y-j, color[player+tank_spr[rot][j+3][i+3]] );
	    }
	}
    } else if( rot == 6 ) {
	rot = 0;

	for( j = -3; j <= 3; j++ ) {
	    for( i = -3; i <= 3; i++ ) {
		if( tank_spr[rot][j+3][i+3] != 0 )
		    PutPixel( x+j, y-i, color[player+tank_spr[rot][j+3][i+3]] );
	    }
	}
    } else if( rot == 7 ) {
	rot = 1;

	for( j = -3; j <= 3; j++ ) {
	    for( i = -3; i <= 3; i++ ) {
		if( tank_spr[rot][j+3][i+3] != 0 )
		    PutPixel( x+i, y-j, color[player+tank_spr[rot][j+3][i+3]] );
	    }
	}
    }
}


void DrawShadow( int x, int y, int w, int h )
{
    DrawBox(     x,     y,   w,  1, color[5] );
    DrawBox(     x,     y,   2,  h, color[5] );

    DrawBox(   x+2, y+h-1, w-2,  1, color[3] );
    DrawBox( x+w-2,     y,   2,  h, color[3] );
}


void DrawLetter( int x, int y, char ch )
{
    Uint32 col;
	
    if( ch == 'E' )
	col = color[6];
    else
	col = color[7];

    DrawBox( x,   y, 5, 1, col );
    DrawBox( x, y+2, 5, 1, col );
    DrawBox( x, y+4, 5, 1, col );

    if( ch == 'E' )
	DrawBox( x, y, 1, 5, col );
    else {
	DrawBox( x,     y, 1, 3, col );
	DrawBox( x+4, y+2, 1, 3, col );
    }
}


void DrawStatusBox( int x, int y )
{
    DrawBox(       x,  y, 68, 24, color[4] );
    DrawShadow(    x,  y, 68, 24 );

    DrawLetter(  x+4,  y+4, 'E' );
    DrawShadow( x+11,  y+3, 53, 7 );
    DrawLetter(  x+4, y+15, 'S' );
    DrawShadow( x+11, y+14, 53, 7 );
}


void DrawFrames( void )
{
    SDL_FillRect( screen, NULL, color[2] );

    DrawBox(     2,   2, 76, 90, color[0] );
    DrawBox(    82,   2, 76, 90, color[0] );

    DrawStatusBox( 6, 94 );
    DrawStatusBox( 86, 94 );
}


void DrawNoise( int x, int y, int w, int h )
{
    int i, j, n;

    n = 0;
    DrawBox( x, y, w, h, color[0] );

    for( j = 0; j < h; j++ ) {
	for( i = 0; i < w; i++ ) {
	    if( n == 0 ) {
		j += 2+(int)(5.0*rand()/(RAND_MAX+1.0));
		n  = 90+(int)(1400.0*rand()/(RAND_MAX+1.0));
		break;
	    } else
		n--;
			
	    PutPixel( x+i, y+j, color[50 + (int)(20.0*rand()/(RAND_MAX+1.0))] );
	}
    }
}


int NoiseProb( double E )
{
    if( 1.0/(80.0*E) > 0.50 )
	return( 1.0*rand()/(RAND_MAX+1.0) > 0.50 );
    else
	return( 1.0*rand()/(RAND_MAX+1.0) > 1.0/(80.0*E) );
}


void Draw( void )
{
    int x, y;
    int i, j;
    SDL_Rect rect;

    /* Draw status */
    DrawBox( 19,  98, 49, 5, color[0] );
    DrawBox( 19, 109, 49, 5, color[0] );
    DrawBox( 99,  98, 49, 5, color[0] );
    DrawBox( 99, 109, 49, 5, color[0] );

    DrawBox( 19,  98, 49.0*Tank[1].Energy,  5, color[6] );
    if( Tank[1].Shields > 0.0 )
	DrawBox( 19, 109, 49.0*Tank[1].Shields, 5, color[7] );

    DrawBox( 99,  98, 49.0*Tank[0].Energy,  5, color[6] );
    if( Tank[0].Shields > 0.0 )
	DrawBox( 99, 109, 49.0*Tank[0].Shields, 5, color[7] );

    /* Draw field or noise */
    if( Tank[0].Energy >= 0.25 || NoiseProb( Tank[0].Energy ) ) {
	x = Round(Tank[0].x);
	y = Round(Tank[0].y);
	for( j = 0; j < 90; j++ ) {
	    for( i = 0; i < 76; i++ )
		PutPixel( 82+i, 2+j, color[field[y+j-45][x+i-38]] );
	}
    }
    else
	noise0 = 2;

    if( Tank[1].Energy >= 0.25 || NoiseProb( Tank[1].Energy ) ) {
	x = Round(Tank[1].x);
	y = Round(Tank[1].y);
	for( j = 0; j < 90; j++ ) {
	    for( i = 0; i < 76; i++ )
		PutPixel( 2+i, 2+j, color[field[y+j-45][x+i-38]] );
	}
    }
    else
	noise1 = 2;

    /* Draw Tanks */
    if( Tank[0].deathc <= 0.0 ) {
	DrawTank( 120, 47, Tank[0].rot, 0 );

	rect.x = Video_X*2/RES_X;
	rect.y = Video_Y*2/RES_Y;
	rect.w = Video_X*(2+76)/RES_X - Video_X*2/RES_X;
	rect.h = Video_Y*(2+90)/RES_Y - Video_Y*2/RES_Y;

	SDL_SetClipRect( screen, &rect );
	DrawTank( Round(Tank[0].x) - Round(Tank[1].x) + 40,
		  Round(Tank[0].y) - Round(Tank[1].y) + 47,
		  Tank[0].rot, 0 );
	SDL_SetClipRect( screen, NULL );
    } 
    if( Tank[1].deathc <= 0.0 ) {
	DrawTank( 40, 47, Tank[1].rot, 1 );

	rect.x = Video_X*82/RES_X;
	rect.y = Video_Y*2/RES_Y;
	rect.w = Video_X*(82+76)/RES_X - Video_X*82/RES_X;
	rect.h = Video_Y*(2+90)/RES_Y - Video_Y*2/RES_Y;

	SDL_SetClipRect( screen, &rect );
	DrawTank( Round(Tank[1].x) - Round(Tank[0].x) + 120,
		  Round(Tank[1].y) - Round(Tank[0].y) + 47,
		  Tank[1].rot, 1 );
	SDL_SetClipRect( screen, NULL );
    }

    /* Draw Ammo */
    for( j = 0; j < 2; j++ ) {
	for( i = 0; i < 128; i++ ) {
	    /* Draw ammo on screen of tank 0 */
	    if( Ammo[j][i].exists ) {
		x = Round(Ammo[j][i].x) - Round(Tank[0].x);
		y = Round(Ammo[j][i].y) - Round(Tank[0].y);
		if( x < 38 && x >= -38 && y < 45 && y >= -45 )
		    PutPixel( x + 120, y + 47, color[12] );
				
		x = Round(Ammo[j][i].x - rot_xtable[Ammo[j][i].rot])
		    - Round(Tank[0].x);
		y = Round(Ammo[j][i].y - rot_ytable[Ammo[j][i].rot])
		    - Round(Tank[0].y);
		if( x < 38 && x >= -38 && y < 45 && y >= -45 )
		    PutPixel( x + 120, y + 47, color[13] );
	    }

	    /* Draw ammo on screen of tank 1 */
	    if( Ammo[j][i].exists ) {
		x = Round(Ammo[j][i].x) - Round(Tank[1].x);
		y = Round(Ammo[j][i].y) - Round(Tank[1].y);
		if( x < 38 && x >= -38 && y < 45 && y >= -45 )
		    PutPixel( x + 40, y + 47, color[12] );
				
		x = Round(Ammo[j][i].x - rot_xtable[Ammo[j][i].rot])
		    - Round(Tank[1].x);
		y = Round(Ammo[j][i].y - rot_ytable[Ammo[j][i].rot])
		    - Round(Tank[1].y);
		if( x < 38 && x >= -38 && y < 45 && y >= -45 )
		    PutPixel( x + 40, y + 47, color[13] );
	    }

	    /* Draw explosion on screen of tank j */
	    if( Expl[i].lifetime > 0.0 &&
		Round(Expl[i].x) - Round(Tank[j].x) < 38 &&
		Round(Expl[i].x) - Round(Tank[j].x) >= -38 &&
		Round(Expl[i].y) - Round(Tank[j].y) < 45 &&
		Round(Expl[i].y) - Round(Tank[j].y) >= -45 ) {
		PutPixel( Round(Expl[i].x) - Round(Tank[j].x) + 120-80*j,
			  Round(Expl[i].y) - Round(Tank[j].y) + 47,
			  color[12] );
	    }
	}
    }

    /* Draw noise */
    if( noise0 ) {
	DrawNoise( 82, 2, 76, 90 );
	noise0--;
    }
    if( noise1 ) {
	DrawNoise( 2, 2, 76, 90 );
	noise1--;
    }
}


void Explosion( double x, double y, int n, int type )
{
    double rot;
    int i, j;
	
    for( i = 0; i < n; i++ ) {
	for( j = 0; j < 128; j++ ) {
	    if( Expl[j].lifetime <= 0.0 && type == 0 ) {
		Expl[j].x = x;
		Expl[j].y = y;
		rot = 2.0*M_PI*rand()/(RAND_MAX+1.0);
		Expl[j].vx = sin(rot);
		Expl[j].vy = cos(rot);
		Expl[j].lifetime = 0.25;

		break;
	    } else if( Expl[j].lifetime <= 0.0 ) {
		Expl[j].x = x;
		Expl[j].y = y;
		rot = 2.0*M_PI*rand()/(RAND_MAX+1.0);
		Expl[j].vx = 0.5*sin(rot);
		Expl[j].vy = 0.5*cos(rot);
		Expl[j].lifetime = 0.7;

		break;
	    }
	}
    }
}


inline void CTest_Sub( int y, int x, int i )
{
    int y0, x0;

    y0 = Round( Tank[i].y );
    x0 = Round( Tank[i].x );

    if( field[y][x] > max )
	max = field[y][x];
    if( y >= y0-2 && y <= y0+2 && x >= x0-2 && x <= x0+2 && Tank[i].deathc <= 0.0 )
	max = 50;
}


int ATest( int y, int x, int i )
{
    int y0, x0;

    if( i == 0 )
	i = 1;
    else if( i == 1 )
	i = 0;

    y0 = Round( Tank[i].y );
    x0 = Round( Tank[i].x );

    if( y >= y0-2 && y <= y0+2 && x >= x0-2 && x <= x0+2 && Tank[i].deathc <= 0.0 )
	return( 50 );
	
    return( field[y][x] );
}
	
	
/*  Collision Tester
 *
 *  Returns worst of following:
 *  8,  if there is ground (field = 8 or 9) under the tank position (y,x)
 *  10, if there is rock, wall or a tank (field = 10, 30 or 40
 *       + tankcheck) under the tank position (y,x)
 *  0   otherways
 */
int CTest( int y, int x, int rot, int i )
{
    max = 0;

    if( i == 0 )
	i = 1;
    else if( i == 1 )
	i = 0;

    if( rot == 0 ) { /* To right */
	CTest_Sub( y-2, x , i );
	CTest_Sub( y-2, x+1 , i );
	CTest_Sub( y-2, x+2 , i );

	CTest_Sub( y+2, x   , i );
	CTest_Sub( y+2, x+1 , i );
	CTest_Sub( y+2, x+2 , i );

	CTest_Sub( y-1, x   , i );
	CTest_Sub( y-1, x+1 , i );

	CTest_Sub( y  , x   , i );
	CTest_Sub( y  , x+1 , i );

	CTest_Sub( y+1, x   , i );
	CTest_Sub( y+1, x+1 , i );
    } else if( rot == 1 ) { /* To down and right */
	CTest_Sub( y-1, x-1 , i );
	CTest_Sub( y  , x-1 , i );
	CTest_Sub( y+1, x-1 , i );
	CTest_Sub( y+2, x-1 , i );

	CTest_Sub( y-1, x   , i );
	CTest_Sub( y  , x   , i );
	CTest_Sub( y+1, x   , i );
	CTest_Sub( y+2, x   , i );
	CTest_Sub( y+3, x   , i );

	CTest_Sub( y-1, x+1 , i );
	CTest_Sub( y  , x+1 , i );
	CTest_Sub( y+1, x+1 , i );

	CTest_Sub( y-1, x+2 , i );
	CTest_Sub( y  , x+2 , i );

	CTest_Sub( y  , x+3 , i );
    } else if( rot == 2 ) { /* To down */
	CTest_Sub( y  , x-2 , i );
	CTest_Sub( y+1, x-2 , i );
	CTest_Sub( y+2, x-2 , i );

	CTest_Sub( y  , x+2 , i );
	CTest_Sub( y+1, x+2 , i );
	CTest_Sub( y+2, x+2 , i );

	CTest_Sub( y  , x-1 , i );
	CTest_Sub( y+1, x-1 , i );

	CTest_Sub( y  , x   , i );
	CTest_Sub( y+1, x   , i );

	CTest_Sub( y  , x+1 , i );
	CTest_Sub( y+1, x+1 , i );
    } else if( rot == 3 ) { /* To down and left */
	CTest_Sub( y-1, x-1 , i );
	CTest_Sub( y-1, x   , i );
	CTest_Sub( y-1, x+1 , i );

	CTest_Sub( y  , x-3 , i );
	CTest_Sub( y  , x-2 , i );
	CTest_Sub( y  , x-1 , i );
	CTest_Sub( y  , x   , i );
	CTest_Sub( y  , x+1 , i );

	CTest_Sub( y+1, x-2 , i );
	CTest_Sub( y+1, x-1 , i );
	CTest_Sub( y+1, x   , i );
	CTest_Sub( y+1, x+1 , i );

	CTest_Sub( y+2, x-1 , i );
	CTest_Sub( y+2, x   , i );

	CTest_Sub( y+3, x   , i );
    } else if( rot == 4 ) { /* To left */
	CTest_Sub( y-2, x   , i );
	CTest_Sub( y-2, x-1 , i );
	CTest_Sub( y-2, x-2 , i );

	CTest_Sub( y+2, x   , i );
	CTest_Sub( y+2, x-1 , i );
	CTest_Sub( y+2, x-2 , i );

	CTest_Sub( y-1, x   , i );
	CTest_Sub( y-1, x-1 , i );

	CTest_Sub( y  , x-0 , i );
	CTest_Sub( y  , x-1 , i );

	CTest_Sub( y+1, x   , i );
	CTest_Sub( y+1, x-1 , i );
    } else if( rot == 5 ) { /* To up and left */
	CTest_Sub( y  , x-3 , i );

	CTest_Sub( y  , x-2 , i );
	CTest_Sub( y+1, x-2 , i );

	CTest_Sub( y-1, x-1 , i );
	CTest_Sub( y  , x-1 , i );
	CTest_Sub( y+1, x-1 , i );

	CTest_Sub( y-3, x   , i );
	CTest_Sub( y-2, x   , i );
	CTest_Sub( y-1, x   , i );
	CTest_Sub( y  , x   , i );
	CTest_Sub( y+1, x   , i );

	CTest_Sub( y-2, x+1 , i );
	CTest_Sub( y-1, x+1 , i );
	CTest_Sub( y  , x+1 , i );
	CTest_Sub( y+1, x+1 , i );
    } else if( rot == 6 ) { /* To up */
	CTest_Sub( y  , x-2 , i );
	CTest_Sub( y-1, x-2 , i );
	CTest_Sub( y-2, x-2 , i );

	CTest_Sub( y  , x+2 , i );
	CTest_Sub( y-1, x+2 , i );
	CTest_Sub( y-2, x+2 , i );

	CTest_Sub( y  , x-1 , i );
	CTest_Sub( y-1, x-1 , i );

	CTest_Sub( y  , x   , i );
	CTest_Sub( y-1, x   , i );

	CTest_Sub( y  , x+1 , i );
	CTest_Sub( y-1, x+1 , i );	
    } else if( rot == 7 ) { /* To up and right */
	CTest_Sub( y-3, x   , i );

	CTest_Sub( y-2, x   , i );
	CTest_Sub( y-2, x+1 , i );

	CTest_Sub( y-1, x-1 , i );
	CTest_Sub( y-1, x   , i );
	CTest_Sub( y-1, x+1 , i );
	CTest_Sub( y-1, x+2 , i );

	CTest_Sub( y  , x-1 , i );
	CTest_Sub( y  , x   , i );
	CTest_Sub( y  , x+1 , i );
	CTest_Sub( y  , x+2 , i );
	CTest_Sub( y  , x+3 , i );
		
	CTest_Sub( y+1, x-1 , i );
	CTest_Sub( y+1, x   , i );
	CTest_Sub( y+1, x+1 , i );
    }

    return( max );
}


/* Clear earth under the tank */
void Tank_Tunnel( int y, int x, int rot )
{
    if( rot == 0 ) {
	TUNNEL_MACRO( field[y-2][x-2] );
	TUNNEL_MACRO( field[y-2][x-1] );
	TUNNEL_MACRO( field[y-2][x  ] );

	TUNNEL_MACRO( field[y+2][x-2] );
	TUNNEL_MACRO( field[y+2][x-1] );
	TUNNEL_MACRO( field[y+2][x  ] );

	TUNNEL_MACRO( field[y-1][x-2] );
	TUNNEL_MACRO( field[y-1][x-1] );
	TUNNEL_MACRO( field[y-1][x  ] );

	TUNNEL_MACRO( field[y  ][x-2] );
	TUNNEL_MACRO( field[y  ][x-1] );
	TUNNEL_MACRO( field[y  ][x  ] );

	TUNNEL_MACRO( field[y+1][x-2] );
	TUNNEL_MACRO( field[y+1][x-1] );
	TUNNEL_MACRO( field[y+1][x  ] );
    } else if( rot == 1 ) { /* To down and right */
	TUNNEL_MACRO( field[y  ][x-2] );
	TUNNEL_MACRO( field[y+1][x-2] );

	TUNNEL_MACRO( field[y-1][x-1] );
	TUNNEL_MACRO( field[y  ][x-1] );
	TUNNEL_MACRO( field[y+1][x-1] );
	TUNNEL_MACRO( field[y+2][x-1] );

	TUNNEL_MACRO( field[y-2][x  ] );
	TUNNEL_MACRO( field[y-1][x  ] );
	TUNNEL_MACRO( field[y  ][x  ] );
	TUNNEL_MACRO( field[y+1][x  ] );
		
	TUNNEL_MACRO( field[y-2][x+1] );
	TUNNEL_MACRO( field[y-1][x+1] );
	TUNNEL_MACRO( field[y  ][x+1] );

	TUNNEL_MACRO( field[y-1][x+2] );
    } else if( rot == 2 ) { /* To down */
	TUNNEL_MACRO( field[y-2][x-2] );
	TUNNEL_MACRO( field[y-1][x-2] );
	TUNNEL_MACRO( field[y  ][x-2] );

	TUNNEL_MACRO( field[y-2][x+2] );
	TUNNEL_MACRO( field[y-1][x+2] );
	TUNNEL_MACRO( field[y  ][x+2] );

	TUNNEL_MACRO( field[y-2][x-1] );
	TUNNEL_MACRO( field[y-1][x-1] );
	TUNNEL_MACRO( field[y  ][x-1] );

	TUNNEL_MACRO( field[y-2][x  ] );
	TUNNEL_MACRO( field[y-1][x  ] );
	TUNNEL_MACRO( field[y  ][x  ] );

	TUNNEL_MACRO( field[y-2][x+1] );
	TUNNEL_MACRO( field[y-1][x+1] );
	TUNNEL_MACRO( field[y  ][x+1] );
    } else if( rot == 3 ) { /* To down and left */
	TUNNEL_MACRO( field[y-2][x  ] );
	TUNNEL_MACRO( field[y-2][x-1] );

	TUNNEL_MACRO( field[y-1][x-2] );
	TUNNEL_MACRO( field[y-1][x-1] );
	TUNNEL_MACRO( field[y-1][x  ] );
	TUNNEL_MACRO( field[y-1][x+1] );

	TUNNEL_MACRO( field[y  ][x-1] );
	TUNNEL_MACRO( field[y  ][x  ] );
	TUNNEL_MACRO( field[y  ][x+1] );
	TUNNEL_MACRO( field[y  ][x+2] );

	TUNNEL_MACRO( field[y+1][x  ] );
	TUNNEL_MACRO( field[y+1][x+1] );
	TUNNEL_MACRO( field[y+1][x+2] );

	TUNNEL_MACRO( field[y+2][x+1] );
    }
    else if( rot == 4 ) /* To left */
    {
	TUNNEL_MACRO( field[y-2][x+2] );
	TUNNEL_MACRO( field[y-2][x+1] );
	TUNNEL_MACRO( field[y-2][x  ] );

	TUNNEL_MACRO( field[y+2][x+2] );
	TUNNEL_MACRO( field[y+2][x+1] );
	TUNNEL_MACRO( field[y+2][x  ] );

	TUNNEL_MACRO( field[y-1][x+2] );
	TUNNEL_MACRO( field[y-1][x+1] );
	TUNNEL_MACRO( field[y-1][x  ] );

	TUNNEL_MACRO( field[y  ][x+2] );
	TUNNEL_MACRO( field[y  ][x+1] );
	TUNNEL_MACRO( field[y  ][x  ] );

	TUNNEL_MACRO( field[y+1][x+2] );
	TUNNEL_MACRO( field[y+1][x+1] );
	TUNNEL_MACRO( field[y+1][x  ] );
    }
    else if( rot == 5 ) /* To up and left */
    {
	TUNNEL_MACRO( field[y+1][x-2] );

	TUNNEL_MACRO( field[y  ][x-1] );
	TUNNEL_MACRO( field[y+1][x-1] );
	TUNNEL_MACRO( field[y+2][x-1] );

	TUNNEL_MACRO( field[y-1][x  ] );
	TUNNEL_MACRO( field[y  ][x  ] );
	TUNNEL_MACRO( field[y+1][x  ] );
	TUNNEL_MACRO( field[y+2][x  ] );

	TUNNEL_MACRO( field[y-2][x+1] );
	TUNNEL_MACRO( field[y-1][x+1] );
	TUNNEL_MACRO( field[y  ][x+1] );
	TUNNEL_MACRO( field[y+1][x+1] );

	TUNNEL_MACRO( field[y-1][x+2] );
	TUNNEL_MACRO( field[y  ][x+2] );
    }
    else if( rot == 6 ) /* To up */
    {
	TUNNEL_MACRO( field[y+2][x-2] );
	TUNNEL_MACRO( field[y+1][x-2] );
	TUNNEL_MACRO( field[y  ][x-2] );

	TUNNEL_MACRO( field[y+2][x+2] );
	TUNNEL_MACRO( field[y+1][x+2] );
	TUNNEL_MACRO( field[y  ][x+2] );

	TUNNEL_MACRO( field[y+2][x-1] );
	TUNNEL_MACRO( field[y+1][x-1] );
	TUNNEL_MACRO( field[y  ][x-1] );

	TUNNEL_MACRO( field[y+2][x  ] );
	TUNNEL_MACRO( field[y+1][x  ] );
	TUNNEL_MACRO( field[y  ][x  ] );

	TUNNEL_MACRO( field[y+2][x+1] );
	TUNNEL_MACRO( field[y+1][x+1] );
	TUNNEL_MACRO( field[y  ][x+1] );
    }
    else if( rot == 7 ) /* To up and right */
    {
	TUNNEL_MACRO( field[y-2][x-1] );

	TUNNEL_MACRO( field[y-1][x-2] );
	TUNNEL_MACRO( field[y-1][x-1] );
	TUNNEL_MACRO( field[y-1][x  ] );

	TUNNEL_MACRO( field[y  ][x-2] );
	TUNNEL_MACRO( field[y  ][x-1] );
	TUNNEL_MACRO( field[y  ][x  ] );
	TUNNEL_MACRO( field[y  ][x+1] );

	TUNNEL_MACRO( field[y+1][x-1] );
	TUNNEL_MACRO( field[y+1][x  ] );
	TUNNEL_MACRO( field[y+1][x+1] );
	TUNNEL_MACRO( field[y+1][x+2] );

	TUNNEL_MACRO( field[y+2][x  ] );
	TUNNEL_MACRO( field[y+2][x+1] );
    }
}


void HandleKeys( void )
{
    int i;

    for( i = 0; i < 2; i++ )
    {
	Tank[i].oldrot = Tank[i].rot;
	Tank[i].move = 0;
	Tank[i].fire = 0;

	if( Tank[i].mode == TANK_AI )
	{
	    Handle_AI( i );
	    continue;
	}
		
	/* Get direction of movement */
	if( key_pl[i].down && key_pl[i].right )
	{
	    Tank[i].rot = 1;
	    Tank[i].move = 1;
	}
	else if( key_pl[i].down && key_pl[i].left )
	{
	    Tank[i].rot = 3;
	    Tank[i].move = 1;
	}
	else if( key_pl[i].up && key_pl[i].right )
	{
	    Tank[i].rot = 7;
	    Tank[i].move = 1;
	}
	else if( key_pl[i].up && key_pl[i].left )
	{
	    Tank[i].rot = 5;
	    Tank[i].move = 1;
	}
	else if( key_pl[i].right )
	{
	    Tank[i].rot = 0;
	    Tank[i].move = 1;
	}
	else if( key_pl[i].down )
	{
	    Tank[i].rot = 2;
	    Tank[i].move = 1;
	}
	else if( key_pl[i].left )
	{
	    Tank[i].rot = 4;
	    Tank[i].move = 1;
	}
	else if( key_pl[i].up )
	{
	    Tank[i].rot = 6;
	    Tank[i].move = 1;
	}
	if( key_pl[i].fire )
	{
	    Tank[i].fire = 1;
	}
    }
}


void HandleActions( double dt )
{
    double step;
    double dx = 0.0;
    double dy = 0.0;
    int val = 0;
    int i, j, k;
	
    HandleKeys();

    for( i = 0; i < 2; i++ )
    {
	/* Align when turning */
	if( Tank[i].oldrot != Tank[i].rot )
	{
	    Tank[i].y = Round(Tank[i].y);
	    Tank[i].x = Round(Tank[i].x);
	}

	/* Make movement */
	if( Tank[i].move && Tank[i].deathc <= 0.0 )
	{
	    if( !Tank[i].tunneling || Tank[i].fire )
		step = TANK_SPEED*dt;
	    else
		step = DIG_SPEED*dt;

	    for( k = 0; 0.5*k < step; k++ )
	    {
		val = CTest( Round(Tank[i].y + 0.5*k*rot_ytable[Tank[i].rot]),
			     Round(Tank[i].x + 0.5*k*rot_xtable[Tank[i].rot]), 
			     Tank[i].rot, i );
		if( val != 0 )
		{
		    Tank[i].tunneling = 1;
		    if( !Tank[i].fire )
			step = DIG_SPEED*dt;
		}
		if( val == 10 || val == 30 || val == 40 || val == 50 )
		    break;
	    }

	    if( val == 10 || val == 30 || val == 40 || val == 50 ) /* Rock, wall or a tank */
	    {
		if( k != 0 )
		    k--;
		Tank[i].y = Round( Tank[i].y + 0.5*k*rot_ytable[Tank[i].rot] );
		Tank[i].x = Round( Tank[i].x + 0.5*k*rot_xtable[Tank[i].rot] );
	    }
	    else
	    {
		Tank[i].y += rot_ytable[Tank[i].rot]*step;
		Tank[i].x += rot_xtable[Tank[i].rot]*step;
	    }

	    if( CTest( Round(Tank[i].y), Round(Tank[i].x), Tank[i].rot, i ) == 0 )
		Tank[i].tunneling = 0;

	    Tank_Tunnel( Round(Tank[i].y), Round(Tank[i].x), Tank[i].rot );
	}

	/* Make new ammo */
	if( Tank[i].fire && Time_Now() - Tank[i].last > FIRE_DELAY && Tank[i].deathc <= 0.0 )
	{
	    for( j = 0; j < 128; j++ )
	    {
		if( !Ammo[i][j].exists )
		{
		    Tank[i].last = Time_Now();
		    Tank[i].Energy -= ENERGY_SHOT;
		    Ammo[i][j].exists = 1;
		    Ammo[i][j].rot = Tank[i].rot;
		    Ammo[i][j].x = Round(Tank[i].x + rot_xtable[Ammo[i][j].rot]);
		    Ammo[i][j].y = Round(Tank[i].y + rot_ytable[Ammo[i][j].rot]);

		    break;
		}
	    }
	}

	/* Ammo collisions */
	for( j = 0; j < 128; j++ )
	{
	    if( Ammo[i][j].exists )
	    {
		dx = rot_xtable[Ammo[i][j].rot]*dt*AMMO_SPEED;
		dy = rot_ytable[Ammo[i][j].rot]*dt*AMMO_SPEED;

		for( k = 0; 0.5*k < dt*AMMO_SPEED; k++ )
		{
		    val = ATest( Round(Ammo[i][j].y + 0.5*k*rot_ytable[Ammo[i][j].rot]),
				 Round(Ammo[i][j].x + 0.5*k*rot_xtable[Ammo[i][j].rot]),
				 i );
		    if( val != 0 )
			break;
		}

		if( val == 8 || val == 9 )
		{
		    field[Round(Ammo[i][j].y + 0.5*k*rot_ytable[Ammo[i][j].rot])]
			[Round(Ammo[i][j].x + 0.5*k*rot_xtable[Ammo[i][j].rot])] = 0;
		    Ammo[i][j].exists = 0;
		    Explosion( Round(Ammo[i][j].x + 0.5*k*rot_xtable[Ammo[i][j].rot]),
			       Round(Ammo[i][j].y + 0.5*k*rot_xtable[Ammo[i][j].rot]), 10, 0 );
		}
		else if( val == 10 || val == 30 || val == 40 )
		{
		    k--;
		    Ammo[i][j].exists = 0;
		    Explosion( Round(Ammo[i][j].x + 0.5*k*rot_xtable[Ammo[i][j].rot]),
			       Round(Ammo[i][j].y + 0.5*k*rot_xtable[Ammo[i][j].rot]), 10, 0 );
		}
		else if( val == 50 ) /* Tank hit  */
		{
		    Ammo[i][j].exists = 0;
		    if( i == 0 )
			Tank[1].Shields -= SHOT_DAMAGE;
		    else if( i == 1 )
			Tank[0].Shields -= SHOT_DAMAGE;

		    Explosion( Round(Ammo[i][j].x + 0.5*k*rot_xtable[Ammo[i][j].rot]),
			       Round(Ammo[i][j].y + 0.5*k*rot_xtable[Ammo[i][j].rot]), 10, 0 );
		}
		else
		{
		    Ammo[i][j].y += dy;
		    Ammo[i][j].x += dx;
		}
	    }
	}

	/* Use energy */
	Tank[i].Energy -= ENERGY_DROP*dt;
		
	/* Rebirth */
	if( Tank[i].deathc > 0.0 )
	{
	    Tank[i].deathc -= dt;
	    if( Tank[i].deathc <= 0.0 )
	    {
		Tank[i].rot       = 6;
		Tank[i].tunneling = 1;
		Tank[i].x         = Tank[i].basex;
		Tank[i].y         = Tank[i].basey;
		Tank[i].Energy    = 1.0;
		Tank[i].Shields   = 1.0;
		Tank[i].deathc    = 0.0;

		if( CTest( Round(Tank[i].y), Round(Tank[i].x), Tank[i].rot, i ) == 50 )
		{
		    if( i == 0 )
			Tank[1].Shields = 0.0;
		    else if( i == 1 )
			Tank[0].Shields = 0.0;
		}
	    }
	}
		
	/* Death */
	if( Tank[i].Shields <= 0.0 && Tank[i].deathc <= 0.0 )
	{
	    Tank[i].Shields = 0.0;
	    Explosion( Tank[i].x, Tank[i].y, 30, 1 );
	    Tank[i].deathc = 4.0;
	    Tank[i].deaths++;
	}
	else if( Tank[i].Energy <= 0.0 && Tank[i].deathc <= 0.0 )
	{
	    Tank[i].Energy = 0.0;
	    Explosion( Tank[i].x, Tank[i].y, 30, 1 );
	    Tank[i].deathc = 4.0;		
	    Tank[i].deaths++;
	}

	/* Repair Shields and Energy */
	if( Tank[i].deathc <= 0.0 &&
	    Tank[i].x <= Tank[i].basex + BASE_SIZEX &&
	    Tank[i].x >= Tank[i].basex - BASE_SIZEX &&
	    Tank[i].y <= Tank[i].basey + BASE_SIZEY &&
	    Tank[i].y >= Tank[i].basey - BASE_SIZEY )
	{
	    Tank[i].Shields += REPAIR_SPEED2*dt;
	    Tank[i].Energy  += REPAIR_SPEED1*dt;

	    if( Tank[i].Shields > 1.0 )
		Tank[i].Shields = 1.0;
	    if( Tank[i].Energy > 1.0 )
		Tank[i].Energy = 1.0;
	}
		
	if( i == 0 )
	    j = 1;
	else
	    j = 0;

	if( Tank[i].deathc <= 0.0 &&
	    Tank[i].x <= Tank[j].basex + BASE_SIZEX &&
	    Tank[i].x >= Tank[j].basex - BASE_SIZEX &&
	    Tank[i].y <= Tank[j].basey + BASE_SIZEY &&
	    Tank[i].y >= Tank[j].basey - BASE_SIZEY )
	{
	    Tank[i].Energy  += REPAIR_SPEED2*dt;

	    if( Tank[i].Energy > 1.0 )
		Tank[i].Energy = 1.0;
	}
    }

    /* Explosion collisions */
    for( j = 0; j < 128; j++ )
    {
	if( Expl[j].lifetime > 0.0 )
	{
	    dx = Expl[j].vx*dt*PART_SPEED;
	    dy = Expl[j].vy*dt*PART_SPEED;
			
	    for( k = 0; 0.5*k < dt*PART_SPEED; k++)
	    {
		val = field[Round(Expl[j].y + 0.5*k*Expl[j].vy)]
		    [Round(Expl[j].x + 0.5*k*Expl[j].vx)];
		if( val != 0 )
		    break;
	    }
			
	    if( val == 8 || val == 9 )
	    {
		field[Round(Expl[j].y + 0.5*k*Expl[j].vy)]
		    [Round(Expl[j].x + 0.5*k*Expl[j].vx)] = 0;
		Expl[j].lifetime = 0.0;
	    }
	    else if( val == 10 || val == 30 || val == 40 )
	    {
		Expl[j].lifetime = 0.0;
	    }
	    else
	    {
		Expl[j].y += dy;
		Expl[j].x += dx;
	    }

	    Expl[j].lifetime -= dt;
	}
    }
}


void Init_Tanks( void )
{
    int i, j;
	
    for( j = 0; j < 2; j++ )
    {
	Tank[j].rot       = 6;
	Tank[j].tunneling = 1;
	Tank[j].x         = Tank[j].basex;
	Tank[j].y         = Tank[j].basey;
	Tank[j].Energy    = 1.0;
	Tank[j].Shields   = 1.0;
	Tank[j].deathc    = 0.0;
	Tank[j].deaths    = 0;

	for( i = 0; i < 128; i++ )
	    Ammo[j][i].exists = 0;
    }
	
    for( j = 0; j < 128; j++ )
	Expl[j].lifetime = 0.0;
}


/* End of file tunneler.c */
