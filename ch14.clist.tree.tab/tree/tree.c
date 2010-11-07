/*
 * Auth: Eric Harlow
 *
 * Linux application development.
 *
 * Starts with the current directory and add the files 
 * and directories into the tree widget to illustrate 
 * how to use the tree and how the tree looks when 
 * populated with data. 
 */
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <gtk/gtk.h>

#define MAX_PATH 1024

static void PopulateListbox (char *szPath);

GtkWidget *listbox;



/*
 * AddListItem
 *
 * Add an item to a list box.
 *
 * listbox - listbox to add the item to
 * text - text string to display in the listbox
 */
void AddListItem (GtkWidget *listbox, char *sText)
{
    GtkWidget *item;

    /* --- Create list item from data --- */
    item = gtk_list_item_new_with_label (sText);

    /* --- Add item to listbox --- */
    gtk_container_add (GTK_CONTAINER (listbox), item);
 
    /* --- Make it visible --- */
    gtk_widget_show (item);
}



/*
 * IsDirectory
 *
 * Check to see if the path is a directory or just a file. 
 *
 * buffer - full path to check.
 * 
 * returns (TRUE) if the path is a directory 
 */
int IsDirectory (char *buffer)
{
    struct stat buf;

    if (stat (buffer, &buf) < 0) {

        /* --- Error - ignored. --- */
        return (FALSE);
    }

    /* --- Return whether it is a directory. --- */
    return (S_ISDIR (buf.st_mode));
}


/*
 *
 */
void select_item (GtkWidget *widget, gpointer data)
{
    printf ("item selected %s\n", (char *) data);
  
    PopulateListbox ((char *) data);
}


/*
 * CreateSubtree
 *
 * Add directories into the tree control and puts in
 * an event handler so that when an item is selected
 * in the tree, the listbox will be populated.
 *
 * szPath - path to add the files from
 */ 
static void CreateSubtree (char *szPath, char *szDir, GtkWidget* item)
{
    DIR *directory;
    struct dirent *dirEntry;
    GtkWidget* item_subtree = NULL;
    GtkWidget* item_new;
    char buffer[MAX_PATH];

    /* --- Read current directory --- */
    directory = opendir (szPath);

    /* --- While we are reading through the directory  --- */
    while (dirEntry = readdir (directory)) {

        /* --- Don't count these as valid directories for display --- */
        if (!strcmp (dirEntry->d_name, "..") ||
            !strcmp (dirEntry->d_name, ".")) {

            /* --- Ignore these directories ("." and "..") --- */
        } else {

            /* --- Create full path --- */
            sprintf (buffer, "%s/%s", szPath, dirEntry->d_name); 

            /* --- If this is a directory --- */
            if (IsDirectory (buffer)) {

                if (item_subtree == NULL) {

                    /* --- Create a new tree item. --- */
                    item_subtree = gtk_tree_new ();

                    /* --- Add the item to the tree. --- */
                    gtk_tree_item_set_subtree (GTK_TREE_ITEM (item), item_subtree);
                }

                /* --- Create an entry for the file. --- */
                item_new = gtk_tree_item_new_with_label (dirEntry->d_name);

                /* --- Add item to the tree --- */
                gtk_tree_append (GTK_TREE (item_subtree), item_new);

                /* --- Load all of its elements into the tree --- */
                CreateSubtree (buffer, dirEntry->d_name, item_new); 

                /* --- Make it visible. --- */
                gtk_widget_show (item_new);

                /* --- Notify when an item is selected. --- */
                gtk_signal_connect (GTK_OBJECT (item_new), 
                        "select",
                        GTK_SIGNAL_FUNC (select_item), 
                        g_strdup (buffer));

            }
        }
    }

    /* --- All done --- */
    closedir (directory);

    gtk_widget_show (item);
}



/*
 * PopulateListbox
 *
 * Adds the files for the directory into the listbox.
 * Only adds files.
 *
 * szPath - path to add the files from
 */ 
