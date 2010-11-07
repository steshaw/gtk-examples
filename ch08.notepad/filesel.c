/*
 * Auth: Eric Harlow
 * File: filesel.c
 */

#include <gtk/gtk.h>

typedef struct {

    void (*func)();
    GtkWidget *filesel;

} typFileSelectionData;

/*
 * --- Filename, remember it.
 */
static   char        *sFilename = NULL; 

char *GetExistingFile ()
{
    return (sFilename);
}
    
/*
 * FileOk
 *
 * The "Ok" button has been clicked 
 * Call the function (func) to do what is needed
 * to the file.
 */
void FileOk (GtkWidget *w, gpointer data)
{
    char *sTempFile;
    typFileSelectionData *localdata;
    GtkWidget *filew;
 
    localdata = (typFileSelectionData *) data;
    filew = localdata->filesel;

    /* --- Which file? --- */
    sTempFile = gtk_file_selection_get_filename (GTK_FILE_SELECTION (filew));

    /* --- Free old memory --- */
    if (sFilename) g_free (sFilename);

    /* --- Duplicate the string --- */
    sFilename = g_strdup (sTempFile); 

    /* --- Call the function that does the work. --- */
    (*(localdata->func)) (sFilename);

    /* --- Close the dialog --- */
    gtk_widget_destroy (filew);
}

/*
 * destroy
 *
 * Function to handle the destroying of the dialog.  We must
 * release the focus that we grabbed.
 */
static void destroy (GtkWidget *widget, gpointer *data)
{
    /* --- Remove the focus. --- */
    gtk_grab_remove (widget);

    g_free (data);
}

/*
 * GetFilename
 *
 * Show a dialog with a title and if "Ok" is selected
 * call the function with the name of the file.
 */
void GetFilename (char *sTitle, void (*callback) (char *))
{
    GtkWidget *filew = NULL;
    typFileSelectionData *data;

    /* --- Create a new file selection widget --- */
    filew = gtk_file_selection_new (sTitle);

    data = g_malloc (sizeof (typFileSelectionData));
    data->func = callback;
    data->filesel = filew;

    gtk_signal_connect (GTK_OBJECT (filew), "destroy",
            (GtkSignalFunc) destroy, data);

    /* --- Connect the "ok" button --- */
    gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),
            "clicked", (GtkSignalFunc) FileOk, data);
    
    /* --- Connect the cancel button --- */
    gtk_signal_connect_object (
             GTK_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),
             "clicked", (GtkSignalFunc) gtk_widget_destroy, 
             (gpointer) filew);
    
    if (sFilename) {

        /* --- Set the default filename --- */
        gtk_file_selection_set_filename (GTK_FILE_SELECTION (filew), 
                                         sFilename);
    }
    
    /* --- Show the dialog --- */
    gtk_widget_show (filew);

    /* --- Grab the focus. --- */
    gtk_grab_add (filew);
}
