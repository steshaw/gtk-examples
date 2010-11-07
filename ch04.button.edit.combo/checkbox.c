/* 
 * File: checkbox.c 
 * Auth: Eric Harlow
 *
 * Show a checkbox.
 */

#include <gtk/gtk.h>

/*
 * CloseAppWindow
 * 
 * Close down the application.
 */
gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();

    return (FALSE);
}


/*
 * CheckEvent
 *
 * Handle a checkbox signal
 */
void CheckEvent (GtkWidget *widget, gpointer *data)
{
    g_print ("checkbox event: %s\n", data);
}


/*
 * CreateCheckBox
 *
 * Add a checkbox to the packing widget.
 */
GtkWidget *CreateCheckBox (GtkWidget *box, char *szLabel)
{
    GtkWidget *check;

    /* --- Get the check button --- */
    check = gtk_check_button_new_with_label (szLabel);

    /* --- Pack the checkbox into the vertical box (box). --- */
    gtk_box_pack_start (GTK_BOX (box), check, FALSE, FALSE, 10);

    /* --- Show the widget --- */
    gtk_widget_show (check);

    return (check);
}


/*
 * main
 */
int main (int argc, char *argv[])
{
    GList *cbitems = NULL;
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *check;
    
    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);
    
    /* --- Create the top level window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- You should always remember to connect the delete_event
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
     * --- Create check buttons
     */
    check = CreateCheckBox (vbox, "Have Linux");
    gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (check), TRUE);
    gtk_signal_connect (GTK_OBJECT (check), "toggled",
			GTK_SIGNAL_FUNC (CheckEvent), "check 1 toggled");
    gtk_signal_connect (GTK_OBJECT (check), "pressed",
			GTK_SIGNAL_FUNC (CheckEvent), "check 1 pressed");
    gtk_signal_connect (GTK_OBJECT (check), "released",
			GTK_SIGNAL_FUNC (CheckEvent), "check 1 released");
    gtk_signal_connect (GTK_OBJECT (check), "clicked",
			GTK_SIGNAL_FUNC (CheckEvent), "check 1 clicked");
    gtk_signal_connect (GTK_OBJECT (check), "enter",
			GTK_SIGNAL_FUNC (CheckEvent), "check 1 enter");
    gtk_signal_connect (GTK_OBJECT (check), "leave",
			GTK_SIGNAL_FUNC (CheckEvent), "check 1 leave");

    check = CreateCheckBox (vbox, "Use Linux");
    gtk_signal_connect (GTK_OBJECT (check), "toggled",
			GTK_SIGNAL_FUNC (CheckEvent), "check 2 toggled");
    gtk_signal_connect (GTK_OBJECT (check), "pressed",
			GTK_SIGNAL_FUNC (CheckEvent), "check 2 pressed");
    gtk_signal_connect (GTK_OBJECT (check), "released",
			GTK_SIGNAL_FUNC (CheckEvent), "check 2 released");
    gtk_signal_connect (GTK_OBJECT (check), "clicked",
			GTK_SIGNAL_FUNC (CheckEvent), "check 2 clicked");
    gtk_signal_connect (GTK_OBJECT (check), "enter",
			GTK_SIGNAL_FUNC (CheckEvent), "check 2 enter");
    gtk_signal_connect (GTK_OBJECT (check), "leave",
			GTK_SIGNAL_FUNC (CheckEvent), "check 2 leave");

    check = CreateCheckBox (vbox, "Like Linux");
    gtk_signal_connect (GTK_OBJECT (check), "toggled",
			GTK_SIGNAL_FUNC (CheckEvent), "check 3 toggled");
    gtk_signal_connect (GTK_OBJECT (check), "pressed",
			GTK_SIGNAL_FUNC (CheckEvent), "check 3 pressed");
    gtk_signal_connect (GTK_OBJECT (check), "released",
			GTK_SIGNAL_FUNC (CheckEvent), "check 3 released");
    gtk_signal_connect (GTK_OBJECT (check), "clicked",
			GTK_SIGNAL_FUNC (CheckEvent), "check 3 clicked");
    gtk_signal_connect (GTK_OBJECT (check), "enter",
			GTK_SIGNAL_FUNC (CheckEvent), "check 3 enter");
    gtk_signal_connect (GTK_OBJECT (check), "leave",
			GTK_SIGNAL_FUNC (CheckEvent), "check 3 leave");

    /* 
     * --- Make the main window visible
     */ 
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);
    gtk_widget_show (window);

    gtk_main ();
    return (0);
}

