/*
 * Auth: Eric Harlow
 * File: animate.c
 *
 * Linux application development
 *
 * Defender game.  
 */

#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "defender.h"

void AddKey (GdkEventKey *event);
void RemoveKey (GdkEventKey *event);
void HandleKeysPressed ();
void StartGame ();
void DrawScreen (GdkPixmap *pixmap, GtkWidget *drawing_area);
int GetGameHeight ();
void LoadImages (GtkWidget *window);

/* --- Backing pixmap for drawing area  --- */
GdkPixmap *pixmap = NULL;


/*
 * DisplaySprite
 *
 * Display the sprite on the drawing_area at the prescribed 
 * coordinates.  Use a mask so that we do not draw the 
 * invisible area - it should be transparent.
 *
 * drawing_area - where to draw the sprite
 * sprite - the sprite to draw
 * x, y - position to draw the sprite.
 */
void DisplaySprite (GtkWidget *drawing_area, typSprite *sprite, int x, int y)
{
    GdkGC *gc;

    /* --- Get gc of a the normal style --- */
    gc = drawing_area->style->fg_gc[GTK_STATE_NORMAL];

    /* --- Set up the clip mask and origin --- */
    gdk_gc_set_clip_mask (gc, sprite->mask);
    gdk_gc_set_clip_origin (gc, x, y);

    /* --- Copy pixmap to the window --- */
    gdk_draw_pixmap (pixmap,
          drawing_area->style->fg_gc[GTK_STATE_NORMAL],
          sprite->pixmap,
          0, 0, x, y,
          sprite->width, sprite->height);

    /* --- Clear the clip mask when done. --- */
    gdk_gc_set_clip_mask (gc, NULL);
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

    /* --- Draw the game on the background. --- */
    DrawScreen (pixmap, drawing_area);

    /* --- Copy background to screen --- */
    update_rect.x = 0;
    update_rect.y = 0;
    update_rect.width = drawing_area->allocation.width;
    update_rect.height = drawing_area->allocation.height;

    /* --- Copy the background to the foreground --- */
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
 * GotFocus
 *
 * Called when the window gets the focus.  This is necessary
 * because the keys repeat themeselves and block out other 
 * keys when pressed at the same time.  The only way around
 * this is to disable that behavior and handle the key "up"
 * and "down" ourselves.  Of course, this change is global 
 * across all applications so we should only set it when we
 * have the focus.
 */
static gint GotFocus (GtkWidget *widget, gpointer data)
{
    gdk_key_repeat_disable ();
    return (FALSE);
}

/*
 * LostFocus
 *
 * See GotFocus.  Since the change is global, we should
 * restore the repeat so that other applications aren't
 * messed up.
 */
static gint LostFocus (GtkWidget *widget, gpointer data)
{
    gdk_key_repeat_restore ();
    return (FALSE);
}


/*
 * KeyPress
 *
 * Hey, a keypress.  Add the key to the list of 
 * keys currently being pressed.
 */
static gint KeyPress (GtkWidget *widget, GdkEventKey *event)
{

    AddKey (event);
    return (FALSE);
}

/*
 * KeyRelease
 *
 * Hey, they let go of a keypress.  Remove the 
 * key from the list of keys that is currently
 * being pressed.
 */
static gint KeyRelease (GtkWidget *widget, GdkEventKey *event)
{
    RemoveKey (event);
    return (FALSE);
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
    gc = gdk_gc_new (pixmap);

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

    StartGame ();

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- Don't allow window to be resized.  --- */
    gtk_window_set_policy (GTK_WINDOW (window), FALSE, FALSE, TRUE);

    vbox = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);

    gtk_signal_connect (GTK_OBJECT (window), "destroy",
                GTK_SIGNAL_FUNC (quit), NULL);

    /* --- Create the drawing area  --- */
    drawing_area = gtk_drawing_area_new ();
    gtk_drawing_area_size (GTK_DRAWING_AREA (drawing_area), 400, GetGameHeight ());
    gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);

    gtk_widget_show (drawing_area);

    gtk_widget_set_events (window, GDK_KEY_RELEASE_MASK);

    /* --- Signals used to handle backing pixmap --- */
    gtk_signal_connect (GTK_OBJECT (drawing_area), "expose_event",
                (GtkSignalFunc) expose_event, NULL);
    gtk_signal_connect (GTK_OBJECT (drawing_area), "configure_event",
                (GtkSignalFunc) configure_event, NULL);

    /* --- Focus signals --- */
    gtk_signal_connect (GTK_OBJECT (window), "focus_in_event",
                (GtkSignalFunc) GotFocus, NULL);
    gtk_signal_connect (GTK_OBJECT (window), "focus_out_event",
                (GtkSignalFunc) LostFocus, NULL);

    /* --- Key press signals --- */
    gtk_signal_connect (GTK_OBJECT (window), "key_press_event",
                (GtkSignalFunc) KeyPress, NULL);
    gtk_signal_connect (GTK_OBJECT (window), "key_release_event",
                (GtkSignalFunc) KeyRelease, NULL);

    /* --- Show the window --- */
    gtk_widget_show (window);

    /* --- Repaint every second --- */
    gtk_timeout_add (100, Repaint, drawing_area);

    LoadImages (window);

    /* --- Call gtk-main loop --- */
    gtk_main ();

    return 0;
}



/*
 * GetWidthHeight
 *
 * Get the height and width of the pixmap from the xpm data.
 */
void GetWidthHeight (gchar **xpm, int *width, int *height)
{
	
	sscanf (xpm [0], "%d %d", width, height);
}
