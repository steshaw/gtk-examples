/*
 * Show how to modify a widgets colors.
 * --- NORMAL, PRELIGHT, ACTIVE, INSENSITIVE, SELECTED
 */

#include <gtk/gtk.h>
#include <stdlib.h>

GtkWidget *button;
GtkStyle *styleRed;

/*
 * --- Define colors
 */
GdkColor red = {0, 0xffff, 0x0000, 0x0000};
GdkColor blue = {0, 0x0000, 0x0000, 0xffff};
GdkColor green = {0, 0x0000, 0xffff, 0x0000};
GdkColor yellow = {0, 0xffff, 0xffff, 0x0000};
GdkColor purple = {0, 0xffff, 0x0000, 0xffff};
GdkColor orange = {0, 0xffff, 0x9999, 0x0000};
GdkColor cyan = {0, 0x0000, 0xffff, 0xffff};
GdkColor black = {0, 0x0000, 0x0000, 0x0000};
GdkColor white = {0, 0xffff, 0xffff, 0xffff};

/* --- Create a list of colors to use for the
 *     random color button.
 */
GdkColor colorlist[] = {
    {0, 0xffff, 0x0000, 0x0000},
    {0, 0x0000, 0x0000, 0xffff},
    {0, 0x0000, 0xffff, 0x0000},
    {0, 0xffff, 0xffff, 0x0000},
    {0, 0xffff, 0x0000, 0xffff},
    {0, 0xffff, 0x9999, 0x0000},
    {0, 0x0000, 0xffff, 0xffff},
    {0, 0x0000, 0x0000, 0x0000},
    {0, 0xffff, 0xffff, 0xffff} 
};

/* --- Calculate number of buttons. --- */
static int numColors = sizeof (colorlist) / sizeof (GdkColor);

/* --- Create styles for the dialog windows --- */
GtkStyle *styleChristmas;
GtkStyle *styleHalloween;
GtkStyle *styleStPatricks;
GtkStyle *styleUSIndependence;

/* --- vbox for the buttons --- */
GtkWidget *vbox;

void CallButton (GtkWidget *widget, gpointer data);

/*
 * AllocateColors
 * 
 * Allocate the colors in the colormap.
 */
void AllocateColors ()
{
    int i;

    gdk_color_alloc (gdk_colormap_get_system (), &red);
    gdk_color_alloc (gdk_colormap_get_system (), &blue);
    gdk_color_alloc (gdk_colormap_get_system (), &green);
    gdk_color_alloc (gdk_colormap_get_system (), &yellow);
    gdk_color_alloc (gdk_colormap_get_system (), &purple);
    gdk_color_alloc (gdk_colormap_get_system (), &orange);
    gdk_color_alloc (gdk_colormap_get_system (), &cyan);
    gdk_color_alloc (gdk_colormap_get_system (), &black);
    gdk_color_alloc (gdk_colormap_get_system (), &white);

    for (i = 0; i < numColors; i++) {
        gdk_color_alloc (gdk_colormap_get_system (), &colorlist[i]);
    }
}

/*
 * CreateColorfulStyle 
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
GtkStyle *CreateColorfulStyle (GdkColor fg, 
                               GdkColor text, 
                               GdkColor bg)
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
        style->fg[i] = fg;
        style->text[i] = text;
        style->bg[i] = bg;
    }

    /* --- All done, here's new style --- */
    return (style);
}


/*
 * RandomStyle
 *
 * Create a random style using the list of colors
 * above.  There is no checking to see if the colors 
 * would be a bad choice.  (like background color is the
 * same as the foreground color)
 */
GtkStyle *RandomStyle ()
{
    GtkStyle *style;

    /* --- Create a randomly colored style --- */
    style = CreateColorfulStyle (
               colorlist[random () % numColors],
               colorlist[random () % numColors],
               colorlist[random () % numColors]);

    return (style);
}

/*
 * CreateFunkyStyles
 *
 * Create styles for the buttons.  Each style has a different set of
 * colors that will be assigned to buttons to make each have a different
 * color.
 */
void CreateFunkyStyles ()
{
    /* --- Red on green --- */
    styleChristmas = CreateColorfulStyle (red, red, green);

    /* --- Orange on black --- */
    styleHalloween = CreateColorfulStyle (orange, orange, black);

    /* --- Green on white --- */
    styleStPatricks = CreateColorfulStyle (green, green, white);

    /* --- red and white on blue --- */
    styleUSIndependence = CreateColorfulStyle (red, white, blue);
}


/*
 * CloseApp
 *
 * Shut down GTK.
 */
void CloseApp (GtkWidget *widget, gpointer gdata)
{
    gtk_main_quit ();
}


/*
 * button_was_clicked
 * 
 * event handler called when the button is clicked.
 */
void ButtonClicked (GtkWidget *widget, gpointer gdata)
{
    GtkStyle *style;

    /* --- Get the style from the callback --- */
    style = (GtkStyle *) gdata;

    /* --- Push colorful new style - now it's the default --- */
    gtk_widget_push_style (style);
   
    /* --- Show dialog using fancy style --- */
    CallButton (widget, (gpointer) 2);

    /* --- Remove the style. --- */
    gtk_widget_pop_style ();
}



/*
 * StyleSet
 *
 * Prints a message when the widget's style is modified.
 */
void StyleSet (GtkWidget *widget, gpointer data)
{

    printf ("Style set\n");
}


/*
 * SetStyleRecursively
 *
 * Set the widget's style to the style (data) and make sure 
 * that all the children (if it's a container) are also set
 * to that particular style.
 */
