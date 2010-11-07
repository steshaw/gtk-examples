/*
 * File: colordiag.c
 *
 * Show the color dialog box to allow someone to pick
 * a color for the button.
 */
#include <gtk/gtk.h>


/*
 * Structure to eliminate global variables
 */
typedef struct  {
 
    GtkWidget *dialog;
    GdkColor *color;

} typColorDialogInfo;



/*
 * CloseDialog
 *
 * Close the modal dialog using the gtk_main_quit since it 
 * was made modal by the gtk_main function.  Also release
 * the grab and free the memory.
 */
void CloseDialog (GtkWidget *w, typColorDialogInfo *di)
{
    gtk_main_quit ();
    gtk_grab_remove (w);
    g_free (di);
}


/*
 * OkButtonClicked
 *
 * Get the color currently selected in the dialog box and 
 * get the rgb components out.  Set the GdkColor in the 
 * colorInfo structure to those values.
 */
void OkButtonClicked (GtkWidget *w,
                 typColorDialogInfo *colorInfo)
{
    GtkColorSelectionDialog *cs;
    GtkColorSelection *colorsel;
    gdouble color[4];

    /* --- Get the dialog and the color --- */
    cs = (GtkColorSelectionDialog *) colorInfo->dialog;
    colorsel = GTK_COLOR_SELECTION (cs->colorsel);

    /* --- Get the color and assign it to the GdkColor --- */
    gtk_color_selection_get_color (colorsel, color);
    colorInfo->color->red = color[0] * 0xffff;
    colorInfo->color->green = color[1] * 0xffff;
    colorInfo->color->blue = color[2] * 0xffff;

    /* --- Destroy the dialog --- */
    gtk_widget_destroy (GTK_WIDGET (cs));
}

/*
 * ColorSelectionChanged
 *
 * Notified when the color has changed within the dialog.
 */
void ColorSelectionChanged (GtkWidget *w,
                         GtkColorSelectionDialog *cs)
{
    GtkColorSelection *colorsel;
    gdouble color[4];

    /* --- Get the color from the selection --- */
    colorsel = GTK_COLOR_SELECTION (cs->colorsel);
    gtk_color_selection_get_color (colorsel, color);

    g_print ("The color has been changed!\n");
}


/*
 * GetDialogColor
 *
 * Display a modal dialog and get the user to select a 
 * color which can be sent back to the user.
 */
void GetDialogColor (GdkColor *color)
{
    static GtkWidget *window = NULL;
    typColorDialogInfo *colorInfo;

    /* --- New dialog --- */
    window = gtk_color_selection_dialog_new ("Color Selection Dialog");

    /* --- Allocate memory for the colorinfo and populate it --- */
    colorInfo = g_malloc (sizeof (typColorDialogInfo));
    colorInfo->dialog = window;
    colorInfo->color = color;

    gtk_color_selection_set_opacity (
        GTK_COLOR_SELECTION (GTK_COLOR_SELECTION_DIALOG (window)->colorsel),
	TRUE);

    gtk_color_selection_set_update_policy (
        GTK_COLOR_SELECTION (GTK_COLOR_SELECTION_DIALOG (window)->colorsel),
	GTK_UPDATE_CONTINUOUS);

    //gtk_window_position (GTK_WINDOW (window), GTK_WIN_POS_MOUSE);

    /* --- Want to know when window is destroyed --- */
    gtk_signal_connect (GTK_OBJECT (window), "destroy",
                          GTK_SIGNAL_FUNC (CloseDialog),
                          colorInfo);

    /* --- Want to know when color changes --- */
    gtk_signal_connect (
	GTK_OBJECT (GTK_COLOR_SELECTION_DIALOG (window)->colorsel),
	"color_changed",
	GTK_SIGNAL_FUNC (ColorSelectionChanged),
	window);

    /* --- Want to know when 'ok' button pressed --- */
    gtk_signal_connect (
	GTK_OBJECT (GTK_COLOR_SELECTION_DIALOG (window)->ok_button),
	"clicked",
	GTK_SIGNAL_FUNC (OkButtonClicked),
	colorInfo);

    /* --- Destroy dialog when "cancel" is clicked --- */
    gtk_signal_connect_object (
        GTK_OBJECT (GTK_COLOR_SELECTION_DIALOG (window)->cancel_button),
	"clicked",
	GTK_SIGNAL_FUNC(gtk_widget_destroy),
	GTK_OBJECT (window));

    /* --- Show window --- */
    gtk_widget_show (window);

    /* --- Grab --- */
    gtk_grab_add (window);

    /* --- This makes it modal in a loop until quit is called --- */
    gtk_main ();
}
