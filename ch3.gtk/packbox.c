/* 
 * File: packbox.c
 * Auth: Eric Harlow
 *
 * Example of a packing box.
 */

#include <gtk/gtk.h>

void AddListItem (GtkWidget *listbox, char *sText);

/*
 * Destroy
 *
 * Close down the application
 */
gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    /* --- Shut down gtk event loop processing --- */
    gtk_main_quit ();

    /* --- FALSE => ok to close window --- */
    return (FALSE);
}


/*
 * Main - program begins here
 */
int main (int argc, char *argv[])
{
    GList *cbitems = NULL;
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *entry;
    GtkWidget *combo;
    GtkWidget *vbox;
    GtkWidget *box2;
    GtkWidget *check;
    GtkWidget *label;
    GtkWidget *listbox;
    int which;
    
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
    
    /* --- We create a vertical box (vbox) to pack 
     *     the horizontal boxes into. 
     */
    vbox = gtk_vbox_new (FALSE, 0);

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

    /* 
     * --- Create a button
     */

    /* --- Create a new button. --- */
    button = gtk_button_new_with_label ("Button");

    /* --- Pack the button into the vertical box (vbox box1).  --- */
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

    /* --- Show the button  --- */
    gtk_widget_show (button);
	
    /* 
     * --- Create an entry field.
     */ 

    /* --- Create the field --- */
    entry = gtk_entry_new_with_max_length (20);

    /* --- Pack the entry into the vertical box (vbox box1).  --- */
    gtk_box_pack_start (GTK_BOX (vbox), entry, FALSE, FALSE, 0);

    /* --- Put some text in the field. --- */
    gtk_entry_set_text (GTK_ENTRY (entry), "entry field");

    /* --- Show the entry  --- */
    gtk_widget_show (entry);

    /* 
     * --- Create a check button
     */

    /* --- Get the check button --- */
    check = gtk_check_button_new_with_label ("Check button");

    /* --- Pack the checkbox into the vertical box (vbox box1).  --- */
    gtk_box_pack_start (GTK_BOX (vbox), check, FALSE, FALSE, 0);

    /* --- Show the widget --- */
    gtk_widget_show (check);

    /* 
     * --- Create a listbox
     */

    /* --- Create the listbox --- */
    listbox = gtk_list_new ();

    /* --- Set listbox style. --- */
    gtk_list_set_selection_mode (GTK_LIST (listbox), GTK_SELECTION_BROWSE);

    /* --- Pack it in. --- */
    gtk_box_pack_start (GTK_BOX (vbox), listbox, FALSE, FALSE, 0);

    /* --- Make it visible --- */
    gtk_widget_show (listbox);

    AddListItem (listbox, "This is a listbox");
    AddListItem (listbox, "Quite useful ... ");
    AddListItem (listbox, "When it needs to be.");
    AddListItem (listbox, "This list can be ");
    AddListItem (listbox, "quite long, you know.");

    /* 
     * --- Create a list of items
     */ 
    cbitems = g_list_append (cbitems, "Car");
    cbitems = g_list_append (cbitems, "House");
    cbitems = g_list_append (cbitems, "Job");
    cbitems = g_list_append (cbitems, "Computer");

    /*
     * --- Make a combo box.
     */ 
    combo = gtk_combo_new ();
    gtk_combo_set_popdown_strings (GTK_COMBO(combo), cbitems);
    gtk_entry_set_text (GTK_ENTRY (GTK_COMBO(combo)->entry), "Hello");

    /* --- Pack it in. --- */
    gtk_box_pack_start (GTK_BOX (vbox), combo, FALSE, FALSE, 0);

    /* --- Make it visible --- */
    gtk_widget_show (combo);

    /* 
     * --- Make the main window visible
     */ 
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);
    gtk_widget_show (window);

    gtk_main ();
    exit (0);

}

/*
 * AddListItem
 *
 * Add an item to a listbox.
 */
void AddListItem (GtkWidget *listbox, char *sText)
{
	GtkWidget 	*item;

	/* --- Create a list item from the data element --- */
	item = gtk_list_item_new_with_label (sText);

	gtk_container_add (GTK_CONTAINER (listbox), item);

	gtk_widget_show (item);
}

