/* 
 * File: radio.c 
 * Auth: Eric Harlow
 *
 * Radio button example.
 */

#include <gtk/gtk.h>

/*
 * Destroy
 *
 * End the gtk application
 */
void Destroy (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();
}


/*
 * RadioEvent
 *
 * Radio event button occurred.
 */
void RadioEvent (GtkWidget *widget, gpointer *data)
{
    g_print ("radio event: %s\n", data);
}


/*
 * CreateRadio
 *
 * Create a radio button and add it to the packing box
 *
 * box - packing box.
 * group - group to add the box to.
 * szLabel - Label to put by the radio button.
 */
GtkWidget *CreateRadio (GtkWidget *box, GSList **group, char *szLabel)
{
    GtkWidget *radio;

    /* --- Get the radio button --- */
    radio = gtk_radio_button_new_with_label (*group, szLabel);

    *group = gtk_radio_button_group (GTK_RADIO_BUTTON (radio));

    /* --- Pack the radio button into the vertical box (box).  --- */
    gtk_box_pack_start (GTK_BOX (box), radio, FALSE, FALSE, 10);

    /* --- Show the widget --- */
    gtk_widget_show (radio);

    return (radio);
}

/*
 * main
 *
 */
int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *radio;
    GSList *group = NULL;
    
    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);
    
    /* --- Create the top level window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- Set the title --- */
    gtk_window_set_title (GTK_WINDOW (window), "Radio buttons");

    /* --- You should always remember to connect the destroy event
     *     to the main window.
     */
    gtk_signal_connect (GTK_OBJECT (window), "destroy",
			GTK_SIGNAL_FUNC (Destroy), NULL);

    /* --- Give the window a border --- */
    gtk_container_border_width (GTK_CONTAINER (window), 50);
    
    /* 
     * --- We create a vertical box (vbox) to pack the horizontal boxes into.
     */
    vbox = gtk_vbox_new (FALSE, 0);


    /* 
     * --- Create radio buttons
     */
    radio = CreateRadio (vbox, &group, "Have Linux");

    /* --- Listen for events. --- */
    gtk_signal_connect (GTK_OBJECT (radio), "toggled",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 1 toggled");
    gtk_signal_connect (GTK_OBJECT (radio), "pressed",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 1 pressed");
    gtk_signal_connect (GTK_OBJECT (radio), "released",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 1 released");
    gtk_signal_connect (GTK_OBJECT (radio), "clicked",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 1 clicked");
    gtk_signal_connect (GTK_OBJECT (radio), "enter",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 1 enter");
    gtk_signal_connect (GTK_OBJECT (radio), "leave",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 1 leave");

    /* --- Create another radio button --- */
    radio = CreateRadio (vbox, &group, "Use Linux");

    /* --- Listen for events. --- */
    gtk_signal_connect (GTK_OBJECT (radio), "toggled",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 2 toggled");
    gtk_signal_connect (GTK_OBJECT (radio), "pressed",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 2 pressed");
    gtk_signal_connect (GTK_OBJECT (radio), "released",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 2 released");
    gtk_signal_connect (GTK_OBJECT (radio), "clicked",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 2 clicked");
    gtk_signal_connect (GTK_OBJECT (radio), "enter",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 2 enter");
    gtk_signal_connect (GTK_OBJECT (radio), "leave",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 2 leave");

    /* --- Button is set --- */
    gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (radio), TRUE);


    /* --- Create radio button --- */
    radio = CreateRadio (vbox, &group, "Like Linux");

    /* --- Listen for events. --- */
    gtk_signal_connect (GTK_OBJECT (radio), "toggled",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 3 toggled");
    gtk_signal_connect (GTK_OBJECT (radio), "pressed",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 3 pressed");
    gtk_signal_connect (GTK_OBJECT (radio), "released",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 3 released");
    gtk_signal_connect (GTK_OBJECT (radio), "clicked",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 3 clicked");
    gtk_signal_connect (GTK_OBJECT (radio), "enter",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 3 enter");
    gtk_signal_connect (GTK_OBJECT (radio), "leave",
			GTK_SIGNAL_FUNC (RadioEvent), "radio 3 leave");


    /* 
     * --- Make the main window visible
     */ 
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);
    gtk_widget_show (window);

    gtk_main ();
    exit (0);

}


