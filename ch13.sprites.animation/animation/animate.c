/*
 * Auth: Eric Harlow
 * Linux application development
 *
 * Demo of sprites.
 */

#include <gtk/gtk.h>
#include <time.h>

#include "man.h"
#include "woman.h"
#include "ball.h"
#include "driver.h"
#include "police.h"
#include "bike.h"


#define LINE_LEN 20
#define GAP_LEN 15

/*
 * Our sprite data structure
 */
typedef struct {

    char **xpm_data;
    GdkPixmap *pixmap;
    GdkBitmap *mask;

} typSprite;

/*
 * Here's the actor.  Actor consists of one or more 
 * sprites. 
 */
typedef struct {

    int seq;
    int x;
    int y;
    int height;
    int width;
    int nSequences;
    typSprite *sprites;

} typActor;

/*
 * Man walking 
 */
typSprite walk[] =  {
    { xpm_standright, NULL, NULL }, 
    { xpm_walkright1, NULL, NULL },
    { xpm_standright, NULL, NULL }, 
    { xpm_walkright2, NULL, NULL },
    { NULL, NULL, NULL }
};

/*
 * Man on a bike
 */
typSprite sprite_bike[] = {
    { xpm_bike1, NULL, NULL }, 
    { xpm_bike2, NULL, NULL }, 
    { xpm_bike3, NULL, NULL }, 
    { NULL, NULL, NULL }
};


/*
 * Woman walking
 */
typSprite sprite_woman[] =  {
    { xpm_womanr, NULL, NULL }, 
    { xpm_womanwalkr1, NULL, NULL },
    { xpm_womanr, NULL, NULL }, 
    { xpm_womanwalkr2, NULL, NULL },
    { NULL, NULL, NULL }
};


/*
 * Police car
 */
typSprite sprite_police[] = {
    { xpm_police1, NULL, NULL },
    { xpm_police2, NULL, NULL },
    { NULL, NULL, NULL }
};


/*
 * Partially transparent ball.
 */
typSprite sprite_ball[] = {
    { xpm_ball1, NULL, NULL },
    { NULL, NULL, NULL }
};

/*
 * Car
 */
typSprite sprite_car[] = {
    { xpm_car1, NULL, NULL },
    { xpm_car1, NULL, NULL },
    { NULL, NULL, NULL }
};


/*
 * Here are the stars of the production
 */
typActor man;
typActor bike;
typActor woman;
typActor police;
typActor ball1;
typActor car1;

/* --- Backing pixmap for drawing area --- */
static GdkPixmap *pixmap = NULL;

/* --- Flag to use mask --- */
static int bUseMask = TRUE;

/*
 * Prototypes.
 */
void GetWidthHeight (gchar **xpm, int *width, int *height);


/*
 * LoadPixmaps
 *
 * Load the actor's pixmaps, get the sprite information
 * from the xpm data and initialize the actor animation
 * information.
 */
void LoadPixmaps (GtkWidget *widget, typActor *actor, typSprite *sprites)
{

    int i = 0;

    /* --- Get every sequence --- */
    while (sprites[i].xpm_data) {

        /* --- Convert xpm data to pixmap & mask --- */
        sprites[i].pixmap = gdk_pixmap_create_from_xpm_d (
                               widget->window,
                               &sprites[i].mask,
                               NULL,
                               sprites[i].xpm_data);
        i++;
    }

    /* --- Get sprite height and width --- */
    GetWidthHeight (sprites[0].xpm_data, &actor->width, &actor->height);

    /* --- Initialize sprite information --- */
    actor->seq = 0;
    actor->nSequences = i;
    actor->sprites = sprites;
}


/*
 * SequenceSprite
 *
 * Move to the next sprite in the sequence and
 * draw it with the mask
 */
