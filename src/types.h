/* types.h
 * Type definitions for tunneler
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


#ifndef TUNNELER_TYPES_H
#define TUNNELER_TYPES_H


/* Tank modes */
#define TANK_NORMAL 0
#define TANK_AI 1


typedef struct
{
    int up;
    int down;
    int left;
    int right;
    int fire;
} player_keys_t;


struct TANK
{
    int    mode;

    int    rot;
    int    oldrot;
    int    tunneling;
    double x, y;
    int    move;
    int    fire;
    int    basex, basey;
    double Energy;
    double Shields;
    double deathc;
    int    deaths;
    unsigned long last;
};


struct AMMO
{
    char   exists;
    int    rot;
    double x, y;
};


struct EXPL
{
    double lifetime;
    double x, y;
    double vx, vy;
};


#endif /* End of file types.h */
