/*
 * File: graph.c
 * Auth: Eric Harlow
 *
 *
 */

#include <gtk/gtk.h>
#include "device.h"

void DrawDevice (GtkWidget *drawing_area, char *szName, GdkGC *pen, int bAvg);
int NetworkButtonDown ();
int NetworkAvgButtonDown ();
int CPUButton15Down ();
int CPUButton5Down ();
int CPUButton1Down ();
void Repaint ();

GtkWidget *drawing_area;

typedef struct {

    GdkDrawable *pixmap;
    GdkGC *gc;

} typGraphics;

static typGraphics *g;
static GdkGC *penBlack = NULL;
static GdkGC *penRed = NULL;
static GdkGC *penBlue = NULL;
static GdkGC *penGreen = NULL;
static GdkGC *penGray = NULL;


/*
 * NewGraphics
 *
 * Create a new graphics data element to keep track
 * of the pixmap and the gc.
 */
typGraphics *NewGraphics ()
{
    typGraphics *gfx;

    /* --- Allocate the space --- */
    gfx = (typGraphics *) g_malloc (sizeof (typGraphics));

    /* --- Initialize --- */
    gfx->gc = NULL;
    gfx->pixmap = NULL;

    /* --- Hand back, ready to use. --- */
    return (gfx);
}


/*
 * GetPen
 *
 * Get a pen using the GdkColor passed in.  The "pen"
 * (just a GdkGC) is created and returned ready for 
 * use.
 */
GdkGC *GetPen (GdkColor *c)
{
    GdkGC *gc;

    /* --- Create a gc --- */
    gc = gdk_gc_new (g->pixmap);

    /* --- Set the forground to the color --- */
    gdk_gc_set_foreground (gc, c);

    /* --- Return it. --- */
    return (gc);
}


/*
 * NewColor
 *
 * Create and allocate a GdkColor with the color 
 * specified in the parameter list.
 */ 
GdkColor *NewColor (long red, long green, long blue)
{
    /* --- Get the color --- */
    GdkColor *c = (GdkColor *) g_malloc (sizeof (GdkColor));

    /* --- Fill it in. --- */
    c->red = red;
    c->green = green;
    c->blue = blue;

    gdk_color_alloc (gdk_colormap_get_system (), c);

    return (c);
}


/*
 * UpdateAndRepaint
 *
 * Routine to poll for the latest stats on network
 * traffic and cpu utilization and repaint the 
 * screen with the information
 */
gint UpdateAndRepaint (gpointer data)
{
    /* --- Get information about networking --- */
    GetPacketInfo ();

    /* --- Get information about the CPU --- */
    GetCPUInfo ();

    /* --- Repaint. --- */
    Repaint ();

    return (1);
}


/*
 * Repaint
 *
 * Update the screen with the latest numbers.
 */
void Repaint ()
{
    GdkRectangle     update_rect;

    /* --- clear pixmap so we can draw on it. --- */
    gdk_draw_rectangle (g->pixmap,
              drawing_area->style->white_gc,
              TRUE,
              0, 0,
              drawing_area->allocation.width,
              drawing_area->allocation.height);

    /* --- If they want to see the actual network usage --- */
    if (NetworkButtonDown ()) {
        DrawDevice (drawing_area, "eth0:", penGray, 0);
    }

    /* --- If they want to see the averaged network usage --- */
    if (NetworkAvgButtonDown ()) {
        DrawDevice (drawing_area, "eth0:", penBlack, 1);
    }
    /* --- If they want to see the 15 minute cpu util --- */
    if (CPUButton15Down ()) {
        DrawDevice (drawing_area, "cpu15", penBlue, 0);
    }
    /* --- If they want to see the 5 minute cpu util --- */
    if (CPUButton5Down ()) {
        DrawDevice (drawing_area, "cpu5", penGreen, 0);
    }
    /* --- If they want to see the 1 minute cpu util --- */
    if (CPUButton1Down ()) {
        DrawDevice (drawing_area, "cpu1", penRed, 0);
    }

    /* --- Update the screen with the background pixmap --- */
    update_rect.x = 0;
    update_rect.y = 0;
    update_rect.width = drawing_area->allocation.width;
    update_rect.height = drawing_area->allocation.height;

    gtk_widget_draw (drawing_area, &update_rect);
}


/*
 * DrawDevice
 *
 * Draw a graph with the device's information
 *
 * drawing_area - widget 
 * szName - name of the device being monitored
 * pen - GC with the color information
 * bAvg - Average plot indicator.  True => do some averaging
 */
