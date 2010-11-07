/*
 * File: align.c
 * Auth: Eric Harlow
 *
 * Show labels and label alignment
 */

#include <gtk/gtk.h>

void AddListItem (GtkWidget *listbox, char *sText);

/*
 * CloseAppWindow
 *
 * Close down the app.
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
    GList *cbitems = NULL;
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *label;
    GtkWidget *vbox;
    
    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);
    
    /* --- Create the top level window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- You should always remember to connect the delete_event
     *     to the main window.
     */
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    /* --- Give the window a border --- */
    gtk_container_border_width (GTK_CONTAINER (window), 10);
    
    /* 
     * --- We create a vertical box (vbox) to pack the horizontal boxes into. 
     */
    vbox = gtk_hbox_new (FALSE, 0);

    /*
     * --- Create a Label
     */
    
    /* --- create a new label.  --- */
    label = gtk_label_new ("label");
	
    /* --- Align the label to the left side.   --- */
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0);

    /* --- Pack the label into the vertical box (vbox box1).  --- */
    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
	
    /* --- Show the label  --- */
    gtk_widget_show (label);

    /* --- create a new label.  --- */
    label = gtk_label_new ("right");
	
    /* --- Align the label to the left side.   --- */
    gtk_misc_set_alignment (GTK_MISC (label), 1, .5);

    /* --- Pack the label into the vertical box (vbox box1).  --- */
    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
	
    /* --- Show the label  --- */
    gtk_widget_show (label);

    /* --- create a new label.  --- */
    label = gtk_label_new ("center");
	
    /* --- Align the label to the left side.   --- */
    gtk_misc_set_alignment (GTK_MISC (label), .5, 1);

    /* --- Pack the label into the vertical box (vbox box1).  --- */
    gtk_box_pack_start (GTK_BOX (vbox), label, FALSE, FALSE, 0);
	
    /* --- Show the label  --- */
    gtk_widget_show (label);

    /* 
     * --- Create a button
     */

    /* --- Create a new button. --- */
    button = gtk_button_new_with_label ("Button");

    /* --- Pack the button into the vertical box (vbox box1).  --- */
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

    /* --- Show the button  --- */
    gtk_widget_show (button);
	
    /* --- Show the entry  --- */
    gtk_widget_show (vbox);

    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (window);

    gtk_main ();
}

