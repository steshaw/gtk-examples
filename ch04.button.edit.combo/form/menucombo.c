#include <gtk/gtk.h>

void combo_select (GtkWidget *widget, gpointer *data)
{
    g_print ("combo select - %s\n", data);
}



GtkWidget *CreateMenuCombobox ()
{
#define NUM_COMBO_ITEMS 5   
    char *height_list[NUM_COMBO_ITEMS] = {
        "Tall",
        "Not quite tall",
        "Medium",
        "Short",
        "Wish I was tall"
};

    GtkWidget *omenu;
    GtkWidget *menu;
    GtkWidget *menuitem;
    GSList *group;
    int i;

    omenu = gtk_option_menu_new ();
    menu = gtk_menu_new ();
    group = NULL;

    for (i = 0; i < NUM_COMBO_ITEMS; i++) {
 
        // --- Create a menu item with a label
        menuitem = gtk_radio_menu_item_new_with_label (group, height_list[i]);
        
        // --- Get the group that the menu item is in.
        group = gtk_radio_menu_item_group (GTK_RADIO_MENU_ITEM (menuitem));

        // --- Add the menu item to the menu
        gtk_menu_append (GTK_MENU (menu), menuitem);

        // --- Make the item visible
        gtk_widget_show (menuitem);

        // --- Let me know when it's selected
        gtk_signal_connect_object (GTK_OBJECT (menuitem),
                                   "activate",
                                   GTK_SIGNAL_FUNC (combo_select),
                                   (gpointer) height_list[i]);

    }

    // --- Associate the menu with the option menu.
    gtk_option_menu_set_menu (GTK_OPTION_MENU (omenu), menu);

    gtk_widget_show (omenu);

    return (omenu);
}