void SetStyleRecursively (GtkWidget *widget, gpointer data)
{
    GtkStyle *style;

    /* --- Get the style --- */
    style = (GtkStyle *) data;

    /* --- Set the style of the widget --- */
    gtk_widget_set_style (widget, style);

    /* --- If it may have children widgets --- */
    if (GTK_IS_CONTAINER (widget)) {

        /* --- Set all the children's styles too. --- */
        gtk_container_foreach (GTK_CONTAINER (widget), 
                           SetStyleRecursively, style);
    }
}


/*
 * WildButton
 *
 * Create a wildly colored button using the color table we
 * defined.
 */
void WildButton (GtkWidget *widget, gpointer data)
{
    GtkStyle *style;

    /* --- Pick a random style --- */
    style = RandomStyle ();

    /* --- Set the style of the widget --- */
    SetStyleRecursively (widget, (gpointer) style);
}



/*
 * ColorizeButtons
 *
 * Set the color of the children widgets to a wild (and random)
 * style.
 */
void ColorizeButtons (GtkWidget *widget, gpointer data)
{
    /* --- All children get their style reset --- */
    gtk_container_foreach (GTK_CONTAINER (vbox), WildButton, NULL);
}



/*
 * CreateColorDialogButton
 *
 * Create a button in the vbox.  The style is going to be 
 * associated with the button in the callback so that the 
 * dialog created when the button is pressed is in the 
 * style.
 */
void CreateColorDialogButton (GtkWidget *vbox, char *label, GtkStyle *style)
{
    GtkWidget *button;

    /* --- Create a button with a label --- */
    button = gtk_button_new_with_label (label);

    /* --- Setup the style in the callback of the "clicked" --- */
    gtk_signal_connect (GTK_OBJECT (button), "clicked", 
                GTK_SIGNAL_FUNC (ButtonClicked), (gpointer) style);

    /* --- Setup the style in the callback of the "clicked" --- */
    gtk_signal_connect (GTK_OBJECT (button), "style_set", 
                GTK_SIGNAL_FUNC (StyleSet), (gpointer) style);

    /* --- The vbox contains the button. --- */
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

    /* --- Make the button visible --- */
    gtk_widget_show (button);
}



/*
 * CreateColorizeButton
 *
 * Create a button that changes all the button colors.
 */
void CreateColorizeButton (GtkWidget *vbox, char *label)
{
    GtkWidget *button;

    /* --- Create a button --- */
    button = gtk_button_new_with_label (label);

    /* --- Setup the signal handler --- */
    gtk_signal_connect (GTK_OBJECT (button), "clicked", 
                GTK_SIGNAL_FUNC (ColorizeButtons), NULL);

    /* --- The main windows contains the button. --- */
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

    /* --- Make the button visible --- */
    gtk_widget_show (button);
}


/*
 * CreateFlashyButton
 *
 */
void CreateFlashyButton (GtkWidget *vbox, char *label)
{
    GtkStyle *defstyle;
    GtkStyle *style;

    defstyle = gtk_widget_get_default_style ();
    style = gtk_style_copy (defstyle);

    style->fg[GTK_STATE_NORMAL] = purple;
    style->text[GTK_STATE_NORMAL] = purple;
    style->bg[GTK_STATE_NORMAL] = cyan;

    style->fg[GTK_STATE_PRELIGHT] = green;
    style->text[GTK_STATE_PRELIGHT] = green;
    style->bg[GTK_STATE_PRELIGHT] = blue;

    style->fg[GTK_STATE_ACTIVE] = orange;
    style->text[GTK_STATE_ACTIVE] = orange;
    style->bg[GTK_STATE_ACTIVE] = yellow;

    gtk_widget_push_style (style);

    /* --- Create a new button --- */
    button = gtk_button_new_with_label (label);

    /* --- The main windows contains the button. --- */
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

    /* --- Make the button visible --- */
    gtk_widget_show (button);

    /* --- Remove the style so it's not the default style --- */
    gtk_widget_pop_style ();
}


/*
 * main
 *
 * Program begins here
 */
int main (int argc, char *argv[]) 
{
    GtkWidget *window;

    /* --- Initialize gtk, handle command line parameters --- */
    gtk_init (&argc, &argv);

    /* --- Create a window in gtk - window is NOT visible yet. --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_signal_connect (GTK_OBJECT (window), "destroy", 
                GTK_SIGNAL_FUNC (CloseApp), NULL);
    
    /* --- Put some room around the objects in container --- */
    gtk_container_border_width (GTK_CONTAINER (window), 15);

    vbox = gtk_vbox_new (FALSE, 0);

    /* --- Allocate colors --- */
    AllocateColors ();

    /* --- Allocate the styles --- */
    CreateFunkyStyles ();

    /* --- Create buttons --- */
    CreateColorDialogButton (vbox, "Christmas", styleChristmas);
    CreateColorDialogButton (vbox, "St. Patricks", styleStPatricks);
    CreateColorDialogButton (vbox, "Halloween", styleHalloween);
    CreateColorDialogButton (vbox, "US Independence", styleUSIndependence);

    CreateColorizeButton (vbox, "Buttons go wild");

    CreateFlashyButton (vbox, "Flashy Button");

    /* --- Now, make the window visible --- */
    gtk_widget_show (vbox);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (window);

    /* --- This is the event loop in gtk.  Wait for "quit" --- */
    gtk_main ();

    /* --- Exit status code. --- */
    return 0;
}

