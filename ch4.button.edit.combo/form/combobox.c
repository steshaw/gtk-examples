
#include <gtk/gtk.h>


//
//
//
void combo_selectx (GtkWidget *widget, gpointer *data)
{
    g_print ("item selected\n");
}



//
// CreateCombobox
//
// Create a drop down combobox with a few items in it.
//
GtkWidget *CreateCombobox ()
{
    GList *cbitems = NULL;
    GtkWidget *combo;

    // 
    // --- Create a list of the items first
    // 
    cbitems = g_list_append (cbitems, "Car");
    cbitems = g_list_append (cbitems, "House");
    cbitems = g_list_append (cbitems, "Job");
    cbitems = g_list_append (cbitems, "Computer");

    // --- Make a combo box.
    combo = gtk_combo_new ();

    gtk_signal_connect (GTK_OBJECT (GTK_COMBO (combo)->entry), "changed",
            GTK_SIGNAL_FUNC (combo_selectx), NULL);

    // --- Create the drop down portion of the combo
    gtk_combo_set_popdown_strings (GTK_COMBO(combo), cbitems);

    // --- Default the text in the field to a value
    gtk_entry_set_text (GTK_ENTRY (GTK_COMBO(combo)->entry), "Hello");

    // --- Make the edit portion non-editable.  They can pick a 
    //     value from the drop down, they just can't end up with
    //     a value that's not in the drop down.
    gtk_entry_set_editable (GTK_ENTRY (GTK_COMBO (combo)->entry), FALSE);
    //gtk_combo_set_value_in_list (GTK_COMBO (combo), 1, FALSE);

    // --- Make it visible
    gtk_widget_show (combo);

    return (combo);
}
