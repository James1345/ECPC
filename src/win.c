#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "win.h"

/*
 * Some globals that ecpc will be using
 */
xcb_connection_t *connection;
xcb_screen_t *screen;

void ecpc_init(void)
{
	/* start connection */
	connection = xcb_connect (NULL, NULL);

	/* Get the first screen */
	screen = xcb_setup_roots_iterator(xcb_get_setup (connection)).data;
	xcb_flush(connection);
}

/* window_new currently sets some defaults, these will be set later by modifying the window */
window window_new(const char * name, int16_t x, int16_t y, uint16_t width, uint16_t height)
{
	window w = xcb_generate_id (connection); /* The new window to be returned */
	
	/* Window masks and values... not sure what they actually do yet :P */
	uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	uint32_t values[2] = { screen->white_pixel, XCB_EVENT_MASK_EXPOSURE };

	xcb_create_window (connection,                    /* connection          */
						XCB_COPY_FROM_PARENT,          /* depth               */
						w,                        	  /* window Id           */
						screen->root,                  /* parent window       */
						x, y,                          /* x, y                */
						width, height,                 /* width, height       */
						10,                            /* border_width        */
						XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
						screen->root_visual,           /* visual              */
						mask, values );                /* masks */
	
	/* Set the title */					
	xcb_change_property (connection,
						XCB_PROP_MODE_REPLACE,
						w,
						WM_NAME,
						STRING,
						8,
						strlen (name),
						name );
	xcb_flush(connection);
	return w;
}

void window_set_visible(window w, int bool)
{
	if(bool)
	{
		xcb_map_window (connection, w);
	}
	else
	{
		xcb_unmap_window (connection, w);
	}
	xcb_flush(connection);
}

graphics graphics_new(uint32_t mask, uint32_t value[])
{
	xcb_gcontext_t context = xcb_generate_id(connection);
	xcb_drawable_t drawable = screen->root;
	xcb_create_gc(connection, context, drawable, mask, value);
	return context;
}

void draw_points(window w, graphics g, point *points)
{
	xcb_poly_point(connection, XCB_COORD_MODE_ORIGIN, w, g, sizeof(points), points);
	xcb_flush(connection);
}

void draw_line(window w, graphics g, point from, point to)
{
	point points[2] = { from, to };
	xcb_poly_line(connection, XCB_COORD_MODE_ORIGIN, w, g, 2, points);
	xcb_flush(connection);
}

/*
 * Testing code
 */
#ifndef NDEBUG
int main(void)
{
	/* geometric objects */
	point          points[] = {
		{10, 10},
		{10, 20},
		{20, 10},
		{20, 20}};

	point          polyline[] = {
		{50, 10},
		{55, 30},     /* rest of points are relative */
		{80, 10},
		{90, 20}};

	xcb_segment_t        segments[] = {
		{100, 10, 140, 30},
		{110, 25, 130, 60}};

	xcb_rectangle_t      rectangles[] = {
		{ 10, 50, 40, 20},
		{ 80, 50, 10, 40}};

	xcb_arc_t            arcs[] = {
		{10, 100, 60, 40, 0, 90 << 6},
		{90, 100, 55, 40, 0, 360 << 6}};
	
	/* Create window */
	ecpc_init();
	window w = window_new("Hello World!", 0, 0, 150, 150);
	window_set_visible(w, 1);
	
	
	/* hack together a context */
	uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	uint32_t value[2] = { screen->black_pixel, 0 };
	graphics g = graphics_new(mask, value);
	
	xcb_poly_line (connection, XCB_COORD_MODE_PREVIOUS, w, g, 4, polyline);
	xcb_flush(connection);
	/* draw primitives */

	xcb_generic_event_t *event;
	while ((event = xcb_wait_for_event (connection))) {
		switch (event->response_type & ~0x80) {
		case XCB_EXPOSE:
			/* We draw the points */
			draw_points(w, g, points);

			/* We draw the polygonal line */
			draw_line(w, g, polyline[0], polyline[1]);
			draw_line(w, g, polyline[2], polyline[3]);

			/* We draw the segements */
			xcb_poly_segment (connection, w, g, 2, segments);

			/* draw the rectangles */
			xcb_poly_rectangle (connection, w, g, 2, rectangles);

			/* draw the arcs */
			xcb_poly_arc (connection, w, g, 2, arcs);

			/* flush the request */
			xcb_flush (connection);

			break;
		default: 
			/* Unknown event type, ignore it */
			break;
		}

		free (event);
	}
	
	return(EXIT_SUCCESS);
}
#endif