void DrawDevice (GtkWidget *drawing_area, char *szName, GdkGC *pen, int bAvg)
{
    typDevice  *dev;
    int    prevx = 0;
    int    prevy = 0;
    int    x = 0;
    int    y = 0;
    int    i;
    int    nLast;

    /* --- Look up the device by the name --- */
    dev = LookupDevice (szName);

    /* --- If we found it --- */
    if (dev) {
    
        /* --- If they're averaging --- */
        if (bAvg) {
            nLast = MAX_VALUES-4;
        } else {
            nLast = MAX_VALUES;
        }

        /* --- Go across the widget and plot --- */
        for (i = 0; i < drawing_area->allocation.width && i < nLast; i++) {

            x = i;
            if (dev->nMax != 0) {

                if (bAvg) {

                    y = ((dev->nValues[i] + 
                          dev->nValues[i+1] +
                          dev->nValues[i+2] + 
                          dev->nValues[i+3] + 
                          dev->nValues[i+4]) * 
                         drawing_area->allocation.height) / 
                         (dev->nMax * 5);
                } else {
                    y = (dev->nValues[i] * 
                     drawing_area->allocation.height) / dev->nMax;
                }
                y = drawing_area->allocation.height - y;
            } else {
                y = 1;
            }

            if (i == 0) {
                prevx = x;
                prevy = y;
            }
    
            /* --- Draw a line from previous point to current pt. --- */
            gdk_draw_line (g->pixmap,
                           pen,
                           prevx, prevy,
                           x, y);
    
            /* --- Next previous point is this point. --- */
            prevx = x;
            prevy = y;
        }
    } else {

        /* --- Should never occur. --- */
        printf ("it's NULL (%s)\n", szName);
    }
}

/* 
 * configure_event
 *
 * Called when the drawing area is created and every
 * time it's resized.
 * Create a new backing pixmap of the appropriate size 
 */
static gint configure_event (GtkWidget *widget, GdkEventConfigure *event)
{
    if (g == NULL) {
        g = NewGraphics ();
    }

    /* --- Free the pixmap --- */
    if (g->pixmap) {
        gdk_pixmap_unref (g->pixmap);
    } 

    /* --- Create a new pixmap --- */
    g->pixmap = gdk_pixmap_new (widget->window,
                             widget->allocation.width,
                             widget->allocation.height,
                             -1);

    /* --- if we don't have our pens yet --- */
    if (penBlack == NULL) {

        /* --- Get our colorful pens. --- */
        penBlack = GetPen (NewColor (0, 0, 0));
        penRed = GetPen (NewColor (0xffff, 0, 0));
        penBlue = GetPen (NewColor (0, 0, 0xffff));
        penGreen = GetPen (NewColor (0, 0xffff, 0));
        penGray = GetPen (NewColor (0x9000, 0x9000, 0x9000));
    }

    /* --- Clear it out --- */
    gdk_draw_rectangle (g->pixmap,
                        widget->style->white_gc,
                        TRUE,
                        0, 0,
                        widget->allocation.width,
                        widget->allocation.height);

    return TRUE;
}

/*
 * expose_event
 * 
 * Redraw the screen using the pixmap 
 */
static gint expose_event (GtkWidget *widget, GdkEventExpose *event)
{
  /* --- Copy it right over. --- */
  gdk_draw_pixmap (widget->window,
          widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
          g->pixmap,
          event->area.x, event->area.y,
          event->area.x, event->area.y,
          event->area.width, event->area.height);

  return FALSE;
}


/*
 * quit
 *
 * Quit the application
 */
void quit ()
{
    gtk_exit (0);
}



/*
 * CreateDrawingArea
 * 
 * Create the drawing area widget and return it.
 */
GtkWidget *CreateDrawingArea ()
{
    /* --- Create the drawing area  --- */
    drawing_area = gtk_drawing_area_new ();

    /* --- Give it a nice size --- */
    gtk_drawing_area_size (GTK_DRAWING_AREA (drawing_area), 200, 200);
  
    /* --- Make it visible --- */
    gtk_widget_show (drawing_area);
  
    /* --- We need the expose_event and the configure_event --- */
    gtk_signal_connect (GTK_OBJECT (drawing_area), "expose_event",
                (GtkSignalFunc) expose_event, NULL);
    gtk_signal_connect (GTK_OBJECT(drawing_area),"configure_event",
                (GtkSignalFunc) configure_event, NULL);

    /* --- Call the function every 2 seconds. --- */
    gtk_timeout_add (2000, UpdateAndRepaint, drawing_area);

    /* --- Return widget so it can be placed on the screen. --- */
    return (drawing_area);
}
