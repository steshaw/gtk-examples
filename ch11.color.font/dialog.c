/*
 * Dialog.c
 *
 * Illustration of dialogs at work
 *
 * Auth: Eric Harlow
 *
 */

#include <gtk/gtk.h>

/*
 * CloseDialog
 *
 * Close the dialog window.  The dialog handle is passed
 * in as the data.
 */
void CloseDialog (GtkWidget *widget, gpointer data)
{

    /* --- Remove grab --- */
    gtk_grab_remove (GTK_WIDGET (data));

    /* --- Close it. --- */
    gtk_widget_destroy (GTK_WIDGET (data));

}


/*
 * DisplayYes
 *
 * Show that the "yes" button was pressed
 */
void DisplayYes (GtkWidget *widget, gpointer data)
{
    /* --- Display message --- */
    g_print ("Withdrawing $60,000,000,000.00\n");

    /* --- Close it. --- */
    gtk_widget_destroy (GTK_WIDGET (data));
}


/*
 * DisplayNo
 *
 * Show that the "No" button was pressed
 */
void DisplayNo (GtkWidget *widget, gpointer data)
{
    /* --- Display message --- */
    g_print ("Money can't buy you happiness.\n");

    /* --- Close it. --- */
    gtk_widget_destroy (GTK_WIDGET (data));
}


/*
 * YesNo
 *
 * Function to display a yes/no window
 */
void YesNo (char *szMessage, void (*YesFunc)(), void (*NoFunc)())
{
    GtkWidget *label;
    GtkWidget *button;
    GtkWidget *dialog_window;

    /* --- Create the dialog --- */
    dialog_window = gtk_dialog_new ();

    /* --- Set the title --- */
    gtk_window_set_title (GTK_WINDOW (dialog_window), "YesNo");

    /* --- Add a small border --- */
    gtk_container_border_width (GTK_CONTAINER (dialog_window), 5);

    /*
     * --- Create the message
     */

    /* --- Create a label with the message --- */
    label = gtk_label_new (szMessage);

    /* --- Give the label some room --- */
    gtk_misc_set_padding (GTK_MISC (label), 10, 10);

    /* --- Add label to the dialog --- */
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->vbox), 
                        label, TRUE, TRUE, 0);

    /* --- Show the widget --- */
    gtk_widget_show (label);

    /* 
     * --- Yes button
     */ 
  
    /* --- Create the "yes" button --- */
    button = gtk_button_new_with_label ("Yes");

    gtk_signal_connect (GTK_OBJECT (button), "clicked",
	                GTK_SIGNAL_FUNC (YesFunc),
	                dialog_window);

    /* --- Add the button to the dialog --- */
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area), 
			  button, TRUE, TRUE, 0);

    /* --- Make the button visible --- */
    gtk_widget_show (button);

    /* 
     * --- No button
     */ 

    /* --- Create the "no" button --- */
    button = gtk_button_new_with_label ("No");

    gtk_signal_connect (GTK_OBJECT (button), "clicked",
	                GTK_SIGNAL_FUNC (NoFunc),
	                dialog_window);

    /* --- Allow "no" to be a default --- */
    GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);

    /* --- Add the "no" button to the dialog --- */
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area), 
			  button, TRUE, TRUE, 0);

    /* --- Make the "no" the default --- */
    gtk_widget_grab_default (button);

    /* --- Make the button visible. --- */
    gtk_widget_show (button);

    /* --- Show the dialog --- */
    gtk_widget_show (dialog_window);
}



/*
 * Popup
 *
 * Display a popup dialog window with a message and an 
 * "Ok" button
 */
void Popup (char *szMessage)
{
    static GtkWidget *label;
    GtkWidget *button;
    GtkWidget *dialog_window;

    /* --- Create a dialog window --- */
    dialog_window = gtk_dialog_new ();

    /* --- Trap the destroy button --- */
    gtk_signal_connect (GTK_OBJECT (dialog_window), "destroy",
	                GTK_SIGNAL_FUNC(gtk_widget_destroyed),
	                &dialog_window);

    /* --- Add a title to the window --- */
    gtk_window_set_title (GTK_WINDOW (dialog_window), "Popup");

    /* --- Create a small border --- */
    gtk_container_border_width (GTK_CONTAINER (dialog_window), 5);

    /*
     * --- Create the message
     */

    /* --- Create the message in a label --- */
    label = gtk_label_new (szMessage);

    /* --- Put some room around the label --- */
    gtk_misc_set_padding (GTK_MISC (label), 10, 10);

    /* --- Add the label to the dialog --- */
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->vbox), 
                        label, TRUE, TRUE, 0);

    /* --- Make the label visible --- */
    gtk_widget_show (label);

    /* 
     * --- "ok" button
     */ 

    /* --- Create the "ok" button --- */
    button = gtk_button_new_with_label ("Ok");

    /* --- Need to close the window if they press "ok" --- */
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
	                GTK_SIGNAL_FUNC (CloseDialog),
	                dialog_window);

    /* --- Allow it to be the default button --- */
    GTK_WIDGET_SET_FLAGS (button, GTK_CAN_DEFAULT);

    /* --- Add the button to the dialog --- */
    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog_window)->action_area), 
			  button, TRUE, TRUE, 0);

    /* --- Make the button the default button --- */
    gtk_widget_grab_default (button);

    /* --- Make the button visible --- */
    gtk_widget_show (button);

    /* --- Make the dialog visible --- */
    gtk_widget_show (dialog_window);

    gtk_grab_add (dialog_window);
}



/*
 * CallButton
 *
 * Function to test the dialogs.
 */
void CallButton (GtkWidget *widget, gpointer data)
{
    switch ((int) data) {

        case 1:
            Popup ("This will bankrupt Microsoft.");
            break;

        case 2:
            YesNo ("Are you sure you want to siphon all of"
                   " Bill Gates money from his account?", 
                   DisplayYes, DisplayNo);
            break;
    }
}





