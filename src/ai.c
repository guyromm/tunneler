/* ai.c
 * Tunneler Artificial Intelligence
 * Last modified 28 Jun 2004
 *
 * Copyright (c) 2004 Taneli Kalvas
 *
 * This file is part of SDL Tunneler
 *
 * SDL Tunneler is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * SDL Tunneler is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SDL Tunneler ; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Questions, comments and bug reports should be sent to the author 
 * directly via email at tvkalvas@cc.jyu.fi
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "types.h"
#include "tunneler.h"
#include "game.h"
#include "timer.h"
#include "terrain.h"


/* Directions
 *
 *  5 6 7
 *  4   0
 *  3 2 1
 */

/* AI brain parameters */
unsigned long last_turn[2];
int evade[2];
unsigned long evade_time[2];

void Init_AI( void )
{
	last_turn[0] = 0;
	last_turn[1] = 0;
	evade_time[0] = 0;
	evade_time[1] = 0;
	evade[0] = 0;
	evade[1] = 0;
}


int PathClear( int x, int y, double dx, double dy )
{
	int x0, y0;
	int k;
	double r;

	r = 0.0;
	while( r < 100.0 )
	{
		x0 = Round(x + r*dx);
		y0 = Round(y + r*dy);

		k = 0;
		if( field[y0][x0] > k )
			k = field[y0][x0];
		if( field[y0+1][x0+1] > k )
			k = field[y0+1][x0+1];
		if( field[y0-1][x0+1] > k )
			k = field[y0-1][x0+1];
		if( field[y0-1][x0-1] > k )
			k = field[y0-1][x0-1];
		if( field[y0+1][x0-1] > k )
			k = field[y0+1][x0-1];

		if( k >= 10 )
			return( 0 );

		r += 1.0;
	}

	return( 1 );
}



/*  Handle AI
 *
 *  Function should set tanks rot, move and fire
 *  using x and y coordinates of tanks, field and bases
 *  and the Energy and Shields
 */
void Handle_AI( int i )
{
	int enemy;
	int targetx, targety;
	double dx, dy;
	double t;         /* Direction of movement in rad [-PI,PI] */

	if( i == 0 )
		enemy = 1;
	else
		enemy = 0;
	
	targetx = Tank[enemy].x;
	targety = Tank[enemy].y;

	/* Get direction */
	dx = targetx - Tank[i].x;
	dy = targety - Tank[i].y;
	t = -1.0*atan2( dy, dx );
	dx = cos(t);
	dy = sin(t);

	if( evade[i] == 1 )
	{
		if( PathClear( Tank[i].x, Tank[i].y, dx, dy ) )
		{
			printf( "Break evade -------------------\n" );
			evade[i] = 0;
		}
		else
		{
			t += M_PI/2.0;
			if( evade_time[i] + 1500 < Time_Now() )
				evade[i] = 0;
		}
	}
	else if( PathClear( Tank[i].x, Tank[i].y, dx, dy ) )
	{
		printf( "t = %lf\t ok!\n", t );
	}
	else if( Tank[i].x <= Tank[i].basex + BASE_SIZEX + 5 &&
		 Tank[i].x >= Tank[i].basex - BASE_SIZEX - 5 &&
		 Tank[i].y <= Tank[i].basey + BASE_SIZEY + 5 &&
		 Tank[i].y >= Tank[i].basey - BASE_SIZEY - 5 )
	{
		printf( "out of base!\n" );

		if( t >= 0.0 )
			t = M_PI/2.0;
		else
			t = -M_PI/2.0;
	}
	else
	{
		/* Evasive action! */
		printf( "start evade!\n" );
		evade[i] = 1;
		evade_time[i] = Time_Now();
	}


	while( t > M_PI )
		t -= 2.0*M_PI;
	while( t < -M_PI )
		t += 2.0*M_PI;

	if( Time_Now() > last_turn[i] + 300 )
	{
		if( t >= -M_PI/8.0 && t < M_PI/8.0 )
			Tank[i].rot = 0;
		else if( t >= -3.0*M_PI/8.0 && t < -M_PI/8.0 )
			Tank[i].rot = 1;
		else if( t >= -5.0*M_PI/8.0 && t < -3.0*M_PI/8.0 )
			Tank[i].rot = 2;
		else if( t >= -7.0*M_PI/8.0 && t < -5.0*M_PI/8.0 )
			Tank[i].rot = 3;
		else if( t < -7.0*M_PI/8.0 || t >= 7.0*M_PI/8.0 )
			Tank[i].rot = 4;
		else if( t >= 5.0*M_PI/8.0 && t < 7.0*M_PI/8.0 )
			Tank[i].rot = 5;
		else if( t >= 3.0*M_PI/8.0 && t < 5.0*M_PI/8.0 )
			Tank[i].rot = 6;
		else
			Tank[i].rot = 7;

		if( Tank[i].rot != Tank[i].oldrot )
			last_turn[i] = Time_Now();
	}

	Tank[i].move = 1;
	
	/* Fire? */
	dx = targetx - Tank[i].x;
	dy = targety - Tank[i].y;
	if( sqrt(dx*dx + dy*dy) < 100 )
		Tank[i].fire = 1;
}

