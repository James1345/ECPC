/*
 * win.h
 * 
 * Copyright 2012 James McMahon <james@myhost>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 
#ifndef _ECPC_WIN_
#define _ECPC_WIN_

#include <xcb/xcb.h> /* For obvious reasons */
#include <xcb/xcb_atom.h> /* Atoms needed for window operations */

#ifdef __cplusplus
extern "C" {
#endif

/* A window */
typedef xcb_drawable_t window;

/* 
 * A graphics context 
 * 
 * A graphics context is a collection of settings (e.g. line color), used to draw on a window.
 */
typedef xcb_gcontext_t graphics;

/*
 * A single point
 */
typedef xcb_point_t point;

/*
 * Initialise ecpc
 */
void ecpc_init(void);

/*
 * Create a new window.
 */
window window_new(const char * name, int16_t x, int16_t y, uint16_t width, uint16_t height);

/*
 * set window visibility
 */
void window_set_visible(window, int); 

/*
 * Create a graphics context, using the same mask and value settings as xcb.
 * TODO - clean this up a bit - currently uses some xcb values.
 */
graphics graphics_new(uint32_t, uint32_t[]);

/*
 *Draw all of the given points
 */
void draw_points(window w, graphics g, point *points);

/*
 * Draw a line between the given points
 */
void draw_line(window w, graphics g, point from, point to);

#ifdef __cplusplus
}
#endif

#endif /* _ECPC_WIN_ */
