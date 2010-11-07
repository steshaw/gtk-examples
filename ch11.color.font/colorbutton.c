/*
 * File: colorbutton.c
 *
 */
#include <gtk/gtk.h>
#include <stdlib.h>

GtkWidget *button;
GtkWidget *vbox;

void GetDialogColor (GdkColor *color);

/*
 * CreateBackgroundStyle 
 *
 * Create a style using the colors passed in.  
 * Set the foreground color, the text color and the
 * background color.  Note that this makes all the 
 * states be the same color. 
 *
 * fg - foreground color
 * text - text color
 * bg - background color
 */
GtkStyle *CreateBackgroundStyle (GdkColor bg)
{
    GtkStyle *defstyle;
    GtkStyle *style;
    int i;

    /* --- Get the default style --- */
    defstyle = gtk_widget_get_default_style ();

    /* --- Make a copy of it. --- */
    style = gtk_style_copy (defstyle);

    /* --- Set the colors for each state --- */
    for (i = 0; i < 5; i++) {

        /* --- Set the colors for the style --- */
        style->bg[i] = bg;
    }

    /* --- All done, here's new style --- */
    return (style);
}


/*
 * NewStyle
 *
 * Create a random style using the list of colors
 * above.  There is no checking to see if the colors 
 * would be a bad choice.
 */
GtkStyle *NewStyle (GdkColor c)
{
    GtkStyle *style;

    /* --- Create a style from the color --- */
    style = CreateBackgroundStyle (c);

    /* --- return the style --- */
    return (style);
}


/*
 * CloseAppWindow
 *
 * Close down the application
 */
gint CloseAppWindow (GtkWidget *widget, gpointer gdata)
{
    g_print ("Quitting...\n");
    gtk_main_quit ();
    return (FALSE);
}


/*
 * SetStyleRecursively
 *
 * Set the style on the current widget and on any 
 * children widgets.
 */
void SetStyleRecursively (GtkWidget *widget, gpointer data)
{
    GtkStyle *style;

    /* --- Get the style --- */
    style = (GtkStyle *) data;

    /* --- Set style on current widget --- */
    gtk_widget_set_style (widget, style);

    /* --- If it has other widgets --- */
    if (GTK_IS_CONTAINER (widget)) {

        /* --- Set the style on those widgets too --- */
        gtk_container_foreach (GTK_CONTAINER (widget), 
                           SetStyleRecursively, style);
    }
}


/*
 * button_was_clicked
 * 
 * event handler called when the button is clicked.
 * We can do this because the color dialog is modal here 
 * and doesn't return until a color is retrieved.
 */
void ButtonClicked (GtkWidget *widget, gpointer gdata)
{
    GtkStyle *style;
    GdkColor color;

    /* --- Call the color dialog to get a color --- */
    GetDialogColor (&color);

    /* --- Create a style based on the color --- */
    style = NewStyle (color);

    /* --- Set the style of the wiget based on that new style */
    SetStyleRecursively (widget, (gpointer) style);
}

/*
 * CreateButton
 *
 * Create a button and add it to the vbox.  Setup the 
 * event handler to "ButtonClicked"
 */
void CreateButton (GtkWidget *vbox, char *label)
{
    GtkWidget *button;

    /* --- Create a button --- */
    button = gtk_button_new_with_label (label);

    gtk_signal_connect (GTK_OBJECT (button), "clicked", 
                GTK_SIGNAL_FUNC (ButtonClicked), NULL);

    /* --- The main windows contains the button.  --- */
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

    /* --- Make the button visible --- */
    gtk_widget_show (button);
}


/*
 * main
 *
 * Program begins here.
 */
int main (int argc, char *argv[]) 
{
    GtkWidget *window;

    /* --- Initialize gtk, handle command line parameters --- */
    gtk_init (&argc, &argv);

    /* --- Create a window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- Need to know when window closes --- */
    gtk_signal_connect (GTK_OBJECT (window), "delete_event", 
                GTK_SIGNAL_FUNC (CloseAppWindow), NULL);
    
    /* --- Put some room around objects in the container --- */
    gtk_container_border_width (GTK_CONTAINER (window), 15);

    /* --- Create vertical packing box --- */
    vbox = gtk_vbox_new (FALSE, 0);

    /* --- Create a button --- */
    CreateButton (vbox, "Pick Color");

    /* --- Now, make the window visible --- */
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (window);

    /* --- Do not return until "quit" --- */
    gtk_main ();

    /* --- Exit status code. --- */
    return 0;
}

