/* 
 * File: button.c
 * Auth: Eric Harlow
 *
 * Example showing button behavior
 */

#include <gtk/gtk.h>

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
 * button_event
 *
 * Some event happened and the name is passed in the
 * data field.
 */
void button_event (GtkWidget *widget, gpointer *data)
{
    g_print ("Button event: %s\n", data);
}



/*
 * main - program begins here.
 */
int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *button;
    
    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);
    
    /* --- Create the top level window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title (GTK_WINDOW (window), "Push Button");

    /* --- You should always remember to connect the delete_event
     *     to the main window.
     */
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    /* --- Give the window a border --- */
    gtk_container_border_width (GTK_CONTAINER (window), 50);

    /* 
     * --- Create a button
     */

    /* --- Create a new button. --- */
    button = gtk_button_new_with_label ("Button");

    /* --- Show the button  --- */
    gtk_widget_show (button);

    /* 
     * --- Register event handlers
     */
    gtk_signal_connect (GTK_OBJECT (button), "pressed",
			GTK_SIGNAL_FUNC (button_event), "pressed");
    gtk_signal_connect (GTK_OBJECT (button), "released",
			GTK_SIGNAL_FUNC (button_event), "released");

    gtk_signal_connect (GTK_OBJECT (button), "clicked",
			GTK_SIGNAL_FUNC (button_event), "clicked");

    gtk_signal_connect (GTK_OBJECT (button), "enter",
			GTK_SIGNAL_FUNC (button_event), "enter");
    gtk_signal_connect (GTK_OBJECT (button), "leave",
			GTK_SIGNAL_FUNC (button_event), "leave");

    /* 
     * --- Make the main window visible
     */ 
    gtk_container_add (GTK_CONTAINER (window), button);
    gtk_widget_show (window);

    gtk_main ();
    exit (0);
}

