/*
 * menu.c
 *
 * Menus using the item factory
 */

#include <gtk/gtk.h>

static void QuitApp (gpointer callback_data,
                         guint callback_action,
                         GtkWidget *widget);
static void ShowMenu (gpointer callback_data,
                         guint callback_action,
                         GtkWidget *widget);


/*
 * Structure to build the menus
 */
static GtkItemFactoryEntry menu_items[] = {
  {"/_File",                 NULL,         0,        0, "<Branch>" },
  {"/File/tearoff1",         NULL,         ShowMenu, 0, "<Tearoff>" },
  {"/File/_New",             "<control>N", ShowMenu, 0 },
  {"/File/_Open",            "<control>O", ShowMenu, 0 },
  {"/File/_Save",            "<control>S", ShowMenu, 0 },
  {"/File/Save _As...",      NULL,         ShowMenu, 0 },
  {"/File/sep1",             NULL,         ShowMenu, 0, "<Separator>" },
  {"/File/_Quit",            "<control>Q", QuitApp,  0 },

  {"/_Edit",                 NULL,         0,        0, "<Branch>" },
  {"/_Edit/Cut",             "<control>X", 0,        0, 0},
  {"/_Edit/_Copy",           "<control>C", 0,        0, 0},
  {"/_Edit/_Paste",          "<control>V", 0,        0, 0},
  {"/_Edit/_Font",           NULL,         0,        0, "<Branch>" },
  {"/_Edit/Font/_Bold",      NULL,         ShowMenu, 0, "<RadioItem>" },
  {"/_Edit/Font/_Italics",   NULL,         ShowMenu, 0, "<RadioItem>" },
  {"/_Edit/Font/_Underline", NULL,         ShowMenu, 0, "<RadioItem>" },
  {"/_Edit/_Color",          NULL,         0,        0, "<Branch>" },
  {"/_Edit/Color/_Red",      NULL,         ShowMenu, 0, "<CheckItem>" },
  {"/_Edit/Color/_Blue",     NULL,         ShowMenu, 0, "<CheckItem>" },
  {"/_Edit/Color/_Green",    NULL,         ShowMenu, 0, "<CheckItem>" },
 
  {"/_Help",                 NULL,         0,        0, "<LastBranch>" },
  {"/Help/_About",           NULL,         ShowMenu, 0 },
};

/*
 * FactoryQuit
 *
 * Quit the app when called from the menu.
 */
static void QuitApp (gpointer callback_data,
                         guint callback_action,
                         GtkWidget *widget)
{
    /* --- Display a message about the menu --- */
    g_message ("ItemFactory: activated \"%s\"", 
                gtk_item_factory_path_from_widget (widget));

    /* --- Exit the application --- */
    gtk_main_quit ();
}


/*
 * ShowMenu
 *
 * Display the item that was selected 
 */
static void ShowMenu (gpointer  callback_data,
                         guint     callback_action,
                         GtkWidget *widget)
{
    g_message ("ItemFactory: activated \"%s\", action %d", 
               gtk_item_factory_path_from_widget (widget), (int) callback_action);
}



/*
 * --- Number of items in the menu 
 */
static int nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);


/*
 * CloseApp
 *
 * Close down GTK when they close the application
 * window
 */
static gint CloseApp (GtkWidget *widget, gpointer data)
{
    gtk_main_quit ();
    return (TRUE);
}


/*
 *
 */
static void CreateItemFactory ()
{
    GtkWidget *window = NULL;
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *separator;
    GtkWidget *label;
    GtkWidget *button;
    GtkAccelGroup *accel_group;
    GtkItemFactory *item_factory;
      
    /* --- Create the window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
      
    /* --- Set the minimum size --- */
    gtk_widget_set_usize (window, 200, 200);

    /* --- Connect signals for destruction --- */
    gtk_signal_connect (GTK_OBJECT (window), "destroy",
              GTK_SIGNAL_FUNC (gtk_widget_destroyed),
              &window);
    gtk_signal_connect (GTK_OBJECT (window), "delete-event",
              GTK_SIGNAL_FUNC (CloseApp),
              NULL);
      
    /* --- Create a new accel group --- */
    accel_group = gtk_accel_group_new ();

    /* --- Create a new item factory --- */
    item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, 
                                         "<blah>", 
                                         accel_group);

    /* --- Create the items in the factory using data --- */
    gtk_item_factory_create_items (item_factory, 
                                   nmenu_items, 
                                   menu_items, 
                                   NULL);

    /* --- Attach the accel group to app window --- */
    gtk_accel_group_attach (accel_group, GTK_OBJECT (window));

    /* --- Set the title of the window --- */
    gtk_window_set_title (GTK_WINDOW (window), "Item Factory");

    /* --- No border --- */
    gtk_container_border_width (GTK_CONTAINER (window), 0);
      
    /* --- Vertical packing box. --- */
    box1 = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), box1);
     
    /* --- Put menu in the vertical packing box --- */
    gtk_box_pack_start (GTK_BOX (box1),
             gtk_item_factory_get_widget (item_factory, "<blah>"),
             FALSE, FALSE, 0);

    /* --- Make everything visible --- */
    gtk_widget_show_all (window);
}

int main (int argc, char *argv[])
{
  gtk_init (&argc, &argv);

  CreateItemFactory ();

  gtk_main ();

  return 0;
}
