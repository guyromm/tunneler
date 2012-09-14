/* timer.c
 * Timer functions
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


#include <SDL/SDL.h>


Uint32 now, old;


Uint32 Time_Now( void )
{
    return( now );
}


void Init_Timer( void )
{
    now = SDL_GetTicks();
}


double Timer( void )
{
    old = now;
    now = SDL_GetTicks();
    return( (now - old)/1000.0 );
}



