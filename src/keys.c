/* keys.c
 * Keyboard functions
 * Last modified 26 Jun 2004
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
#include <SDL/SDL.h>
#include "types.h"


/* Keyboard definitions */
player_keys_t sym_pl[2];

/* Current keyboard status */
player_keys_t key_pl[2];

/* Keyboard status for menu buttons */
int key_quit       = 0;
int key_menu_enter = 0;
int key_menu_up    = 0;
int key_menu_down  = 0;
int key_menu_left  = 0;
int key_menu_right = 0;




void HandleKeyEvent( SDL_KeyboardEvent *key )
{
    if( key->type == SDL_KEYDOWN ||
	key->type == SDL_KEYUP )
    {
	int i;
	int b = key->type == SDL_KEYDOWN ? 1 : 0;

	/* Player keys */
	for( i = 0; i < 2; i++ ) {
	    if( key->keysym.sym == sym_pl[i].up )
		key_pl[i].up    = b;
	    else if( key->keysym.sym == sym_pl[i].down )
		key_pl[i].down  = b;
	    else if( key->keysym.sym == sym_pl[i].left )
		key_pl[i].left  = b;
	    else if( key->keysym.sym == sym_pl[i].right )
		key_pl[i].right = b;
	    else if( key->keysym.sym == sym_pl[i].fire )
		key_pl[i].fire  = b;
	}

	/* Menu keys */
	if( key->keysym.sym == SDLK_ESCAPE )
	    key_quit        = b;
	else if( key->keysym.sym == SDLK_UP )
	    key_menu_up     = b;
	else if( key->keysym.sym == SDLK_DOWN )
	    key_menu_down   = b;
	else if( key->keysym.sym == SDLK_LEFT )
	    key_menu_left   = b;
	else if( key->keysym.sym == SDLK_RIGHT )
	    key_menu_right  = b;
	else if( key->keysym.sym == SDLK_RETURN )
	    key_menu_enter  = b;
    }
}



