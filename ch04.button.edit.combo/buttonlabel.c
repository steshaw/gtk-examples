/*
 * File: buttonlabel.c
 * Auth: Eric Harlow
 *
 */ 
#include <gtk/gtk.h>

/*
 * CloseAppWindow
 *
 * Close down the application
 */
gint CloseAppWindow (GtkWidget *widget, gpointer gdata)
{
    g_print ("Quitting...\n");
    gtk_main_quit ();

    return (FALSE);
}

/*
 * LabelModify
 * 
 * Modify the button label
 */
void LabelModify (GtkWidget *child, gpointer data)
{
    char      *szText;
    GList     *children;

    /* --- If it's a label widget --- */
    if (GTK_IS_LABEL (child)) {

        /* --- Get the text on the label --- */
        gtk_label_get (GTK_LABEL (child), &szText);

        /* --- Set the text based on the current text --- */
        if (!strcmp (szText, "Windows")) {
            gtk_label_set (GTK_LABEL (child), "Linux");
        } else if (!strcmp (szText, "Linux")) {
            gtk_label_set (GTK_LABEL (child), "AmigaOS");
        } else {
            gtk_label_set (GTK_LABEL (child), "Windows");
        }
    }
}

/*
 * button_was_clicked
 * 
 * event handler called when the button is clicked.
 */
void button_was_clicked (GtkWidget *widget, gpointer gdata)
{
    gtk_container_foreach (GTK_CONTAINER (widget), 
                           (GtkCallback) LabelModify, NULL);
}

/*
 * main
 *
 */
int main (int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *button;

    /* --- Initialize gtk, handle command line parameters --- */
    gtk_init (&argc, &argv);

    /* --- Create a window in gtk - note the window is NOT visible yet. --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event", 
                GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    /* --- Put some breathing room around the objects in the container --- */
    gtk_container_border_width (GTK_CONTAINER (window), 15);

    /* --- Create a button --- */
    button = gtk_button_new_with_label ("Click me!");

    /* --- Give the button an event handler - it'll call the function
     *     button_was_clicked when the button is clicked.
     */
    gtk_signal_connect (GTK_OBJECT (button), 
                            "clicked", 
                            GTK_SIGNAL_FUNC (button_was_clicked), 
                            "clicked");

    /* --- The main windows contains the button.  --- */
    gtk_container_add (GTK_CONTAINER (window), button);

    /* --- Make the button visible --- */
    gtk_widget_show (button);

    /* --- Create a button --- */
    button = gtk_button_new_with_label ("Windows"); 

    /* --- Make the button visible --- */
    gtk_widget_show (button);

    /* --- Now, make the window visible --- */
    gtk_widget_show (window);

    /* --- This is the event loop in gtk.  Do not return until "quit" --- */
    gtk_main ();

    /* --- Exit status code. --- */
    return 0;
}

