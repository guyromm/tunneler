/* main.c
 * Tunneler main and menus
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
#include <time.h>
#include <SDL/SDL.h>
#include "ai.h"
#include "keys.h"
#include "types.h"
#include "graphics.h"
#include "game.h"
#include "terrain.h"
#include "tunneler.h"
#include "config.h"


char *argv0;
char *conffile;


/* Write settings to configuration file */
void Write_Config( void )
{
    FILE *fp;

    fp = fopen( conffile, "w" );
    if( fp == NULL ) {
	printf( "Couldn't write configuration file %s!\n", conffile );
	exit( 1 );
    }

    printf( "Writing %s\n", conffile );
    fprintf( fp, "# Tunneler configuration file.\n" );
    fprintf( fp, "# Do not edit.\n" );
    fprintf( fp, "fullscreen = %d\n", Video_fullscreen == SDL_FULLSCREEN );
    fprintf( fp, "width = %d\n", Video_X );
    fprintf( fp, "height = %d\n", Video_Y );

    fprintf( fp, "pl0_up = %d\n",    sym_pl[0].up );
    fprintf( fp, "pl0_down = %d\n",  sym_pl[0].down );
    fprintf( fp, "pl0_left = %d\n",  sym_pl[0].left );
    fprintf( fp, "pl0_right = %d\n", sym_pl[0].right );
    fprintf( fp, "pl0_fire = %d\n",  sym_pl[0].fire );

    fprintf( fp, "pl1_up = %d\n",    sym_pl[1].up );
    fprintf( fp, "pl1_down = %d\n",  sym_pl[1].down );
    fprintf( fp, "pl1_left = %d\n",  sym_pl[1].left );
    fprintf( fp, "pl1_right = %d\n", sym_pl[1].right );
    fprintf( fp, "pl1_fire = %d\n",  sym_pl[1].fire );

    fclose( fp );
}


/* Read contents of configuration file. A new file is created if there
 * is none. */
