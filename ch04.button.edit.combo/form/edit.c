#include <gtk/gtk.h>


//
// edit_changed
//
void edit_changed (GtkWidget *widget, gpointer *data) 
{
    gchar *str;

    str = gtk_entry_get_text (GTK_ENTRY (widget));
    g_print ("Edit changed to %s\n", str);
}



//
// CreateNameField
//
// Create an edit field for the name.
//
GtkWidget *CreateEditField ()
{

    GtkWidget *hbox;
    GtkWidget *label;
    GtkWidget *entry;

    // --- Create a box to put the label and edit field.
    hbox = gtk_vbox_new (FALSE, 0);

    // --- create a new label. 
    label = gtk_label_new ("Name:");
	
    // --- Align the label to the left side.  
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0);

    // --- Pack the label into the vertical box (vbox box1). 
    gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
	
    // --- Show the label 
    gtk_widget_show (label);

    // --- Create an entry field.
    entry = gtk_entry_new_with_max_length (20);

    // --- Pack the entry into the vertical box (vbox box1). 
    gtk_box_pack_start (GTK_BOX (hbox), entry, FALSE, FALSE, 0);

    // --- Put some text in the field.
    gtk_entry_set_text (GTK_ENTRY (entry), "entry field");

    // --- Show the entry 
    gtk_widget_show (entry);

    gtk_signal_connect (GTK_OBJECT (entry), "changed",
			GTK_SIGNAL_FUNC (edit_changed), NULL);

    // --- Make the box visible
    gtk_widget_show (hbox);

    return (hbox);
}
