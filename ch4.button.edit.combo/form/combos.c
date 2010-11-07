//
// Form.c
//
// Example showing different widgets in use.
//
//

#include "gtk/gtk.h"

//
// Functions we're calling.
//
GtkWidget *CreateMenuCombobox ();
GtkWidget *CreateCombobox ();
GtkWidget *CreateEditField ();

//
// delete_event
//
// Called when the window is closed.
//
void delete_event (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();
}


//
// Program starts here
//
int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *combo;
    GtkWidget *box;
    GtkWidget *vbox;
    GtkWidget *widget;
    
    // --- GTK initialization
    gtk_init (&argc, &argv);
    
    // --- Create the top level window
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    // --- You should always remember to connect the destroy event
    //     to the main window.
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (delete_event), NULL);

    // --- Give the window a border
    gtk_container_border_width (GTK_CONTAINER (window), 10);

    // --- Create a new vbox - 
    vbox = gtk_vbox_new (FALSE, 3);

    // --- Create an edit field
    widget = CreateEditField ();
    gtk_box_pack_start (GTK_BOX (vbox), widget, FALSE, FALSE, 10);
    
    // --- Create a combo box.
    combo = CreateCombobox ();
    gtk_box_pack_start (GTK_BOX (vbox), combo, FALSE, FALSE, 10);

    // --- Create a drop down list.
    box = CreateMenuCombobox ();
    gtk_box_pack_start (GTK_BOX (vbox), box, FALSE, FALSE, 10);

    // --- Add the box and make it visible
    gtk_widget_show (vbox);

    // --- Make the main window visible
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (window);

    gtk_main ();
    exit (0);
}


