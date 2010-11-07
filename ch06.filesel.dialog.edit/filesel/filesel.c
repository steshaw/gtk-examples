/*
 * File selection dialog
 */
#include <gtk/gtk.h>

static   GtkWidget     *filew;
static   char        *sFilename = NULL; 
    
/* 
 * Get the selected filename and print it to the console 
 */
void file_ok_sel (GtkWidget *w, GtkFileSelection *fs)
{
    char *sTempFile;

    /* --- Get the name --- */
    sTempFile = gtk_file_selection_get_filename (GTK_FILE_SELECTION (fs));

    printf ("Getting %s\n", sTempFile);

    /* --- Allocate space and save it. --- */
    sFilename = malloc (sizeof (char) * (strlen (sTempFile) + 1));
    strcpy (sFilename, sTempFile);

    /* --- Destroy the file selection --- */
    gtk_widget_destroy (filew);
}


void file_cancel_sel (GtkWidget *w, GtkFileSelection *fs)
{

    /* --- Destroy the file selection --- */
    gtk_widget_destroy (filew);
}


/*
 * DestroyDialog
 *
 * Destroy the dialog (obvious, eh?) but also remove the
 * grab and close the modal. 
 */
int DestroyDialog (GtkWidget *widget, gpointer *data)
{
    printf ("widget destroyed\n");
    gtk_grab_remove (widget);
    gtk_main_quit ();
    return (FALSE);
}


/*
 * GetFilename 
 */
char *GetFilename (char *sTitle)
{
 
    /* --- If the value is saved from last time we used the dialog,
     * --- free the memory
     */
    if (sFilename) {
        free (sFilename);
        sFilename = NULL;
    }

    /* --- Create a new file selection widget --- */
    filew = gtk_file_selection_new (sTitle);

    /* --- If it's destroyed --- */
    gtk_signal_connect (GTK_OBJECT (filew), "destroy",
            (GtkSignalFunc) DestroyDialog, &filew);

    /* --- Connect the ok_button to file_ok_sel function  --- */
    gtk_signal_connect (
            GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),
            "clicked", (GtkSignalFunc) file_ok_sel, filew );
    
    /* --- Connect the cancel_button to destroy the widget  --- */
    gtk_signal_connect (
             GTK_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),
             "clicked", (GtkSignalFunc) file_cancel_sel, filew);
    
    /* --- Lets set the filename, as if this were a save 
     *     dialog, and we are giving a default filename 
     */
    gtk_file_selection_set_filename (GTK_FILE_SELECTION(filew), "");
    
    /* --- Of course, we show it --- */
    gtk_widget_show (filew);

    /* --- Make sure we keep the focus --- */
    gtk_grab_add (filew);

    gtk_main ();

    return (sFilename);
}


/*
 * ButtonClicked
 *
 * Get a filename from the file selection dialog.
 */
void ButtonClicked (GtkWidget *widget, gpointer data)
{
    /* --- Pass in a title of "hey, hey, hey" --- */
    char *sFile = GetFilename ("Hey, hey, hey");

    printf ("Got %s\n", sFile);
}


/*
 * CloseApp
 *
 * The window is closing down, end the gtk loop
 */
int CloseApp (GtkWidget *widget, gpointer *data)
{
    printf ("CloseApp\n");
    gtk_main_quit ();
    return (FALSE);
}



/*
 * main
 *
 * Program begins here
 */
int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *frame;          

    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);

    /* --- Create the top window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- Give the window a title. --- */
    gtk_window_set_title (GTK_WINDOW (window), "Frame example");

    /* --- Set the window size. --- */
    gtk_widget_set_usize (window, 250, 250);

    /* --- Need to know when app is being closed --- */
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
                        GTK_SIGNAL_FUNC (CloseApp), NULL);

    /*
     * --- Create the start button with smilely face on it
     */
    frame = gtk_button_new_with_label ("File dialog");

    gtk_signal_connect (GTK_OBJECT (frame), "clicked",
                        GTK_SIGNAL_FUNC (ButtonClicked), NULL);

    gtk_container_add (GTK_CONTAINER (window), frame);

    /*
     * --- Make everything visible
     */

    gtk_widget_show (frame);
    gtk_widget_show (window);

    gtk_main ();
    return (0);
}