void Read_Config( void )
{
    FILE *fp;
    int k;
    char buf[256];
    char *home;

#ifndef WIN32
    home = getenv( "HOME" );
    if( home == NULL ) {
	printf( "Environment variable HOME not defined!\n" );
	printf( "Couldn't read configuration file!\n" );
	return;
    }

    conffile = malloc( strlen(home) + strlen(CONF_FILE) + 1 );
    sprintf( conffile, "%s/%s", home, CONF_FILE );
#else
    char *end;
    char *str;

    if( *argv0 == '\"' )
	argv0++;

    end = argv0;
    while( *end != '\0' )
	end++;

    while( *end != '\\' && end != argv0 )
	end--;

    if( *end == '\\' ) {
	end++;

	home = malloc( end-argv0+1 );
	str = home;
	while( argv0 != end )
	{
	    *str = *argv0;
	    argv0++;
	    str++;
	}
	*str = '\0';

	conffile = malloc( strlen(home) + strlen(CONF_FILE) + 1 );
	sprintf( conffile, "%s\\%s", home, CONF_FILE );
    } else {
	conffile = malloc( strlen(CONF_FILE) + 1 );
	sprintf( conffile, "%s", CONF_FILE );
    }
#endif

    fp = fopen( conffile, "r" );
    if( fp == NULL ) {
	printf( "Couldn't read configuration file!\n" );

	sym_pl[0].up    = SDLK_UP;
	sym_pl[0].down  = SDLK_DOWN;
	sym_pl[0].left  = SDLK_LEFT;
	sym_pl[0].right = SDLK_RIGHT;
	sym_pl[0].fire  = SDLK_RSHIFT;

	sym_pl[1].up    = SDLK_w;
	sym_pl[1].down  = SDLK_s;
	sym_pl[1].left  = SDLK_a;
	sym_pl[1].right = SDLK_d;
	sym_pl[1].fire  = SDLK_LCTRL;

	Write_Config();
	return;
    }

    /* Read configuration file */
    k = 0;
    while( 1 ) {
	k++;
	if( fgets( buf, 256, fp ) == NULL )
	    break;

	/* Skip commented and empty lines */
	if( buf[0] == '#' || buf[0] == '\0' || buf[0] == '\n' )
	    continue;

	if( !strncmp( buf, "fullscreen =", 12 ) ) {
	    if( atoi( &buf[12] ) == 1 )
		Video_fullscreen = SDL_FULLSCREEN;
	}
	else if( !strncmp( buf, "width =", 7 ) )
	    Video_X = atoi( &buf[7] );
	else if( !strncmp( buf, "height =", 8 ) )
	    Video_Y = atoi( &buf[8] );
	else if( !strncmp( buf, "pl0_up =", 8 ) )
	    sym_pl[0].up = atoi( &buf[8] );
	else if( !strncmp( buf, "pl0_down =", 10 ) )
	    sym_pl[0].down = atoi( &buf[10] );
	else if( !strncmp( buf, "pl0_left =", 10 ) )
	    sym_pl[0].left = atoi( &buf[10] );
	else if( !strncmp( buf, "pl0_right =", 11 ) )
	    sym_pl[0].right = atoi( &buf[11] );
	else if( !strncmp( buf, "pl0_fire =", 10 ) )
	    sym_pl[0].fire = atoi( &buf[10] );
	else if( !strncmp( buf, "pl1_up =", 8 ) )
	    sym_pl[1].up = atoi( &buf[8] );
	else if( !strncmp( buf, "pl1_down =", 10 ) )
	    sym_pl[1].down = atoi( &buf[10] );
	else if( !strncmp( buf, "pl1_left =", 10 ) )
	    sym_pl[1].left = atoi( &buf[10] );
	else if( !strncmp( buf, "pl1_right =", 11 ) )
	    sym_pl[1].right = atoi( &buf[11] );
	else if( !strncmp( buf, "pl1_fire =", 10 ) )
	    sym_pl[1].fire = atoi( &buf[10] );
	else {
	    printf( "Unknown field in configuration file on line %d\n", k );
	    exit( 1 );
	}
    }

    fclose( fp );
}


void HandleEvents( void )
{
    SDL_Event event;

    while( SDL_PollEvent( &event ) ) {
	if( event.type == SDL_KEYDOWN || event.type == SDL_KEYUP )
	    HandleKeyEvent( &event.key );
	else if( event.type == SDL_QUIT ) {
	    printf( "Fast quit\n" );
	    exit( 1 );
	}
    }
}


int GetKeyPress( void )
{
    SDL_KeyboardEvent *key;
    SDL_Event event;

    while( 1 ) {
	while( SDL_PollEvent( &event ) ) {
	    if( event.type == SDL_KEYDOWN ) {
		key = &event.key;
		return( key->keysym.sym );
	    } else if( event.type == SDL_QUIT ) {
		printf( "Fast quit\n" );
		exit( 1 );
	    }
	}

	SDL_Delay( 50 );
    }

    return( 0 );
}


