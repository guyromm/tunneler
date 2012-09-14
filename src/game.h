/* game.h
 * Global game definitions of tunneler
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
 

#ifndef TUNNELER_GAME_H
#define TUNNELER_GAME_H



#ifndef WIN32
#define CONF_FILE      ".tunnelerrc"
#else
#define CONF_FILE      "tunneler.ini"
#endif

/* Gameworld related configuration */

#define FIELD_SIZEX    800
#define FIELD_SIZEY    600

#define TANK_SPEED     30.0
#define FIRE_DELAY     150
#define AMMO_SPEED     60.0
#define PART_SPEED     60.0
#define DIG_SPEED      10.0
#define BASE_SIZEX     16
#define BASE_SIZEY     19
#define BASE_DOORSIZE  5

#define SHOT_DAMAGE    0.1
#define REPAIR_SPEED1  0.125
#define REPAIR_SPEED2  0.0625
#define ENERGY_DROP    0.003
#define ENERGY_SHOT    0.008


#define TANK_SPRITE { { 0, 0, 0, 0, 0, 0, 0 }, { 2, 2, 2, 2, 2, 2, 0 }, \
	{ 0, 1, 1, 1, 1, 0, 0 }, { 0, 1, 1, 3, 3, 3, 3 }, { 0, 1, 1, 1, 1, 0, 0 }, \
	{ 2, 2, 2, 2, 2, 2, 0 }, { 0, 0, 0, 0, 0, 0, 0 } }, \
	{ { 0, 0, 0, 2, 0, 0, 0 }, { 0, 0, 0, 1, 2, 0, 0 }, { 0, 0, 1, 1, 1, 2, 0 }, \
	{ 2, 1, 1, 3, 1, 1, 2 }, { 0, 2, 1, 1, 3, 0, 0 }, { 0, 0, 2, 1, 0, 3, 0 }, \
	{ 0, 0, 0, 2, 0, 0, 0 } }


#endif /* End of file game.h */
