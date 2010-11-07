/*
 * File selection dialog
 */
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

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

    /* --- Allocate space and save it.--- */
    sFilename = malloc (sizeof (char) * (strlen (sTempFile) + 1));
    strcpy (sFilename, sTempFile);

    /* --- Destroy the file selection --- */
    gtk_widget_destroy (filew);
}



/*
 *
 */
int CloseDialog (GtkWidget *widget, gpointer *data)
{
    return (FALSE);
}


/*
 *
 */
int DestroyDialog (GtkWidget *widget, gpointer *data)
{
    gtk_grab_remove (widget);
    gtk_main_quit ();
    return (FALSE);
}


/*
 *
 */
char *GetFilename (char *sTitle)
{
 
    /* --- If the value is saved from last time we used the dialog,
     *     free the memory --- */
    if (sFilename) {
        free (sFilename);
        sFilename = NULL;
    }

    /* --- Create a new file selection widget --- */
    filew = gtk_file_selection_new (sTitle);

    /* --- If it's destroyed. --- */
    gtk_signal_connect (GTK_OBJECT (filew), "delete_event",
            (GtkSignalFunc) CloseDialog, &filew);

    /* --- If it's destroyed. --- */
    gtk_signal_connect (GTK_OBJECT (filew), "destroy",
            (GtkSignalFunc) DestroyDialog, &filew);

    /* --- Connect the ok_button to file_ok_sel function  --- */
    gtk_signal_connect (
            GTK_OBJECT (GTK_FILE_SELECTION (filew)->ok_button),
            "clicked", (GtkSignalFunc) file_ok_sel, filew );
    
    /* --- Connect the cancel_button to destroy the widget  --- */
    gtk_signal_connect_object (
             GTK_OBJECT (GTK_FILE_SELECTION (filew)->cancel_button),
             "clicked", (GtkSignalFunc) gtk_widget_destroy,
             GTK_OBJECT (filew));
    
    /* --- Lets set the filename, as if this were a save  --- */
    /* --- dialog, and we are giving a default filename  --- */
    gtk_file_selection_set_filename (GTK_FILE_SELECTION(filew), "");
    
    /* --- Of course, we show it --- */
    gtk_widget_show (filew);

    /* --- Make sure we keep the focus --- */
    gtk_grab_add (filew);

    /* --- Modal dialog  --- */
    gtk_main ();

    return (sFilename);
}


/*
 * CloseApp
 *
 * The window is closing down, end the gtk loop
 *
 */
int CloseApp (GtkWidget *widget, gpointer *data)
{
    printf ("CloseApp\n");
    gtk_main_quit ();
    return (FALSE);
}


