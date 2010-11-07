/*
 * File: gfontsel.c
 *
 * Front end to the font dialog box.  Creates a modal window
 * to select a font.  Use the GetFont () function to pick a name.
 *
 */

#include <gtk/gtk.h>
//#include "gtkfontsel.h"

gchar *szFontName = NULL;

/*
 * OkClicked
 *
 * Ok button was clicked
 */
void OkClicked (GtkWidget *widget, GtkWidget *fontsel)
{

    /* --- Typecast to correct type --- */
    GtkFontSelectionDialog *fsd = GTK_FONT_SELECTION_DIALOG (fontsel);

    /* --- Get the font name --- */
    szFontName = gtk_font_selection_dialog_get_font_name (fsd);

    /* --- Display the font name --- */
    printf ("Ok clicked - %s\n", szFontName);
   
    // --- Destroy the font selection window --- */
    gtk_widget_destroy (fontsel);
}


/*
 * Quit
 *
 * Quit from the program
 */
void Quit (GtkWidget *widget, gpointer data)
{
    gtk_main_quit ();
}


/*
 * GetFont
 *
 * Show the font selection dialog to allow user to
 * pick a font.  Return a gchar * with the font
 * name.
 */
gchar *GetFont ()
{
    GtkWidget *widget;
    GtkFontSelectionDialog *fontsel;
    szFontName = NULL;

    /* --- Create the font selection dialog --- */
    widget = gtk_font_selection_dialog_new ("Font Selector");

    /* --- Typecast to correct type --- */
    fontsel = GTK_FONT_SELECTION_DIALOG (widget);

    /* --- Callback for the clicked button --- */
    gtk_signal_connect (GTK_OBJECT (fontsel->ok_button), "clicked",
		        GTK_SIGNAL_FUNC (OkClicked), fontsel);

    /* --- Callback for the cancel button --- */
    gtk_signal_connect_object (GTK_OBJECT (fontsel->cancel_button), "clicked",
			       GTK_SIGNAL_FUNC (gtk_widget_destroy),
			       GTK_OBJECT (fontsel));

    /* --- Destroy signal --- */
    gtk_signal_connect (GTK_OBJECT (fontsel), "destroy",
		        GTK_SIGNAL_FUNC (Quit), fontsel);


    /* --- Show the dialog --- */
    gtk_widget_show (widget);

    /* --- Modal - wait until destroyed --- */
    gtk_main ();

    /* --- Return the font name --- */
    return (szFontName);
}
