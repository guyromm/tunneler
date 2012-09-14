/* keys.h
 * Keyboard function headers
 * Last modified 26 Jun 2004
 *
 * Copyright (c) 2004 Taneli Kalvas
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
 

#ifndef TUNNELER_KEYS_H
#define TUNNELER_KEYS_H


#include <SDL/SDL.h>
#include "types.h"


/* Keyboard definitions */
extern player_keys_t sym_pl[2];

/* Current keyboard status */
extern player_keys_t key_pl[2];

/* Keyboard status for menu buttons */
extern int key_quit;
extern int key_menu_enter;
extern int key_menu_up;
extern int key_menu_down;
extern int key_menu_left;
extern int key_menu_right;



void HandleKeyEvent( SDL_KeyboardEvent *key );


#endif /* End of file keys.h */
