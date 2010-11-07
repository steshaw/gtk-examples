/*
 * Frame.c
 *
 */

#include "gtk/gtk.h"
#include <stdlib.h>
#include <time.h>

#define max(x,y) (((x)>(y))?(x):(y))
#define min(x,y) (((x)<(y))?(x):(y))


/*
 * CloseAppWindow
 *
 * The window is closing down, end the gtk loop
 */
gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();

    return (FALSE);
}



/*
 * main
 *
 * Program begins here
 */
int main (int argc, char *argv[])
{
    GtkWidget *window;
    GdkBitmap *mask;
    GtkStyle *style;
    GtkWidget *table;          
    GtkWidget *frame;          

    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);

    /* --- Create the top window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- Give the window a title. --- */
    gtk_window_set_title (GTK_WINDOW (window), "Frame example");

    /* --- Set the window size. --- */
    gtk_widget_set_usize (window, 250, 250);

    /* --- You should always remember to connect the delete_event
     *     to the main window.
     */
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
                        GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    /* --- Create a 10x10 table for the buttons --- */
    table = gtk_table_new (2, 2, TRUE); 

    /*
     * --- Create the start button with smilely face on it
     */
    frame = gtk_frame_new ("In - right");
    gtk_widget_show (frame);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
    gtk_frame_set_label_align (GTK_FRAME (frame), .5, 0);

    /* --- Put the button in the table in the right place. --- */
    gtk_table_attach (GTK_TABLE (table), frame, 
                      0, 1, 
                      0, 1, 
                      GTK_FILL | GTK_EXPAND, 
                      GTK_FILL | GTK_EXPAND, 
                      7, 0);

    /*
     * --- Create the start button with smilely face on it
     */
    frame = gtk_frame_new ("out - left");
    gtk_widget_show (frame);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_OUT);
    gtk_frame_set_label_align (GTK_FRAME (frame), 0, 0);

    /* --- Put the button in the table in the right place. --- */
    gtk_table_attach (GTK_TABLE (table), frame, 
                      1, 2, 
                      0, 1, 
                      GTK_FILL | GTK_EXPAND, 
                      GTK_FILL | GTK_EXPAND, 
                      3, 0);

    /*
     * --- Create the start button with smilely face on it
     */
    frame = gtk_frame_new ("etched out - bottom");
    gtk_widget_show (frame);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_ETCHED_OUT);
    gtk_frame_set_label_align (GTK_FRAME (frame), 0, 1);

    /* --- Put the button in the table in the right place. --- */
    gtk_table_attach (GTK_TABLE (table), frame, 
                      0, 1, 
                      1, 2, 
                      GTK_FILL | GTK_EXPAND, 
                      GTK_FILL | GTK_EXPAND, 
                      3, 0);

    /*
     * --- Create the start button with smilely face on it
     */
    frame = gtk_frame_new ("etched in - bottom");
    gtk_widget_show (frame);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_NONE);
    gtk_frame_set_label_align (GTK_FRAME (frame), 0, 1);

    /* --- Put the button in the table in the right place. --- */
    gtk_table_attach (GTK_TABLE (table), frame, 
                      1, 2, 
                      1, 2, 
                      GTK_FILL | GTK_EXPAND, 
                      GTK_FILL | GTK_EXPAND, 
                      3, 0);

    /*
     * --- Bombs left on the page is a label
     */ 

    /* --- Make them visible --- */
    gtk_container_add (GTK_CONTAINER (window), table);


    gtk_widget_show (table);
    gtk_widget_show (window);

    gtk_main ();
    exit (0);
}



