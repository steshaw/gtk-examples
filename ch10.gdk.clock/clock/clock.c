/*
 * Auth: Eric Harlow
 * Linux Application Development
 *
 *
 */

#include <math.h>
#include <gtk/gtk.h>
#include <time.h>

int radius;

/*
 * DrawTickAt
 * 
 * Draw a tick mark on the clock's face.  The tick marks
 * are drawn at the hour marks to help people read the 
 * time on the clock.
 *
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
    GtkWidget*	drawing_area = (GtkWidget *) data;
    int midx, midy;
    int	nHour;
    float dRadians;
    time_t now;
    struct tm *now_tm;
    GdkDrawable *drawable;

    /* --- Get drawing area window --- */
    drawable = drawing_area->window;

    /* --- Clear Rectangle --- */
    gdk_draw_rectangle (drawable,
		      drawing_area->style->white_gc,
		      TRUE,
		      0, 0,
		      drawing_area->allocation.width,
		      drawing_area->allocation.height);

    /* --- Get the midpoint --- */
    midx = drawing_area->allocation.width / 2;
    midy = drawing_area->allocation.height / 2;

    /* --- Find smaller value --- */
    radius = MIN (midx, midy);

    /* --- Draw clock face (circle) --- */
    gdk_draw_arc (drawable, 
                  drawing_area->style->black_gc,
                  0,
                  0, 0, midx + midx, midy + midy, 0, 360 * 64);

    /* --- Draw Tickmarks at each hour --- */
    for (nHour = 1; nHour <= 12; nHour++) {

	DrawTickAt (drawable, 
                    drawing_area->style->black_gc, 
                    nHour, midx, midy);
    }

    /* --- Get Time --- */
    time (&now);

    /* --- Convert time --- */
    now_tm = localtime (&now);

    /* 
     * --- Draw the second hand
     */

    /* --- Get radians from seconds --- */
    dRadians = now_tm->tm_sec * 3.14 / 30.0;
    
    /* --- Draw seconds --- */
    gdk_draw_line (drawable, drawing_area->style->black_gc,
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
    gdk_draw_line (drawable, drawing_area->style->black_gc,
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
    gdk_draw_line (drawable, drawing_area->style->black_gc,
                   midx, midy,
                   midx + (int) (radius * 0.5 * sin (dRadians)),
                   midy - (int) (radius * 0.5 * cos (dRadians)));

    return (TRUE);
}

/*
 * quit
 *
 * exit gtk message loop
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

    /* --- Initialize GTK --- */
    gtk_init (&argc, &argv);

    /* --- Create a top level window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- Get a packing box --- */
    vbox = gtk_hbox_new (FALSE, 0);

    /* --- Add packing box to window --- */
    gtk_container_add (GTK_CONTAINER (window), vbox);

    /* --- Make packing box visible --- */
    gtk_widget_show (vbox);

    /* --- Listen for the destroy --- */
    gtk_signal_connect (GTK_OBJECT (window), "destroy",
		        GTK_SIGNAL_FUNC (quit), NULL);

    /* --- Create the drawing area  --- */
    drawing_area = gtk_drawing_area_new ();

    /* --- Set the size --- */
    gtk_drawing_area_size (GTK_DRAWING_AREA (drawing_area), 200, 200);

    /* --- Add drawing area to packing box --- */
    gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);

    /* --- Make drawing area visible --- */
    gtk_widget_show (drawing_area);

    /* --- Make the window visible --- */
    gtk_widget_show (window);

    /* --- Repaint every 1000ms (every second) --- */
    gtk_timeout_add (1000, Repaint, (gpointer) drawing_area);

    /* --- Start gtk message loop --- */
    gtk_main ();

    return 0;
}