static void PopulateListbox (char *szPath)
{
    DIR *directory;
    struct dirent *dirEntry;
    char buffer[MAX_PATH];

    /* --- Clear the listbox. --- */
    gtk_list_clear_items (GTK_LIST (listbox), 
                          0, 
                          g_list_length (GTK_LIST (listbox)->children));

    /* --- Read current directory --- */
    directory = opendir (szPath);

    /* --- While we are reading through the directory  --- */
    while (dirEntry = readdir (directory)) {

        /* --- Don't count these as valid directories for display --- */
        if (!strcmp (dirEntry->d_name, "..") ||
            !strcmp (dirEntry->d_name, ".")) {

            /* --- Ignore these directories ("." and "..") --- */
        } else {

            /* --- Create full path --- */
            sprintf (buffer, "%s/%s", szPath, dirEntry->d_name); 

            /* --- If this not directory --- */
            if (!IsDirectory (buffer)) {

                /* --- Add file to the listbox --- */
                AddListItem (listbox, dirEntry->d_name);
            }
        }
    }

    /* --- All done --- */
    closedir (directory);
}



/*
 * CreateTree
 *
 * Create the tree that shows the file structure.  
 *
 * window - parent window
 */
static void CreateTree (GtkWidget *window)
{
    char buffer[MAX_PATH];
    GtkWidget *box1;
    GtkWidget *box2;
    GtkWidget *scrolled_win;
    GtkWidget *tree;
    GtkWidget *leaf;

    /* --- Vertical box --- */
    box1 = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), box1);
    gtk_widget_show (box1);

    /* --- create tree box --- */
    box2 = gtk_vbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box1), box2, TRUE, TRUE, 0);
    gtk_container_border_width(GTK_CONTAINER(box2), 5);
    gtk_widget_show(box2);

    /* --- Create scrolled window for the tree --- */
    scrolled_win = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_win),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start (GTK_BOX (box2), scrolled_win, TRUE, TRUE, 0);
    gtk_widget_set_usize (scrolled_win, 250, 250);
    gtk_widget_show (scrolled_win);
  
    /* 
     * --- create root tree widget 
     */ 
    tree = gtk_tree_new();

    /*
     * --- Create listbox
     */
    listbox = gtk_list_new ();
    gtk_widget_set_usize (listbox, 250, 250);
    gtk_box_pack_start (GTK_BOX (box1), listbox, TRUE, TRUE, 0);
    gtk_widget_show (listbox);

    /* --- Add the tree to the scrolled window. --- */
    gtk_container_add (GTK_CONTAINER (scrolled_win), tree);

    /* --- Make it visible. --- */
    gtk_widget_show (tree);

    /* 
     * --- Create root tree item widget (current directory)
     */
    leaf = gtk_tree_item_new_with_label (
                   getcwd (buffer, sizeof (buffer)));

    /* --- Add item to the tree. --- */
    gtk_tree_append (GTK_TREE (tree), leaf);

    /* --- Make it visible --- */
    gtk_widget_show (leaf);

    /* --- Create a subtree under this item --- */
    CreateSubtree (getcwd (buffer, sizeof (buffer)), 
                   getcwd (buffer, sizeof (buffer)), leaf);

    /* --- Make window visible. --- */
    gtk_widget_show (window);
}



/*
 * delete_event
 *
 * The window is closing down, end the gtk loop
 */
void delete_event (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();
}



/*
 * main
 *
 * Program begins here
 */
int main (int argc, char *argv[])
{
    GtkWidget *window;

    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);

    /* --- Create the top window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- Give the window a title. --- */
    gtk_window_set_title (GTK_WINDOW (window), "Files using trees");

    /* --- Set the window size. --- */
    gtk_widget_set_usize (window, 250, 250);

    /* --- You should always remember to connect the destroy event 
     *     to the main window.
     */
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
                        GTK_SIGNAL_FUNC (delete_event), NULL);


    gtk_widget_show (window);

    /* --- Create the tree  --- */
    CreateTree (window);

    gtk_main ();

    exit (0);
}