void PrintKey( int x, int y, int key, Uint32 color)
{
    char str[22];

    switch( key ) {
    case SDLK_UP:
	snprintf( str, 21, "%5s", "up" );
	break;
    case SDLK_DOWN:
	snprintf( str, 21, "%5s", "down" );
	break;
    case SDLK_LEFT:
	snprintf( str, 21, "%5s", "left" );
	break;
    case SDLK_RIGHT:
	snprintf( str, 21, "%5s", "right" );
	break;
    case SDLK_RSHIFT:
	snprintf( str, 21, "%5s", "rshft" );
	break;
    case SDLK_LSHIFT:
	snprintf( str, 21, "%5s", "lshft" );
	break;
    case SDLK_RCTRL:
	snprintf( str, 21, "%5s", "rctrl" );
	break;
    case SDLK_LCTRL:
	snprintf( str, 21, "%5s", "lctrl" );
	break;
    case SDLK_RETURN:
	snprintf( str, 21, "%5s", "rtrn" );
	break;
    case SDLK_TAB:
	snprintf( str, 21, "%5s", "tab" );
	break;
    case SDLK_BACKSPACE:
	snprintf( str, 21, "%5s", "bspc" );
	break;
    case SDLK_SPACE:
	snprintf( str, 21, "%5s", "spc" );
	break;
    case SDLK_SLASH:
	snprintf( str, 21, "%5s", "slash" );
	break;
    case SDLK_PLUS:
	snprintf( str, 21, "%5s", "+" );
	break;
    case SDLK_MINUS:
	snprintf( str, 21, "%5s", "-" );
	break;
    case SDLK_COMMA:
	snprintf( str, 21, "%5s", "," );
	break;
    case SDLK_PERIOD:
	snprintf( str, 21, "%5s", "." );
	break;
    case SDLK_COLON:
	snprintf( str, 21, "%5s", ":" );
	break;
    case SDLK_SEMICOLON:
	snprintf( str, 21, "%5s", ";" );
	break;
    case SDLK_LESS:
	snprintf( str, 21, "%5s", "<" );
	break;
    case SDLK_EQUALS:
	snprintf( str, 21, "%5s", "=" );
	break;
    case SDLK_GREATER:
	snprintf( str, 21, "%5s", ">" );
	break;
    case SDLK_AT:
	snprintf( str, 21, "%5s", "@" );
	break;
    case SDLK_LEFTBRACKET:
	snprintf( str, 21, "%5s", "[" );
	break;
    case SDLK_BACKSLASH:
	snprintf( str, 21, "%5s", "\\" );
	break;
    case SDLK_RIGHTBRACKET:
	snprintf( str, 21, "%5s", "]" );
	break;
    case SDLK_0:
	snprintf( str, 21, "%5s", "0" );
	break;
    case SDLK_1:
	snprintf( str, 21, "%5s", "1" );
	break;
    case SDLK_2:
	snprintf( str, 21, "%5s", "2" );
	break;
    case SDLK_3:
	snprintf( str, 21, "%5s", "3" );
	break;
    case SDLK_4:
	snprintf( str, 21, "%5s", "4" );
	break;
    case SDLK_5:
	snprintf( str, 21, "%5s", "5" );
	break;
    case SDLK_6:
	snprintf( str, 21, "%5s", "6" );
	break;
    case SDLK_7:
	snprintf( str, 21, "%5s", "7" );
	break;
    case SDLK_8:
	snprintf( str, 21, "%5s", "8" );
	break;
    case SDLK_9:
	snprintf( str, 21, "%5s", "9" );
	break;
    case SDLK_a:
	snprintf( str, 21, "%5s", "a" );
	break;
    case SDLK_b:
	snprintf( str, 21, "%5s", "b" );
	break;
    case SDLK_c:
	snprintf( str, 21, "%5s", "c" );
	break;
    case SDLK_d:
	snprintf( str, 21, "%5s", "d" );
	break;
    case SDLK_e:
	snprintf( str, 21, "%5s", "e" );
	break;
    case SDLK_f:
	snprintf( str, 21, "%5s", "f" );
	break;
    case SDLK_g:
	snprintf( str, 21, "%5s", "g" );
	break;
    case SDLK_h:
	snprintf( str, 21, "%5s", "h" );
	break;
    case SDLK_i:
	snprintf( str, 21, "%5s", "i" );
	break;
    case SDLK_j:
	snprintf( str, 21, "%5s", "j" );
	break;
    case SDLK_k:
	snprintf( str, 21, "%5s", "k" );
	break;
    case SDLK_l:
	snprintf( str, 21, "%5s", "l" );
	break;
    case SDLK_m:
	snprintf( str, 21, "%5s", "m" );
	break;
    case SDLK_n:
	snprintf( str, 21, "%5s", "n" );
	break;
    case SDLK_o:
	snprintf( str, 21, "%5s", "o" );
	break;
    case SDLK_p:
	snprintf( str, 21, "%5s", "p" );
	break;
    case SDLK_q:
	snprintf( str, 21, "%5s", "q" );
	break;
    case SDLK_r:
	snprintf( str, 21, "%5s", "r" );
	break;
    case SDLK_s:
	snprintf( str, 21, "%5s", "s" );
	break;
    case SDLK_t:
	snprintf( str, 21, "%5s", "t" );
	break;
    case SDLK_u:
	snprintf( str, 21, "%5s", "u" );
	break;
    case SDLK_v:
	snprintf( str, 21, "%5s", "v" );
	break;
    case SDLK_w:
	snprintf( str, 21, "%5s", "w" );
	break;
    case SDLK_x:
	snprintf( str, 21, "%5s", "x" );
	break;
    case SDLK_y:
	snprintf( str, 21, "%5s", "y" );
	break;
    case SDLK_z:
	snprintf( str, 21, "%5s", "z" );
	break;

    default:
	snprintf( str, 21, "k_%03d", key );
    }

    PutStr( x, y, str, color );
}