SequenceSprite (GtkWidget *drawing_area, typActor *actor, int nDist)
{
    GdkGC *gc;

    actor->x += nDist;
    if (actor->x > drawing_area->allocation.width) {
        actor->x = 0;
    }

    /* --- Use the next image for the actor --- */
    actor->seq++;

    /* --- Use 0 if at the end --- */
    if (actor->seq >= actor->nSequences) {
        actor->seq = 0;
    }

    /* --- Get foreground color --- */
    gc = drawing_area->style->fg_gc[GTK_STATE_NORMAL];

    if (bUseMask) {

        /* --- Set the clipping of the sprites --- */
        gdk_gc_set_clip_mask (gc, actor->sprites[actor->seq].mask);

        /* --- Set the origin of the clipping --- */
        gdk_gc_set_clip_origin (gc, actor->x, actor->y);
    }

    /* --- Copy pixmap to the window, properly clipped --- */
    gdk_draw_pixmap (pixmap,
          drawing_area->style->fg_gc[GTK_STATE_NORMAL],
          actor->sprites[actor->seq].pixmap,
          0, 0,
          actor->x, actor->y,
          actor->width, actor->height);

    if (bUseMask) {

        /* --- Clear the clipping mask --- */
        gdk_gc_set_clip_mask (gc, NULL);
    }
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
    static offset = 0;
    int nTop;

    /* --- clear pixmap (background image) --- */
    gdk_draw_rectangle (pixmap,
              drawing_area->style->black_gc,
              TRUE,
              0, 0,
              drawing_area->allocation.width,
              drawing_area->allocation.height);

    /* --- Draw road --- */
    gdk_draw_rectangle (pixmap, 
              drawing_area->style->white_gc,
              TRUE,
              50, 0,
              100,
              drawing_area->allocation.height);

    /* 
     * Draw lines on the road
     */

    /* --- Figure out where first line should be. --- */
    offset++;
    if ((offset - GAP_LEN) >= 0) {
        offset -= (LINE_LEN + GAP_LEN);
    }

    /* --- Draw the lines all the way down the road --- */
    nTop = offset;
    do {
        gdk_draw_rectangle (pixmap,
              drawing_area->style->black_gc,
              TRUE,
              100, nTop, 5, LINE_LEN);
        nTop += LINE_LEN + GAP_LEN;
    } while (nTop < drawing_area->allocation.height);

    /* --- Draw each of these images --- */
    SequenceSprite (drawing_area, &bike, 3);
    SequenceSprite (drawing_area, &man, 2);
    SequenceSprite (drawing_area, &woman, 2);
    SequenceSprite (drawing_area, &police, 0);
    SequenceSprite (drawing_area, &ball1, 2);
    SequenceSprite (drawing_area, &car1, 3);

    /* --- The whole screen needs to be updated --- */
    update_rect.x = 0;
    update_rect.y = 0;
    update_rect.width = drawing_area->allocation.width;
    update_rect.height = drawing_area->allocation.height;

    /* --- So update it --- */
    gtk_widget_draw (drawing_area, &update_rect);
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
 * quit
 *
 * Get out of the application
 */
void quit ()
{
    gtk_exit (0);
}


/*
 * GetWidgthHeight
 *
 * Get the widget and height for the xpm data
 *
 */
void GetWidthHeight (gchar **xpm, int *width, int *height)
{
	
	sscanf (xpm [0], "%d %d", width, height);
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

    if (argc > 1) {
        bUseMask = FALSE;
    }

    /* --- Create the top level window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    vbox = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);

    gtk_signal_connect (GTK_OBJECT (window), "destroy",
                GTK_SIGNAL_FUNC (quit), NULL);

    /* --- Create the drawing area  --- */
    drawing_area = gtk_drawing_area_new ();
    gtk_drawing_area_size (GTK_DRAWING_AREA (drawing_area), 200, 300);
    gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);

    gtk_widget_show (drawing_area);

    /* --- Signals used to handle backing pixmap --- */
    gtk_signal_connect (GTK_OBJECT (drawing_area), "expose_event",
                (GtkSignalFunc) expose_event, NULL);
    gtk_signal_connect (GTK_OBJECT(drawing_area),"configure_event",
                (GtkSignalFunc) configure_event, NULL);

    /* --- Show the window --- */
    gtk_widget_show (window);

    /* --- Repaint every so often --- */
    gtk_timeout_add (100, Repaint, drawing_area);

    /* --- Load all these sprites --- */
    LoadPixmaps (window, &man, walk);
    LoadPixmaps (window, &bike, sprite_bike);
    LoadPixmaps (window, &woman, sprite_woman);
    LoadPixmaps (window, &police, sprite_police);
    LoadPixmaps (window, &ball1, sprite_ball);
    LoadPixmaps (window, &car1, sprite_car);

    /* --- Position them --- */
    bike.x = 30;
    bike.y = 60;

    man.x = 50;
    man.y = 60;

    man.x = 60;
    man.y = 60;

    police.x = 60;
    police.y = 90;

    ball1.x = 0;
    ball1.y = 90;

    car1.x = 0;
    car1.y = 120;

    /* --- Call gtk-main loop --- */
    gtk_main ();

    return 0;
}



