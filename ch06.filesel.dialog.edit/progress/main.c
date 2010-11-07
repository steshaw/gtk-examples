/*
 * Application code begins here.
 */

#include <gtk/gtk.h>

int ptimer;
int nValue;

/*
 * UpdateProgressTimer 
 *
 * This is the timer callback function.   It updates the 
 * progressbar and closes it when it reaches the end.
 */
UpdateProgressTimer (gpointer data)
{

    /* --- Increment value --- */
    nValue += 1;

    /* --- Update the progress bar --- */
    UpdateProgress (nValue, 100);

    /* --- Close if it's at the end. --- */
    if (nValue == 100) {
        EndProgress ();
        gtk_timeout_remove (ptimer);
    }
}

/*
 * ButtonClicked
 *
 * Called to create the progress bar and the time when
 * the button has been clicked.
 */
gint ButtonClicked (GtkWidget *widget, gpointer data)
{
    /* --- Do some initialization --- */
    nValue = 0;
    StartProgress ();

    /* --- Call the timer. --- */
    ptimer = gtk_timeout_add (100, UpdateProgressTimer, data);
}


/*
 * CloseAppWindow
 *
 * Close down the application 
 */
gint CloseAppWindow ()
{
    gtk_main_quit ();
    return (FALSE);
}

/*
 * main
 *
 * program begins here.
 */
main (int argc, char *argv[]) 
{
    GtkWidget *window;
    GtkWidget *button;

    gtk_init (&argc, &argv);

    /* --- Create main window and give it a title --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Progress bar");

    /* --- End app when close window detected --- */
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
                        GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    /* --- Create a button to bring up the progress bar --- */
    button = gtk_button_new_with_label ("Progress bar");
    gtk_widget_show (button);

    /* --- This is where we handle it --- */
    gtk_signal_connect (GTK_OBJECT (button), "clicked", 
                        GTK_SIGNAL_FUNC (ButtonClicked), NULL);

    gtk_container_add (GTK_CONTAINER (window), button);
    gtk_widget_show (window);

    /* --- Let GTK take over --- */
    gtk_main ();
    exit (0);
}

