/* terrain.c
 * Terrain generator for Tunneler
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


#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "terrain.h"
#include "tunneler.h"
#include "types.h"
#include "game.h"


typedef struct WALL Wall;


struct WALL
{
    int x, y;
    struct WALL *next;
};


unsigned char field[FIELD_SIZEY][FIELD_SIZEX];


void Init_Base( int y, int x, int n )
{
    int i, j;

    Tank[n].basex = x;
    Tank[n].basey = y;

    for( i = -BASE_SIZEX; i < BASE_SIZEX; i++ )
	for( j = -BASE_SIZEY; j < BASE_SIZEY; j++ )
	    field[j+y][i+x] = 0;

    for( i = -BASE_SIZEY; i < BASE_SIZEY; i++ ) {
	field[i+y][-BASE_SIZEX+x]  = 30+10*n;
	field[i+y][BASE_SIZEX-1+x] = 30+10*n;
    }

    for( j = -BASE_SIZEX; j <= -BASE_DOORSIZE; j++ ) {
	field[-BASE_SIZEY+y][j+x]  = 30+10*n;
	field[BASE_SIZEY-1+y][j+x] = 30+10*n;
    }
    for( j = BASE_DOORSIZE; j < BASE_SIZEX; j++ ) {
	field[-BASE_SIZEY+y][j+x]  = 30+10*n;
	field[BASE_SIZEY-1+y][j+x] = 30+10*n;
    }
}


Wall *Generate_Wall( void )
{
    int x;
    int skip;
    int range;
    Wall *start;
    Wall *p;
    Wall *newp;

    skip = 64;
    range = 40;

    start = malloc( sizeof(Wall) );
    start->x = 0;
    start->y = 0;
    start->next = malloc( sizeof(Wall) );;
    p = start->next;
    p->x = skip;
    p->y = 0;
    p->next = NULL;

    while( skip > 1 ) {
	x = skip / 2;
	p = start;

	while( 1 ) {
	    while( p->next != NULL && p->next->x < x )
		p = p->next;
	    if( p->next == NULL )
		break;

	    newp    = malloc( sizeof(Wall) );
	    newp->x = x;
	    newp->y = (p->y + p->next->y) / 2 - range + (int)(2.0*(double)range*rand()/(RAND_MAX+1.0));
	    newp->next = p->next;
	    p->next = newp;
	    p = newp->next;
			
	    x += skip;
	}

	skip = skip / 2;
	range = range / 2;
    }

    return( start );
}


void Free_Wall( Wall *wall )
{
    if( wall->next != NULL )
	Free_Wall( wall->next );

    free( wall );
}


void Init_Field( void )
{
    int i, j;
    int i2, j2;
    Wall *start;
    Wall *p;
	
    /* Generate background sand */
    for( i = 0; i < FIELD_SIZEY; i++ )
	for( j = 0; j < FIELD_SIZEX; j++ )
	    field[i][j] = 8 + (unsigned char)(2.0*rand()/(RAND_MAX+1.0));


    /* Generate walls */
    for( j = 0; j < FIELD_SIZEX; j+=64 ) {
	start = Generate_Wall();
		
	p = start;
	while( p != NULL ) {
	    if( j+p->x == FIELD_SIZEX )
		break;

	    for( i = 0; i < 100+p->y; i++ )
		field[i][j+p->x] = 10;

	    p = p->next;
	}

	Free_Wall( start );
    }

    for( j = 0; j < FIELD_SIZEX; j+=64 ) {
	start = Generate_Wall();
		
	p = start;
	while( p != NULL ) {
	    if( j+p->x == FIELD_SIZEX )
		break;

	    for( i = 0; i < 100+p->y; i++ )
		field[FIELD_SIZEY-i-1][j+p->x] = 10;

	    p = p->next;
	}

	Free_Wall( start );
    }

    for( j = 0; j < FIELD_SIZEY; j+=64 ) {
	start = Generate_Wall();
		
	p = start;
	while( p != NULL ) {
	    if( j+p->x == FIELD_SIZEY )
		break;

	    for( i = 0; i < 100+p->y; i++ )
		field[j+p->x][i] = 10;

	    p = p->next;
	}

	Free_Wall( start );
    }

    for( j = 0; j < FIELD_SIZEY; j+=64 ) {
	start = Generate_Wall();
		
	p = start;
	while( p != NULL ) {
	    if( j+p->x == FIELD_SIZEY )
		break;

	    for( i = 0; i < 100+p->y; i++ )
		field[j+p->x][FIELD_SIZEX-i-1] = 10;

	    p = p->next;
	}

	Free_Wall( start );
    }


    for( i = 0; i < 50; i++ )
	for( j = 0; j < FIELD_SIZEX; j++ )
	    field[i][j] = 10;

    for( i = FIELD_SIZEY-50; i < FIELD_SIZEY; i++ )
	for( j = 0; j < FIELD_SIZEX; j++ )
	    field[i][j] = 10;

    for( i = 0; i < FIELD_SIZEY; i++ )
	for( j = 0; j < 50; j++ )
	    field[i][j] = 10;

    for( i = 0; i < FIELD_SIZEY; i++ )
	for( j = FIELD_SIZEX-50; j < FIELD_SIZEX; j++ )
	    field[i][j] = 10;

	
    /* Set base positions */
    i = 150 + (int)(((double)FIELD_SIZEY-300.0)*rand()/(RAND_MAX+1.0));
    j = 150 + (int)(((double)FIELD_SIZEX-300.0)*rand()/(RAND_MAX+1.0));

    Init_Base( i, j, 0 );

    do {
	i2 = 150 + (int)(((double)FIELD_SIZEY-300.0)*rand()/(RAND_MAX+1.0));
	j2 = 150 + (int)(((double)FIELD_SIZEX-300.0)*rand()/(RAND_MAX+1.0));
    }
    while( (i-i2)*(i-i2) + (j-j2)*(j-j2) < 150*150 );
	
    Init_Base( i2, j2, 1 );
}


/* End of file terrain.c */