void Settings( void )
{
    int vmode;
    int j = 0;
    char str[22];
    char str2[22];

    if( Video_fullscreen == SDL_FULLSCREEN )
    {
	for( vmode = 0; fs_modes[vmode]; vmode++ )
	{
	    if( fs_modes[vmode]->w == Video_X &&
		fs_modes[vmode]->h == Video_Y )
		break;
	}
    }
    else
    {
	for( vmode = 0; win_modes[vmode]; vmode++ )
	{
	    if( win_modes[vmode]->w == Video_X &&
		win_modes[vmode]->h == Video_Y )
		break;
	}
    }

    while( !key_quit )
    {
	HandleEvents();
		
	if( key_menu_up )
	    j--;
	else if( key_menu_down )
	    j++;

	if( j >= 2 && j <= 6 && (key_menu_left || key_menu_right) )
	    j += 5;
	else if( j >= 7 && j <= 11 && (key_menu_left || key_menu_right) )
	    j -= 5;
		
	if( j == 12 )
	    j = 0;
	else if( j == -1 )
	    j = 11;
		
	key_menu_up    = 0;
	key_menu_down  = 0;
	key_menu_left  = 0;
	key_menu_right = 0;

	SDL_FillRect( screen, NULL, color[0] );

	PutStr( 6*8, 2*8,  "Settings",           color[12] );
	PutStr( 1*8, 4*8,  "Fullscreen:",        color[12] );
	PutStr( 1*8, 5*8,  "Mode:",              color[12] );
	PutStr( 1*8, 7*8,  "       Tank1 Tank2", color[12] );
	PutStr( 1*8, 8*8,  "Up:",                color[12] );
	PutStr( 1*8, 9*8,  "Down:",              color[12] );
	PutStr( 1*8, 10*8, "Left:",              color[12] );
	PutStr( 1*8, 11*8, "Right:",             color[12] );
	PutStr( 1*8, 12*8, "Fire:",              color[12] );

	snprintf( str, 21, "%6s", Video_fullscreen == SDL_FULLSCREEN ? "true" : "false" );
	PutStr( 13*8, 4*8, str,	color[j == 0 ? 12 : 13] );

	snprintf( str2, 21, "%dx%d", Video_X, Video_Y );
	snprintf( str, 21, "%9s", str2 );
	PutStr( 10*8, 5*8, str, color[j == 1 ? 12 : 13] );

	PrintKey(  8*8,  8*8, sym_pl[0].up,    color[j ==  2 ? 12 : 13] );
	PrintKey(  8*8,  9*8, sym_pl[0].down,  color[j ==  3 ? 12 : 13] );
	PrintKey(  8*8, 10*8, sym_pl[0].left,  color[j ==  4 ? 12 : 13] );
	PrintKey(  8*8, 11*8, sym_pl[0].right, color[j ==  5 ? 12 : 13] );
	PrintKey(  8*8, 12*8, sym_pl[0].fire,  color[j ==  6 ? 12 : 13] );

	PrintKey( 14*8,  8*8, sym_pl[1].up,    color[j ==  7 ? 12 : 13] );
	PrintKey( 14*8,  9*8, sym_pl[1].down,  color[j ==  8 ? 12 : 13] );
	PrintKey( 14*8, 10*8, sym_pl[1].left,  color[j ==  9 ? 12 : 13] );
	PrintKey( 14*8, 11*8, sym_pl[1].right, color[j == 10 ? 12 : 13] );
	PrintKey( 14*8, 12*8, sym_pl[1].fire,  color[j == 11 ? 12 : 13] );

	if( key_menu_enter )
	{
	    if( j == 0 )
	    {
		if( Video_fullscreen == SDL_FULLSCREEN )
		    Video_fullscreen = 0;
		else
		    Video_fullscreen = SDL_FULLSCREEN;

		vmode = 0;
		if( Video_fullscreen == SDL_FULLSCREEN )
		{
		    Video_X = fs_modes[vmode]->w;
		    Video_Y = fs_modes[vmode]->h;
		}
		else
		{
		    Video_X = win_modes[vmode]->w;
		    Video_Y = win_modes[vmode]->h;
		}
				
	    }
	    else if( j == 1 && Video_fullscreen == SDL_FULLSCREEN )
	    {
		vmode++;
		if( fs_modes[vmode] != NULL )
		{
		    Video_X = fs_modes[vmode]->w;
		    Video_Y = fs_modes[vmode]->h;
		}
		else
		{
		    vmode = 0;
		    Video_X = fs_modes[vmode]->w;
		    Video_Y = fs_modes[vmode]->h;
		}
	    }
	    else if( j == 1 && Video_fullscreen == 0 )
	    {
		vmode++;
		if( win_modes[vmode] != NULL )
		{
		    Video_X = win_modes[vmode]->w;
		    Video_Y = win_modes[vmode]->h;
		}
		else
		{
		    vmode = 0;
		    Video_X = win_modes[vmode]->w;
		    Video_Y = win_modes[vmode]->h;
		}
	    }
	    else if( j == 2 )
	    {
		PrintKey( 8*8, 8*8, sym_pl[0].up, color[0] );
		PutStr( 8*8, 8*8, "key", color[12] );
		SDL_Flip( screen );

		sym_pl[0].up = GetKeyPress();
	    }
	    else if( j == 3 )
	    {
		PrintKey( 8*8, 9*8, sym_pl[0].down, color[0] );
		PutStr( 8*8, 9*8, str, color[0] );
		PutStr( 8*8, 9*8, "key", color[12] );
		SDL_Flip( screen );

		sym_pl[0].down = GetKeyPress();
	    }
	    else if( j == 4 )
	    {
		PrintKey( 8*8, 10*8, sym_pl[0].left, color[0] );
		PutStr( 8*8, 10*8, str, color[0] );
		PutStr( 8*8, 10*8, "key", color[12] );
		SDL_Flip( screen );

		sym_pl[0].left = GetKeyPress();
	    }
	    else if( j == 5 )
	    {
		PrintKey( 8*8, 11*8, sym_pl[0].right, color[0] );
		PutStr( 8*8, 11*8, str, color[0] );
		PutStr( 8*8, 11*8, "key", color[12] );
		SDL_Flip( screen );

		sym_pl[0].right = GetKeyPress();
	    }
	    else if( j == 6 )
	    {
		PrintKey( 8*8, 12*8, sym_pl[0].fire, color[0] );
		PutStr( 8*8, 12*8, str, color[0] );
		PutStr( 8*8, 12*8, "key", color[12] );
		SDL_Flip( screen );

		sym_pl[0].fire = GetKeyPress();
	    }
	    else if( j == 7 )
	    {
		PrintKey( 14*8, 8*8, sym_pl[1].up, color[0] );
		PutStr( 14*8, 8*8, str, color[0] );
		PutStr( 14*8, 8*8, "key", color[12] );
		SDL_Flip( screen );

		sym_pl[1].up = GetKeyPress();
	    }
	    else if( j == 8 )
	    {
		PrintKey( 14*8, 9*8, sym_pl[1].down, color[0] );
		PutStr( 14*8, 9*8, str, color[0] );
		PutStr( 14*8, 9*8, "key", color[12] );
		SDL_Flip( screen );

		sym_pl[1].down = GetKeyPress();
	    }
	    else if( j == 9 )
	    {
		PrintKey( 14*8, 10*8, sym_pl[1].left, color[0] );
		PutStr( 14*8, 10*8, str, color[0] );
		PutStr( 14*8, 10*8, "key", color[12] );
		SDL_Flip( screen );

		sym_pl[1].left = GetKeyPress();
	    }
	    else if( j == 10 )
	    {
		PrintKey( 14*8, 11*8, sym_pl[1].right, color[0] );
		PutStr( 14*8, 11*8, str, color[0] );
		PutStr( 14*8, 11*8, "key", color[12] );
		SDL_Flip( screen );

		sym_pl[1].right = GetKeyPress();
	    }
	    else if( j == 11 )
	    {
		PrintKey( 14*8, 12*8, sym_pl[1].fire, color[0] );
		PutStr( 14*8, 12*8, str, color[0] );
		PutStr( 14*8, 12*8, "key", color[12] );
		SDL_Flip( screen );

		sym_pl[1].fire = GetKeyPress();
	    }

	    if( j == 0 || j == 1 )
	    {
		screen = SDL_SetVideoMode( Video_X, Video_Y, 16,
					   SDL_HWSURFACE | Video_fullscreen | SDL_DOUBLEBUF );
		if( screen == NULL )
		{
		    printf( "Couldn't set video mode %dx%dx16: %s\n", 
			    Video_X, Video_Y, SDL_GetError() );
		    exit( 1 );
		}
	    }
			
	    key_menu_enter = 0;
	}
		
	SDL_Flip( screen );
	SDL_Delay( 10 );
    }
	
    key_quit = 0;
}


