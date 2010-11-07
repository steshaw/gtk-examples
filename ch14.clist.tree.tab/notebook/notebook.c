/*
 * Auth: Eric Harlow
 * File: Notebook.c
 * 
 * Create a sample notebook application
 */

#include <gtk/gtk.h>


void delete_event (GtkWidget *widget, gpointer data)
{
    gtk_main_quit ();
}



/*
 * PageSwitch
 *
 * Event that occurs when a different page is now
 * the focus.
 */
static void PageSwitch (GtkWidget *widget, 
                         GtkNotebookPage *page, 
                         gint page_num)
{

    printf ("page switch\n");
}



/*
 * AddPage
 *
 * Add a page to the notebook 
 *
 * notebook - existing notebook
 * szName - name to give to the new page
 */
GtkWidget *AddPage (GtkWidget *notebook, char *szName)
{
    GtkWidget *label;
    GtkWidget *frame;

    /* --- Create a label from the name. --- */
    label = gtk_label_new (szName);
    gtk_widget_show (label);

    /* --- Create a frame for the page --- */
    frame = gtk_frame_new (szName);
    gtk_widget_show (frame);

    /* --- Add a page with the frame and label --- */
    gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);

    return (frame);
}



/*
 * CreateNotebook
 *
 * Create a new notebook and add pages to it.
 *
 * window - window to create the notebook in.
 */
static void CreateNotebook (GtkWidget *window, 
                            GtkPositionType pos)
{
    GtkWidget *widget;
    GtkWidget *box1;
    GtkWidget *notebook;

    /* --- Let us know when getting destroyed. --- */
    gtk_signal_connect (GTK_OBJECT (window), "destroy",
			  GTK_SIGNAL_FUNC(gtk_widget_destroyed),
			  &window);

    /* --- Set border width --- */
    gtk_container_border_width (GTK_CONTAINER (window), 0);

    box1 = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), box1);

    /* --- Create the notebook --- */
    notebook = gtk_notebook_new ();

    /* --- Listen for the switch page event --- */
    gtk_signal_connect (GTK_OBJECT (notebook), "switch_page",
			  GTK_SIGNAL_FUNC (PageSwitch), NULL);

    /* --- Make sure tabs are set on correct side --- */
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), pos);

    /* --- Add notebook to vbox --- */
    gtk_box_pack_start (GTK_BOX (box1), notebook, TRUE, TRUE, 0);

    /* --- Give notebook a border --- */
    gtk_container_border_width (GTK_CONTAINER (notebook), 10);
   
    /* --- Add pages to the notebook --- */
    widget = AddPage (notebook, "Groups");
    widget = AddPage (notebook, "Users");
    widget = AddPage (notebook, "Todo");
      
    /* --- Show everything. --- */
    gtk_widget_show_all (window);
}


/*
 * CreateNotebookWindow
 *
 * Create a window to display the notebook.
 */
void CreateNotebookWindow (char *szTitle, GtkPositionType pos)
{
    GtkWidget *window;

    /* --- Create the top window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- Give the window a title. --- */
    gtk_window_set_title (GTK_WINDOW (window), szTitle);

    /* --- Set the window size. --- */
    gtk_widget_set_usize (window, 250, 250);

    /* --- You should always remember to connect the destroy event
     *     to the main window.
     */
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
                        GTK_SIGNAL_FUNC (delete_event), NULL);

    /* --- Make main page visible --- */
    gtk_widget_show (window);

    /* --- Create notebook --- */
    CreateNotebook (window, pos);
}


/*
 * main
 *
 * Program begins here
 */
int main (int argc, char *argv[])
{

    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);

    CreateNotebookWindow ("Tabs on top", GTK_POS_TOP);
    CreateNotebookWindow ("Tabs on left", GTK_POS_LEFT);
    CreateNotebookWindow ("Tabs on bottom", GTK_POS_BOTTOM);
    CreateNotebookWindow ("Tabs on right", GTK_POS_RIGHT);

    gtk_main ();
    exit (0);
}
