/*
 * packtable.c
 * 
 * Illustration of how to pack tables.
 */

#include <gtk/gtk.h>

/*
 * Destroy
 *
 * Called when the main window is being closed.  Gives us the 
 * opportunity to exit from the call to gtk_main ()
 */
void Destroy (GtkWidget *widget, gpointer *data)
{
    printf ("destroy\n");
}

gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    printf ("delete\n");

    gtk_main_quit ();
    return (FALSE);
}
    
/*
 * CreateTable
 *
 * Creates a top level window with several options.  Since 
 * this is going to be done many times, it's best just made
 * into a function. 
 */ 
CreateTable (char *szTitle, gint xoptions, gint yoptions) 
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *table;

    /* --- Create the top level window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- Give the window a title. --- */
    gtk_window_set_title (GTK_WINDOW (window), szTitle);

    /* --- You should always remember to connect the destroy event
     *     to the main window.  Note: if any top level window is
     *     closed, we'll shut the whole program down.
     */
    gtk_signal_connect (GTK_OBJECT (window), "destroy",
			GTK_SIGNAL_FUNC (Destroy), NULL);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    /* --- Give the window a border --- */
    gtk_container_border_width (GTK_CONTAINER (window), 10);
    
    /* --- Create a 4x4 table --- */
    table = gtk_table_new (4, 4, TRUE);

    /* --- Add a 1x1 button at 0, 0 --- */
    button = gtk_button_new_with_label ("Button at 0,0");
    gtk_table_attach (GTK_TABLE (table), button, 0, 1, 0, 1, 
                      xoptions, yoptions, 0, 0);
    gtk_widget_show (button);

    /* --- Add a 2x2 button at 2-4, 2-4 --- */
    button = gtk_button_new_with_label ("Big square button");
    gtk_table_attach (GTK_TABLE (table), button, 2, 4, 2, 4, 
                      xoptions, yoptions, 0, 0);
    gtk_widget_show (button);

    /* --- Add a 2x1 short wide button --- */
    button = gtk_button_new_with_label ("Short fat button");
    gtk_table_attach (GTK_TABLE (table), button, 0, 2, 3, 4, 
                      xoptions, yoptions, 0, 0);
    gtk_widget_show (button);

    /* --- Add a 1x2 tall skinny button --- */
    button = gtk_button_new_with_label ("Tall skinny button");
    gtk_table_attach (GTK_TABLE (table), button, 3, 4, 0, 2, 
                      xoptions, yoptions, 0, 0);

    /* --- Make the button visible --- */
    gtk_widget_show (button);

    /* --- Add the table to the window --- */
    gtk_container_add (GTK_CONTAINER (window), table);

    /* --- Make the table visible --- */
    gtk_widget_show (table);

    /* --- Make the window visible --- */
    gtk_widget_show (window);
}


/*
 * --- Main.
 *
 * Program begins here.
 *
 * Rather than duplicate code, the CreateTable function is 
 * called.  It does the work for us - all we do is pass in 
 * how we want the window to appear.
 */
int main (int argc, char *argv[])
{
    
    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);

    /* --- No flags set --- */
    CreateTable ("Nothing", 0, 0);

    /* --- Start showing windows with flags. --- */
    CreateTable ("Fill", GTK_FILL, GTK_FILL);

    CreateTable ("Shrink", GTK_SHRINK, GTK_SHRINK);

    CreateTable ("Expand", 
                 GTK_EXPAND, 
                 GTK_EXPAND);

    CreateTable ("Expand | Shrink", 
                 GTK_EXPAND | GTK_SHRINK, 
                 GTK_EXPAND | GTK_SHRINK);

    CreateTable ("Fill | Expand", 
                 GTK_EXPAND | GTK_FILL, 
                 GTK_EXPAND | GTK_FILL);

    CreateTable ("Shrink | Fill", 
                 GTK_SHRINK | GTK_FILL, 
                 GTK_SHRINK | GTK_FILL);

    CreateTable ("Shrink | Fill | Expand", 
                 GTK_EXPAND | GTK_SHRINK | GTK_FILL, 
                 GTK_EXPAND | GTK_SHRINK | GTK_FILL);

    /* --- Start the gtk loop. --- */
    gtk_main ();

    exit (0);
}
