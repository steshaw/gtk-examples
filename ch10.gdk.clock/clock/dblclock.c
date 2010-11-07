/*
 * Auth: Eric Harlow
 * File: dblclock.c
 * Linux application development
 *
 * Double buffered clock (no flickering)
 */

#include <math.h>
#include <gtk/gtk.h>
#include <time.h>

/* --- Backing pixmap for drawing area  --- */
static GdkPixmap *pixmap = NULL;

int radius;


/*
 * DrawTickAt
 * 
 * Draw a tick mark on the clock's face.
 * pixmap - drawing area
 * gc - pen
 * nHour - 1-12, what time to draw the tick mark
 * cx - width of clock
 * cy - height of clock
 */
void DrawTickAt (GdkDrawable *pixmap, GdkGC *gc, int nHour, int cx, int cy)
{
    /* --- Convert time to radians. --- */
    double dRadians = nHour * 3.14 / 6.0;

    /* --- Draw line from .95 * rad to 1.0 * rad --- */
    gdk_draw_line (pixmap, gc,
                   cx+(int) ((0.95 * radius * sin (dRadians))),
                   cy-(int) ((0.95 * radius * cos (dRadians))),
                   cx+(int) ((1.0 * radius * sin (dRadians))),
                   cy-(int) ((1.0 * radius * cos (dRadians))));
}


/*
 * Repaint
 *
 * data - widget to repaint
 */
gint Repaint (gpointer data)
{
    GtkWidget*    drawing_area = (GtkWidget *) data;
    GdkRectangle  update_rect;
    int midx, midy;
    int    nHour;
    float dRadians;
    time_t now;
    struct tm *now_tm;

    /* --- clear pixmap (background image) --- */
    gdk_draw_rectangle (pixmap,
              drawing_area->style->white_gc,
              TRUE,
              0, 0,
              drawing_area->allocation.width,
              drawing_area->allocation.height);

    /* --- Calculate midpoint of clock --- */
    midx = drawing_area->allocation.width / 2;
    midy = drawing_area->allocation.height / 2;

    /* --- Calculate radius --- */
    radius = MIN (midx, midy);

    /* --- Draw Circle --- */
    gdk_draw_arc (pixmap, 
                  drawing_area->style->black_gc,
                  0,
                  0, 0, midx + midx, midy + midy, 0, 360 * 64);

    /* --- Draw Tickmarks --- */
    for (nHour = 1; nHour <= 12; nHour++) {

        DrawTickAt (pixmap, drawing_area->style->black_gc, 
                    nHour, midx, midy);
    }

    /* --- Get Time --- */
    time (&now);
    now_tm = localtime (&now);

    /*
     * --- Draw the second hand
     */
    
    /* --- Get radians from seconds --- */
    dRadians = now_tm->tm_sec * 3.14 / 30.0;
    
    /* --- Draw seconds --- */
    gdk_draw_line (pixmap, drawing_area->style->black_gc,
                   midx, midy,
                   midx + (0.9 * radius * sin (dRadians)),
                   midy - (0.9 * radius * cos (dRadians)));

    /* 
     * --- Draw the minute 
     */

    /* --- Get radians from minutes and seconds --- */
    dRadians = (now_tm->tm_min * 3.14 / 30.0) +
               (3.14 * now_tm->tm_sec / 1800.0);

    /* --- Draw Minutes --- */
    gdk_draw_line (pixmap, drawing_area->style->black_gc,
                   midx, midy,
                   midx+(int) (0.7 * radius * sin (dRadians)),
                   midy-(int) (0.7 * radius * cos (dRadians)));

    /*
     * --- Draw the hour hand
     */

    /* --- Calculate hours to the time in radians --- */
    dRadians = (now_tm->tm_hour % 12) * 3.14 / 6.0 + 
               (3.14 * now_tm->tm_min / 360.0);

    /* --- Draw the hour hand --- */
    gdk_draw_line (pixmap, drawing_area->style->black_gc,
                   midx, midy,
                   midx + (int) (radius * 0.5 * sin (dRadians)),
                   midy - (int) (radius * 0.5 * cos (dRadians)));

    /* --- Set the rect up - 
     *     The entire pixmap is going to be copied
     *     onto the window so the rect is configured 
     *     as the size of the window.
     */
 
    update_rect.x = 0;
    update_rect.y = 0;
    update_rect.width = drawing_area->allocation.width;
    update_rect.height = drawing_area->allocation.height;

    /* --- And then draw it (calls expose event) --- */
    gtk_widget_draw (drawing_area, &update_rect);

    return (TRUE);
}

/* 
 * configure_event
 *
 * Create a new backing pixmap of the appropriate size 
 * Of course, this is called whenever the window is 
 * resized.  We have to free up things we allocated.
 */
static gint configure_event (GtkWidget *widget, GdkEventConfigure *event)
{
    printf ("configure_event\n");
    /* --- Free background if we created it --- */
    if (pixmap) {
        gdk_pixmap_unref (pixmap);
    } 

    /* --- Create a new pixmap with new size --- */
    pixmap = gdk_pixmap_new (widget->window,
                widget->allocation.width,
                widget->allocation.height,
                -1);

    return TRUE;
}

/*
 * expose_event
 *
 * When the window is exposed to the viewer or 
 * the gdk_widget_draw routine is called, this 
 * routine is called.  Copies the background pixmap
 * to the window.
 */
gint expose_event (GtkWidget *widget, GdkEventExpose *event)
{
    printf ("expose_event\n");

    /* --- Copy pixmap to the window --- */
    gdk_draw_pixmap (widget->window,
          widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
          pixmap,
          event->area.x, event->area.y,
          event->area.x, event->area.y,
          event->area.width, event->area.height);

    return FALSE;
}


/*
 * quit
 *
 * Get out of the application
 */
void quit ()
{
    gtk_exit (0);
}


/*
 * main
 *
 * Program begins here.
 */ 
int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *drawing_area;
    GtkWidget *vbox;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    vbox = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);

    gtk_signal_connect (GTK_OBJECT (window), "destroy",
                GTK_SIGNAL_FUNC (quit), NULL);

    /* --- Create the drawing area  --- */
    drawing_area = gtk_drawing_area_new ();
    gtk_drawing_area_size (GTK_DRAWING_AREA (drawing_area), 200, 200);
    gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);

    gtk_widget_show (drawing_area);

    /* --- Signals used to handle backing pixmap --- */
    gtk_signal_connect (GTK_OBJECT (drawing_area), "expose_event",
                (GtkSignalFunc) expose_event, NULL);
    gtk_signal_connect (GTK_OBJECT(drawing_area),"configure_event",
                (GtkSignalFunc) configure_event, NULL);

    /* --- Show the window --- */
    gtk_widget_show (window);

    /* --- Repaint every second --- */
    gtk_timeout_add (1000, Repaint, drawing_area);

    /* --- Call gtk-main loop --- */
    gtk_main ();

    return 0;
}



