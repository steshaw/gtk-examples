/*
 * File: listbox.c
 * Auth: Eric Harlow
 * 
 * Show the listbox widget in use.
 */

#include <gtk/gtk.h>

/* --- Prototypes. --- */
void AddListItem (GtkWidget *listbox, char *sText);

/*
 * CloseAppWindow
 *
 * --- Closing down window, end gtk
 */
gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();

    return (FALSE);
}


/*
 * listitem_selected
 *
 * Event handler called when an item is selected.
 */
void listitem_selected (GtkWidget *widget, gpointer *data)
{
    g_print ("item selected - %s\n", data);
}


/*
 * GetItemLabel
 *
 * Returns the label's string given a widget that contains the label.
 */
char *GetItemLabel (GtkWidget *widget)
{
    GtkWidget *label = NULL;
    char *str = NULL;
    GList *children;
    GtkWidget *child;
    
    /* --- Get list of children in the list item --- */
    children = gtk_container_children (GTK_CONTAINER (widget));

    /* --- Get the widget --- */
    child = GTK_WIDGET (children->data);

    /* --- If the widget is a label --- */
    if (GTK_IS_LABEL (child)) {

        /* --- Get the string in the label --- */
        gtk_label_get (GTK_LABEL (child), &str);
    }

    /* --- Return the string --- */
    return (str);
}


/*
 * DisplaySelectedItems
 *
 * Display the contents of the list items in the listbox.
 * Show state and label information
 */
void DisplaySelectedItems (GtkWidget *widget)
{
    GList *children;
    GtkWidget *child;
    
    /* --- Get children in the listbox (list items) --- */
    children = gtk_container_children (GTK_CONTAINER (widget));

    /* --- Loop through them all --- */
    while (children) {

        /* --- Get child's widget information --- */
        child = GTK_WIDGET (children->data);

        /* --- Display list item information --- */
        printf ("state = %d, label = %s\n", 
                (int) GTK_WIDGET_STATE (child), 
                GetItemLabel (child));

        /* --- Go to the next child --- */
        children = children->next;
    }
}

/*
 * listbox_changed
 *
 * Event Handler for the listbox
 */
void listbox_changed (GtkWidget *widget, gpointer *data)
{
    GtkList *listbox;

    g_print ("%s\n", data);
    DisplaySelectedItems (widget);
}

/*
 * main
 */
int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *listbox;
    
    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);
    
    /* --- Create the top level window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- You should always remember to connect the destroy event
     *     to the main window.
     */
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
            GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    /* --- Give the window a border --- */
    gtk_container_border_width (GTK_CONTAINER (window), 10);
    
    /* 
     * --- Create a listbox
     */

    /* --- Create the listbox --- */
    listbox = gtk_list_new ();

    gtk_signal_connect (GTK_OBJECT (listbox), "selection_changed",
            GTK_SIGNAL_FUNC (listbox_changed), "selection_changed");

    /* --- Set listbox style. --- */
    gtk_list_set_selection_mode (GTK_LIST (listbox), GTK_SELECTION_BROWSE);

    /* --- Make it visible --- */
    gtk_widget_show (listbox);

    /* --- Add items into the listbox --- */
    AddListItem (listbox, "This is a listbox");
    AddListItem (listbox, "Quite useful ... ");
    AddListItem (listbox, "When it needs to be.");
    AddListItem (listbox, "This list can be ");
    AddListItem (listbox, "quite long, you know.");

    /* 
     * --- Make the main window visible
     */ 
    gtk_container_add (GTK_CONTAINER (window), listbox);
    gtk_widget_show (window);

    gtk_main ();
    return (0);
}

/*
 * AddListItem
 *
 * Add the text to the listbox.
 */
void AddListItem (GtkWidget *listbox, char *sText)
{
    GtkWidget     *item;

    /* --- Create a list item from the data element --- */
    item = gtk_list_item_new_with_label (sText);

    /* --- Conigure for the "select" event --- */
    gtk_signal_connect (GTK_OBJECT (item), "select",
            GTK_SIGNAL_FUNC (listitem_selected), sText);

    /* --- Add item to the listbox --- */
    gtk_container_add (GTK_CONTAINER (listbox), item);

    /* --- Make it visible --- */
    gtk_widget_show (item);
}

