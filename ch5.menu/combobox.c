/*
 * File: combobox.c
 * Auth: Eric Harlow
 * 
 * Routine to put up a font combobox.
 */

#include <gtk/gtk.h>

/*
 * CreateCombobox
 *
 * Create a drop down combobox with a few items in it.
 */
GtkWidget *CreateCombobox ()
{
    GList *cbitems = NULL;
    GtkWidget *combo;

    /* 
     * --- Create a list of the items first
     */ 
    cbitems = g_list_append (cbitems, "Arial");
    cbitems = g_list_append (cbitems, "Times Roman");
    cbitems = g_list_append (cbitems, "Wing Dings");
    cbitems = g_list_append (cbitems, "Sandscript");

    /* --- Make a combo box. --- */
    combo = gtk_combo_new ();

    /* --- Create the drop down portion of the combo --- */
    gtk_combo_set_popdown_strings (GTK_COMBO(combo), cbitems);

    /* --- Default the text in the field to a value --- */
    gtk_entry_set_text (GTK_ENTRY (GTK_COMBO(combo)->entry), "Arial");

    /* --- Make the edit portion non-editable.  They can pick a 
     *     value from the drop down, they just can't end up with
     *     a value that's not in the drop down.
     */
    gtk_entry_set_editable (GTK_ENTRY (GTK_COMBO (combo)->entry), FALSE);

    /* --- Make it visible --- */
    gtk_widget_show (combo);

    return (combo);
}
