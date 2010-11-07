/*
 * --- Editable widget
 */

#include <gtk/gtk.h>


GtkWidget *text;


/*
 * --- Closing down the window
 */
void destroy_window (GtkWidget *widget)
{
    gtk_main_quit ();
}


/*
 * CreateEditable
 *
 */
void CreateEditable (GtkWidget *window, GtkWidget *container)
{

    text = gtk_text_new (NULL, NULL);
    gtk_text_set_editable (GTK_TEXT (text), TRUE);
    gtk_container_add (GTK_CONTAINER (container), text);
    gtk_widget_show (text);

}


/*
 * main
 *
 * Program begins here
 */
int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *main_vbox;
    
    /* --- Start up GTK --- */
    gtk_init(&argc, &argv);
    
    /* --- Create the top level window --- */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title (GTK_WINDOW (window), "editable");
    gtk_container_border_width (GTK_CONTAINER (window), 0);

    /* --- Listen for the destroy of the window --- */
    gtk_signal_connect (GTK_OBJECT(window), "destroy", 
               GTK_SIGNAL_FUNC (destroy_window), 
               NULL);

    /* --- Set the window title and size --- */
    gtk_widget_set_usize(GTK_WIDGET(window), 300, 200);
    
    /* --- Create a new vertical box for storing widgets --- */
    main_vbox = gtk_vbox_new(FALSE, 1);

    /* --- border width is 1 --- */
    gtk_container_border_width(GTK_CONTAINER(main_vbox), 1);

    /* --- Add the vertical box to the main window --- */
    gtk_container_add(GTK_CONTAINER(window), main_vbox);

    /* --- Make the vertical box visible --- */
    gtk_widget_show(main_vbox);
    
    /* --- Show the top level window --- */
    gtk_widget_show (window);

    /* --- Create editable window --- */
    CreateEditable (window, main_vbox);

    /* --- Loop and process messages --- */
    gtk_main();
    
    return(0);
}