void Information( void )
{
    char str[22];

    SDL_FillRect( screen, NULL, color[0] );
    snprintf( str, 21, "SDL Tunneler was" );
    PutStr( 8, 8, str, color[12] );
    snprintf( str, 21, "made by Taneli" );
    PutStr( 8, 16, str, color[12] );
    snprintf( str, 21, "Kalvas in 2003. " );
    PutStr( 8, 24, str, color[12] );

    snprintf( str, 21, "The original DOS" );
    PutStr( 8, 40, str, color[12] );
    snprintf( str, 21, "Tunneler was made" );
    PutStr( 8, 48, str, color[12] );
    snprintf( str, 21, "by Geoffrey" );
    PutStr( 8, 56, str, color[12] );
    snprintf( str, 21, "Silverton in 1991." );
    PutStr( 8, 64, str, color[12] );

    snprintf( str, 21, "This game is free" );
    PutStr( 8, 80, str, color[12] );
    snprintf( str, 21, "software. Happy" );
    PutStr( 8, 88, str, color[12] );
    snprintf( str, 21, "Tunneling!" );
    PutStr( 8, 96, str, color[12] );

    key_menu_enter = 0;
    key_quit = 0;
		
    SDL_Flip( screen );
    SDL_Delay( 10 );

    while( !key_quit )
    {
	HandleEvents();

	if( key_menu_enter )
	{
	    key_menu_enter = 0;
	    break;
	}

	SDL_Delay( 10 );
    }
	
    key_quit = 0;
}


