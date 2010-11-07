/*
 * File: search.c
 * Auth: Eric Harlow
 */

#include <gtk/gtk.h>
#include "notepad.h"
#include <string.h>


int LookForString (char *szHaystack, char *szNeedle, int nStart);

static GtkWidget *dialog_window = NULL;
static GtkWidget *entry;
static char *szNeedle;

/*
 * CloseFindDialog
 *
 * Close the dialog window.  The dialog handle is passed
 * in as the data.
 */
void CloseFindDialog (GtkWidget *widget, gpointer data)
{

    /* --- Close it. --- */
    gtk_widget_destroy (widget);

    dialog_window = NULL;
}




/*
 * FindFunction
 *
 * Looks for the string in the notepad.
 */
void FindFunction (GtkWidget *widget, gpointer data)
{
    int nIndex;
    GtkWidget *text = GetTextWidget ();
    char *szHaystack;   

    /* --- Get the text from the widget --- */
    szHaystack = GetText ();

    /* --- Free up the old needle (text) --- */
    if (szNeedle) {
        g_free (szNeedle);
    }

    /* --- Get text to search for.  --- */
    szNeedle = gtk_editable_get_chars (
                 	GTK_EDITABLE (entry), 0, -1);

    /* --- Get cursor position --- */
    nIndex = GTK_EDITABLE (text)->selection_end_pos;

    /* --- Find the string --- */
    nIndex = LookForString (szHaystack, szNeedle, nIndex);

    if (nIndex >= 0) {

        /* --- Move the cursor to a position --- */
        gtk_text_set_point (GTK_TEXT (text), nIndex);

        /* --- These two lines will force the widget to --- */
        /*     scroll to the position where the text is. --- */
        gtk_text_insert (GTK_TEXT (text), NULL, NULL, NULL, " ", 1);
        gtk_text_backward_delete (GTK_TEXT (text), 1);

        /* --- Select the field found. --- */
        gtk_editable_select_region (GTK_EDITABLE (text), 
                                nIndex, nIndex + strlen (szNeedle));

        /* --- Allow it to be created again --- */
        dialog_window = NULL;
    } else {

        ShowMessage ("Find...", "Not found.  Reached the end of the file.");
    }

    /* --- Release memory. --- */
    g_free (szHaystack);
}



/*
 * LookForString
 *
 * Search for a string (szNeedle) in a larger 
 * string (szHaystack) starting at a given position (nStart)
 * in the large string.
 *
 * This is referred to as the brute force algorithm.
 */
int LookForString (char *szHaystack, char *szNeedle, int nStart)
{
    int nHaystackLength;
    int nNeedleLength;
    int nPos;

    /* --- Get the length of the strings. --- */
    nHaystackLength = strlen (szHaystack);
    nNeedleLength = strlen (szNeedle);

    /* --- Check each string --- */
    for (nPos = nStart; nPos < nHaystackLength; nPos++) {

        /* --- Did we find it here? --- */
        if (strncmp (&szHaystack[nPos], szNeedle, nNeedleLength) == 0) {

             /* --- yes, return index --- */
             return (nPos);
        }
    }

    /* Didn't find string, return -1 --- */
    return (-1);
}
 




/*
 * CancelFunction
 *
 * Close the find dialog window.
 */
void CancelFunction (GtkWidget *widget, gpointer data)
{
    /* --- Close it. --- */
    gtk_widget_destroy (GTK_WIDGET (data));

    dialog_window = NULL;
}




/*
 * FindStringDialog
 *
 * Function to display a search dialog
 */
void FindStringDialog (char *szMessage, void (*YesFunc)(), void (*NoFunc)())
{
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *hbox;

    /* --- If window is already open, just return --- */
    if (dialog_window) return;

    /* --- Create the dialog --- */
    dialog_window = gtk_dialog_new ();

    gtk_signal_connect (GTK_OBJECT (dialog_window), "destroy",
	                GTK_SIGNAL_FUNC (CloseFindDialog),
	                dialog_window);

    /* --- Set the title --- */
    gtk_window_set_title (GTK_WINDOW (dialog_window), "Find");

    /* --- Add a small border --- */
    gtk_container_border_width (GTK_CONTAINER (dialog_window), 5);

    /*
     * --- Create the message
     */

    hbox = gtk_hbox_new (TRUE, TRUE);

    /* --- Create a label with the message --- */
    label = gtk_label_new ("Find What:");
    gtk_widget_show (label);
 
    /* --- Create the entry field --- */
    entry = gtk_entry_new ();
    gtk_widget_show (entry);

    /* --- If they've searched before --- */
    if (szNeedle) {

        /* --- Prefill with the value they looked for last time --- */
        gtk_entry_set_text (GTK_ENTRY (entry), szNeedle);
    }

    gtk_box_pack_start (GTK_BOX (hbox), 
			  label, TRUE, TRUE, 0);

    gtk_box_pack_start (GTK_BOX (hbox), 
			  entry, TRUE, TRUE, 0);
    gtk_widget_show (hbox);

    /* --- Add label to the dialog --- */
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->vbox), 
                        hbox, TRUE, TRUE, 0);

    /* --- Create the "yes" button --- */
    button = gtk_button_new_with_label ("Find Next");

    gtk_signal_connect (GTK_OBJECT (button), "clicked",
	                GTK_SIGNAL_FUNC (YesFunc),
	                dialog_window);

    /* --- Add the button to the dialog --- */
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area), 
			  button, TRUE, TRUE, 0);

    /* --- Make the button visible --- */
    gtk_widget_show (button);

    /* 
     * --- Cancel button
     */ 

    /* --- Create the "Cancel" button --- */
    button = gtk_button_new_with_label ("Cancel");

    gtk_signal_connect (GTK_OBJECT (button), "clicked",
	                GTK_SIGNAL_FUNC (NoFunc),
	                dialog_window);

    /* --- Allow "Cancel" to be a default --- */
    GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);

    /* --- Add the "Cancel" button to the dialog --- */
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area), 
			  button, TRUE, TRUE, 0);

    /* --- Make the "Cancel" the default --- */
    gtk_widget_grab_default (button);

    /* --- Make the button visible. --- */
    gtk_widget_show (button);

    /* --- Show the dialog --- */
    gtk_widget_show (dialog_window);
}



/*
 * menu_Find
 * 
 * Find a string in a the editor
 */
void menu_Find (GtkWidget *widget, gpointer data)
{
    FindStringDialog ("Find", FindFunction, CancelFunction);
}
