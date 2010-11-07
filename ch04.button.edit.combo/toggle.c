/* 
 * File: togglebutton.c
 * Auth: Eric Harlow
 *
 * Example showing toggle button behavior
 */

#include <gtk/gtk.h>

/*
 * CloseAppWindow
 *
 * main window closing, end gtk
 */
gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();

    return (FALSE);
}

/*
 * event happened.
 */
void ButtonEvent (GtkWidget *widget, gpointer *data)
{
    g_print ("Button event: %s\n", data);
}

/*
 * main - begins here.
 */
int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *vbox;
    
    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);
    
    /* --- Create the top level window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title (GTK_WINDOW (window), "Toggle Button");

    /* --- You should always remember to connect the destroy event
     *     to the main window.
     */
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    /* --- Give the window a border --- */
    gtk_container_border_width (GTK_CONTAINER (window), 50);
    
    /* 
     * --- We create a vertical box (vbox) to pack the horizontal boxes into.
     */
    vbox = gtk_vbox_new (FALSE, 0);

    /* 
     * --- Create a button
     */

    /* --- Create a new button. --- */
    button = gtk_toggle_button_new_with_label ("Top Toggle Button");

    /* --- Pack the button into the vertical box (vbox box1). --- */
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

    /* --- Show the button --- */
    gtk_widget_show (button);

    gtk_signal_connect (GTK_OBJECT (button), "toggled",
			GTK_SIGNAL_FUNC (ButtonEvent), "top toggled");
    gtk_signal_connect (GTK_OBJECT (button), "pressed",
			GTK_SIGNAL_FUNC (ButtonEvent), "top pressed");
    gtk_signal_connect (GTK_OBJECT (button), "released",
			GTK_SIGNAL_FUNC (ButtonEvent), "top released");
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
			GTK_SIGNAL_FUNC (ButtonEvent), "top clicked");
    gtk_signal_connect (GTK_OBJECT (button), "enter",
			GTK_SIGNAL_FUNC (ButtonEvent), "top enter");
    gtk_signal_connect (GTK_OBJECT (button), "leave",
			GTK_SIGNAL_FUNC (ButtonEvent), "top leave");

    /* 
     * --- Create another button
     */

    /* --- Create a new button. --- */
    button = gtk_toggle_button_new_with_label ("Bottom Toggle Button");

    /* --- Pack the button into the vertical box (vbox box1).  --- */
    gtk_box_pack_start (GTK_BOX (vbox), button, FALSE, FALSE, 0);

    /* --- Show the button  --- */
    gtk_widget_show (button);

    gtk_signal_connect (GTK_OBJECT (button), "toggled",
			GTK_SIGNAL_FUNC (ButtonEvent), "bottom toggled");
    gtk_signal_connect (GTK_OBJECT (button), "pressed",
			GTK_SIGNAL_FUNC (ButtonEvent), "bottom pressed");
    gtk_signal_connect (GTK_OBJECT (button), "released",
			GTK_SIGNAL_FUNC (ButtonEvent), "bottom released");
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
			GTK_SIGNAL_FUNC (ButtonEvent), "bottom clicked");
    gtk_signal_connect (GTK_OBJECT (button), "enter",
			GTK_SIGNAL_FUNC (ButtonEvent), "bottom enter");
    gtk_signal_connect (GTK_OBJECT (button), "leave",
			GTK_SIGNAL_FUNC (ButtonEvent), "bottom leave");
    /* 
     * --- Make the main window visible
     */ 
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);
    gtk_widget_show (window);

    gtk_main ();
    return (0);
}