void Print_Stats( void )
{
    char str[22];

    SDL_FillRect( screen, NULL, color[0] );

    snprintf( str, 21, "Victories:" );
    PutStr( 25, 20, str, color[12] );

    snprintf( str, 21, "Tank 1: %d", Tank[1].deaths );
    PutStr( 25, 35, str, color[30] );

    snprintf( str, 21, "Tank 2: %d", Tank[0].deaths );
    PutStr( 25, 43, str, color[40] );

    key_menu_enter = 0;
    key_quit = 0;
		
    SDL_Flip( screen );
    SDL_Delay( 10 );

    while( !key_quit )
    {
	HandleEvents();

	if( key_menu_enter )
	{
	    key_menu_enter = 0;
	    break;
	}

	SDL_Delay( 10 );
    }
	
    key_quit = 0;
}


void Print_Field( void )
{
    int x, y;
    int i, j;
	
    key_menu_enter = 0;

    SDL_FillRect( screen, NULL, color[0] );
    SDL_LockSurface( screen );

    for( j = 0; j < Video_Y; j++ )
    {
	for( i = 0; i < Video_X; i++ )
	{
	    x = i*FIELD_SIZEX/Video_X;
	    y = j*FIELD_SIZEY/Video_Y;
			
	    if( x < 50 || x > FIELD_SIZEX-50 || y < 50 || y > FIELD_SIZEY-50 )
		PutPhysPixel( i, j, color[2] );
	    else
		PutPhysPixel( i, j, color[field[y][x]] );
	}
    }

    SDL_UnlockSurface( screen );
    SDL_Flip( screen );
    SDL_Delay( 10 );

    while( !key_quit )
    {
	HandleEvents();

	if( key_menu_enter )
	{
	    key_menu_enter = 0;
	    break;
	}

	SDL_Delay( 10 );
    }

    key_quit = 0;
}


