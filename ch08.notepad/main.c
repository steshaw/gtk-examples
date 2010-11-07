#include <string.h>
#include <gtk/gtk.h>
#include "notepad.h"

gint ClosingAppWindow (GtkWidget *widget, gpointer data);
char *GetExistingFile ();

/*
 * --- main 
 * 
 * program begins here.
 */
int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *main_vbox;
    
    /* --- Start up GTK --- */
    gtk_init(&argc, &argv);
    
    /* --- Create the top level window --- */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    /* --- Title and border --- */
    gtk_window_set_title (GTK_WINDOW (window), "Gnotepad");
    gtk_container_border_width (GTK_CONTAINER (window), 0);

    /* --- Listen for the main window being closed --- */
    gtk_signal_connect (GTK_OBJECT(window), "delete_event", 
               GTK_SIGNAL_FUNC (ClosingAppWindow), 
               NULL);

    /* --- Set the window title and size --- */
    gtk_widget_set_usize (GTK_WIDGET(window), 200, 200);
    
    /* --- Create a new vertical box for storing widgets --- */
    main_vbox = gtk_vbox_new (FALSE, 1);

    /* --- border width is 1 --- */
    gtk_container_border_width (GTK_CONTAINER(main_vbox), 1);

    /* --- Add the vertical box to the main window --- */
    gtk_container_add (GTK_CONTAINER(window), main_vbox);

    /* --- Make the vertical box visible --- */
    gtk_widget_show (main_vbox);

    /* --- Associate window with xwindow to create pixmaps. --- */
    gtk_widget_realize (window);
    
    /* --- Create the menu/toolbar. --- */
    CreateMenu (window, main_vbox);
    CreateText (window, main_vbox);

    /* --- Show the top level window --- */
    gtk_widget_show (window);

    /* --- Loop and process messages --- */
    gtk_main();
    
    return(0);
}


/*
 * ClosingAppWindow
 */
gint ClosingAppWindow (GtkWidget *widget, gpointer data)
{
    /* --- Shut down gtk --- */
    gtk_exit(0);

    /* --- Ok to close --- */
    return (FALSE);
}

/*
 * menu_Quit
 *
 * If Quit is selected from the menu, exit the
 * program
 */
void menu_Quit (GtkWidget *widget, gpointer data)
{
    gtk_exit(0);
}


/*
 * menu_New
 *
 * Create a new, blank work area
 */
void menu_New (GtkWidget *widget, gpointer data)
{
    ClearFile ();
}



/*
 * menu_ImportRTF
 *
 * Import an RTF file
 */
void menu_ImportRTF (GtkWidget *widget, gpointer data)
{
    GetFilename ("Import", ImportFile);
}


/*
 * menu_Open
 *
 * Give the user a dialog to read a file.
 */
void menu_Open (GtkWidget *widget, gpointer data)
{
    GetFilename ("Open", LoadFile);
}



/*
 * menu_Save
 *
 * Save the existing file in it's filename.  If the file
 * does not have a name, prompt for the name.
 */
void menu_Save (GtkWidget *widget, gpointer data)
{
    char *sFile;

    sFile = GetExistingFile ();

    /* --- Valid filename? --- */
    if (sFile && strlen (sFile)) {

         /* --- Save the file --- */
         SaveFile (sFile);
    } else {

         /* --- Ask for the file and save --- */
         menu_SaveAs (widget, data);
    }
}



/*
 * menu_SaveAs
 *
 * Present a menu and save the file using that
 * filename.
 */
void menu_SaveAs (GtkWidget *widget, gpointer data)
{
    GetFilename ("Save As", SaveFile);
}


/* 
 * menu_Cut
 *
 * Cut selected text to the clipboard.
 */
void menu_Cut (GtkWidget *widget, gpointer data)
{
    TextCut ();
}

/*
 * menu_Copy
 *
 * Copy selected text into the clipboard.
 */
void menu_Copy (GtkWidget *widget, gpointer data)
{
    TextCopy ();
}

/*
 * menu_Paste
 *
 * Paste text from the clipboard into the widget
 */
void menu_Paste (GtkWidget *widget, gpointer data)
{
    TextPaste ();
}

/* 
 * menu_About
 *
 * Show information about the dialog box.
 */
void menu_About ()
{
    ShowAbout ();
}
