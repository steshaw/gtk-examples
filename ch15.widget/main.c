/* 
 * File: main.c
 * Auth: Eric Harlow
 *
 * Example showing custom widget
 */

#include <gtk/gtk.h>
#include "gtkgraph.h"

/*
 * CloseAppWindow
 *
 * The window is closing down.  Need to shut down gtk.
 */
gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();

    return (FALSE);
}



/*
 * main - program begins here.
 */
int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *graph;
    
    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);
    
    /* --- Create the top level window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title (GTK_WINDOW (window), "Bar graph");

    /* --- You should always remember to connect the delete_event
           to the main window. --- */
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    /* --- Give the window a border --- */
    gtk_container_border_width (GTK_CONTAINER (window), 20);

    /* 
     * --- Create a graph
     */

    /* --- Create a new graph. --- */
    graph = gtk_graph_new ();

    /* --- Show the graph  --- */
    gtk_widget_show (graph);

    /* --- Set the number of elements in the graph --- */
    gtk_graph_size (GTK_GRAPH (graph), 5);

    /* --- Set the height of each of the graph elements --- */
    gtk_graph_set_value (GTK_GRAPH (graph), 0, 5);
    gtk_graph_set_value (GTK_GRAPH (graph), 1, 10);
    gtk_graph_set_value (GTK_GRAPH (graph), 2, 15);
    gtk_graph_set_value (GTK_GRAPH (graph), 3, 20);
    gtk_graph_set_value (GTK_GRAPH (graph), 4, 25);
    gtk_widget_draw (graph, NULL);

    /* 
     * --- Make the main window visible
     */ 
    gtk_container_add (GTK_CONTAINER (window), graph);
    gtk_widget_show (window);

    gtk_main ();
    exit (0);
}