void Main_Game( void )
{
    double dt;

    SDL_FillRect( screen, NULL, color[0] );
    DrawFrames();
    SDL_Flip( screen );
    DrawFrames();

    Init_Field();
    Init_Tanks();
    Init_Timer();

    while( !key_quit )
    {
	dt = Timer();
	HandleEvents();
	HandleActions( dt );
		
	Draw();
	SDL_Flip( screen );
    }

    key_quit = 0;
}
	
	
int main( int argc, char *argv[] )
{
    int j;
    int i = 1;

    argv0 = argv[0];
    Read_Config();

    while( argv[i] != NULL )
    {
	if( !strcmp( argv[i], "-w" ) )
	{
	    i++;
	    if( argv[i] == NULL )
	    {
		printf( "-w needs a numerical argument\n" );
		exit( 1 );
	    }
	    Video_X = atoi( argv[i] );
	    if( Video_X < 160 )
	    {
		printf( "Screen width needs to be more than 160\n" );
		exit( 1 );
	    }
	}
	else if( !strcmp( argv[i], "-h" ) )
	{
	    i++;
	    if( argv[i] == NULL )
	    {
		printf( "-h needs a numerical argument\n" );
		exit( 1 );
	    }
	    Video_Y = atoi( argv[i] );
	    if( Video_Y < 120 )
	    {
		printf( "Screen width needs to be more than 120\n" );
		exit( 1 );
	    }
	}
	else if( !strcmp( argv[i], "-ai" ) )
	{
	    i++;
	    if( argv[i] == NULL )
	    {
		printf( "-ai needs a numerical argument\n" );
		exit( 1 );
	    }
	    j = atoi( argv[i] );
	    if( j < 0 || j > 1 )
	    {
		printf( "Argument to -ai must be 0 or 1.\n" );
		exit( 1 );
	    }
	    Tank[j].mode = TANK_AI;
	    Init_AI();
	}
	else if( !strcmp( argv[i], "-version" ) || !strcmp( argv[i], "--version" ) )
	{
	    printf( "SDL Tunneler v." VERSION "\n" );
	    exit( 1 );
	}
	else if( !strcmp( argv[i], "-fullscreen" ) || !strcmp( argv[i], "--fullscreen" ) )
	{
	    Video_fullscreen = SDL_FULLSCREEN;
	}
	else if( !strcmp( argv[i], "--listmodes" ) )
	{
	    if( SDL_Init(SDL_INIT_VIDEO) < 0 )
	    {
		printf( "Couldn't initialize SDL: %s\n", SDL_GetError() );
		exit( 1 );
	    }
	    atexit( SDL_Quit );

	    /* Windowed modes */
	    win_modes = SDL_ListModes( NULL, SDL_HWSURFACE | SDL_DOUBLEBUF );
	    if( win_modes == (SDL_Rect **)0 )
	    {
		printf( "No modes available windowed!\n" );
		exit( 1 );
	    }
	    if( win_modes == (SDL_Rect **)-1 )
	    {
		printf( "All resolutions available windowed.\n" );
	    }
	    else
	    {
		printf( "Available modes windowed:\n" );
		for( j = 0; win_modes[j]; j++ )
		    printf("  %dx%d\n", win_modes[j]->w, win_modes[j]->h);
	    }

	    /* Fullscreen modes */
	    fs_modes = SDL_ListModes( NULL, SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF );
	    if( fs_modes == (SDL_Rect **)0 )
	    {
		printf( "No modes available fullscreen!\n" );
		exit( 1 );
	    }
	    if( fs_modes == (SDL_Rect **)-1 )
	    {
		printf( "All resolutions available fullscreen.\n" );
	    }
	    else
	    {
		printf( "Available modes fullscreen:\n" );
		for( j = 0; fs_modes[j]; j++ )
		    printf("  %dx%d\n", fs_modes[j]->w, fs_modes[j]->h);
	    }

	    return( 0 );
	}
	else
	{
	    printf( "Tunneler v." VERSION "\n" );
	    printf( "Copyright (c) 2004,2007 Taneli Kalvas\n" );
	    printf( "\n" );
	    printf( "Usage: tunneler [options]\n" );
	    printf( "  -w width       set width of screen\n" );
	    printf( "  -h height      set height of screen\n" );
	    printf( "  -ai [0,1]      set tank as AI player (under development)\n" );
	    printf( "  --listmodes    list videomodes supported by SDL\n" );
	    printf( "  --fullscreen   use fullscreen videomode\n" );
	    printf( "  --version      display version\n" );
	    return( 0 );
	}

	i++;
    }

    srand( time(NULL) );
    Init_Video();
    Init_Font();

    /* Menu */
    while( !key_quit )
    {
	j = 0;
	key_menu_up    = 0;
	key_menu_down  = 0;
	key_menu_left  = 0;
	key_menu_right = 0;
	key_menu_enter = 0;

	while( !key_quit )
	{
	    HandleEvents();

	    if( key_menu_up )
		j--;
	    else if( key_menu_down )
		j++;

	    if( j == 4 )
		j = 0;
	    else if( j == -1 )
		j = 3;

	    key_menu_up = 0;
	    key_menu_down = 0;

	    SDL_FillRect( screen, NULL, color[0] );
	    PutStr( 67, 11, "SDL", color[12] );
	    PutStr( 18, 20, "Tunneler v." VERSION, color[12] );
	    PutStr( 50, 55, "Start Game",     color[j == 0 ? 12 : 13] );
	    PutStr( 50, 65, "Settings",       color[j == 1 ? 12 : 13] );
	    PutStr( 50, 75, "Information",    color[j == 2 ? 12 : 13] );
	    PutStr( 50, 85, "Quit",           color[j == 3 ? 12 : 13] );

	    if( key_menu_enter )
	    {
		key_menu_enter = 0;

		if( j == 0 )
		{
		    Main_Game();
		    Print_Field();
		    Print_Stats();
		}
		else if( j == 1 )
		{
		    Settings();
		    Write_Config();
		}
		else if( j == 2 )
		{
		    Information();
		}
		else
		{
		    key_quit = 1;
		}
				
		break;
	    }

	    SDL_Flip( screen );
	    SDL_Delay( 10 );
	}
    }

    return( 0 );
}


