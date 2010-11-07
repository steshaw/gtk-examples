/*
 * File: frontend.c
 * Auth: Eric Harlow
 *
 * Routines to add menus/toolbars as a part of
 * the front end application.
 */

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * --- Function prototypes
 */
char *GetFilename (char *sTitle);
void CreateNotebook (GtkWidget *window);
static void CreateMainWindow ();
void CreateToolbar (GtkWidget *vbox_main);
void SetToolbarButton (char *szButton, int nState);
void SelectMenu (GtkWidget *widget, gpointer data);
void DeSelectMenu (GtkWidget *widget, gpointer data);
void SetMenuButton (char *szButton, int nState) ;
GtkWidget *CreateCombobox ();
GtkWidget *CreateWidgetFromXpm (GtkWidget *window, gchar **xpm_data);
GtkWidget *CreateMenuItem (GtkWidget *menu, 
                           char *szName, 
                           char *szAccel,
                           char *szTip, 
                           GtkSignalFunc func,
                           gpointer data);
GtkWidget *CreateSubMenu (GtkWidget *menubar, char *szName);
GtkWidget *CreateBarSubMenu (GtkWidget *menu, char *szName);
void ParseLog (char *sFile);
void PopulatePages ();
void GeneratePixmaps (GtkWidget *widget);

/* 
 * --- Global variables
 */
GtkWidget           *menuRed;
GtkWidget           *menuBlue;
GtkWidget           *menuGreen;
GtkWidget           *menuColor;
GtkWidget           *menuBold;
GtkWidget           *menuItalics;
GtkWidget           *menuUnderline;
GtkWidget           *win_main;
GtkTooltips         *tooltips;
GtkAccelGroup       *accelerator_table;
GtkWidget           *toolbar;
GtkWidget           *tool_bold;
GtkWidget           *tool_italics;
GtkWidget           *tool_underline;
GtkWidget           *toolbar;

/*
 * --- Bitmap for "open"
 */ 
static const gchar *xpm_open[] = {
"16 16 4 1",
"  c None",
"B c #000000000000",
"Y c #FFFFFFFF0000",
"y c #999999990000",
"                ",
"          BBB   ",
"  BBBBB  B   BB ",
"  BYYYB      BB ",
" BYYYYYBBBBB    ",
" BYYYYYYYYYB    ",
" BYYYYYYYYYB    ",
" BYYYYYYYYYB    ",
" BYYBBBBBBBBBBB ",
" BYYByyyyyyyyyB ",
" BYByyyyyyyyyB  ",
" BYByyyyyyyyyB  ",
" BByyyyyyyyyB   ",
" BByyyyyyyyyB   ",
" BBBBBBBBBBB    ",
"                ",
};


/*
 * --- External variables
 */


/*
 * EndProgram
 *
 * Exit from the program
 */
gint EndProgram ()
{
    /* --- End gtk event loop processing --- */
    gtk_main_quit ();

    /* --- Ok to close the app. --- */
    return (FALSE);
}


/*
 * main
 * 
 * --- Program begins here
 */
int main (int argc, char *argv[])
{
    /* --- Initialize GTK --- */
    gtk_init (&argc, &argv);

    /* --- Initialize tooltips. --- */
    tooltips = gtk_tooltips_new();

    /* --- Create the window with menus/toolbars. --- */
    CreateMainWindow ();

    gtk_main();

    return 0;
}


/*
 * OpenLog
 * 
 * Callback to illustrate when a menu/toolbar item is being selected.
 */
void OpenLog (GtkWidget *widget, gpointer data)
{
    char *sFile;

    sFile = GetFilename ("Open Log file");

    ParseLog (sFile);

    PopulatePages ();
}


/*
 * QuitFunc
 * 
 * Callback to illustrate when a menu/toolbar item is being selected.
 */
void QuitFunc (GtkWidget *widget, gpointer data)
{
    gtk_main_quit ();
}


/*
 * CreateMainWindow
 *
 * Create the main window and the menu/toolbar associated with it
 */
static void CreateMainWindow ()
{
    GtkWidget *vbox_main;
    GtkWidget *menubar;
    GtkWidget *menu;
    GtkWidget *menuitem;

    /* --- Create the top window and size it. --- */
    win_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_usize(win_main, 360, 260);
    gtk_window_set_title (GTK_WINDOW (win_main), "Apache web traffic analyzer");
    gtk_container_border_width (GTK_CONTAINER (win_main), 0);

    /* --- Add accelerator group --- */
    accelerator_table = gtk_accel_group_new ();

    /* --- Top level window should listen for the delete_event --- */
    gtk_signal_connect (GTK_OBJECT (win_main), "delete_event",
              GTK_SIGNAL_FUNC (EndProgram), NULL);

    /* --- Create v-box for menu, toolbar --- */
    vbox_main = gtk_vbox_new (FALSE, 0);

    /* --- Put up v-box --- */
    gtk_container_add (GTK_CONTAINER (win_main), vbox_main);
    gtk_widget_show (vbox_main);
    gtk_widget_show (win_main);
  
    /* --- Menu Bar --- */
    menubar = gtk_menu_bar_new ();
    gtk_box_pack_start (GTK_BOX (vbox_main), menubar, FALSE, TRUE, 0);
    gtk_widget_show (menubar);

    /* -----------------
       --- File menu ---
       ----------------- */
    menu = CreateBarSubMenu (menubar, "File");

    menuitem = CreateMenuItem (menu, "Open", "^O", 
                     "Open an existing item", 
                     GTK_SIGNAL_FUNC (OpenLog), "open");

    menuitem = CreateMenuItem (menu, NULL, NULL, 
                     NULL, NULL, NULL);

    menuitem = CreateMenuItem (menu, "Quit", "", 
                     "What's more descriptive than quit?", 
                     GTK_SIGNAL_FUNC (QuitFunc), "quit");

    /* --- Create the toolbar --- */
    CreateToolbar (vbox_main);

    CreateNotebook (vbox_main);

    GeneratePixmaps (win_main);
}



/*
 * ButtonClicked
 *
 * If the button on the toolbar was pressed, set the equivalent item in 
 * the menu to keep them in sync.  The data has information about which
 * item was pressed.
 */
void ButtonClicked (GtkWidget *widget, gpointer data)
{
    int nState = GTK_TOGGLE_BUTTON (widget)->active;

    SetMenuButton ((char *) data, nState);
}


/*
 * SelectMenu
 *
 * Select the check menu item based on the string (data) being passed
 * in to the function.
 */
void SelectMenu (GtkWidget *widget, gpointer data)
{
    GtkToggleButton *button = NULL;
    char *szButton;

    /* --- Get state of the menu --- */
    int nState = GTK_CHECK_MENU_ITEM (widget)->active;
 
    /* --- Show param and button state--- */
    szButton = (char *) data;
    printf ("select menu %s - %d\n", szButton, nState);

    /* --- Toggle the toolbar button --- */

    if (!strcmp (szButton, "bold")) {
        button = GTK_TOGGLE_BUTTON (tool_bold);
    } else if (!strcmp (szButton, "italics")) {
        button = GTK_TOGGLE_BUTTON (tool_italics);
    } else if (!strcmp (szButton, "underline")) {
        button = GTK_TOGGLE_BUTTON (tool_underline);
    } 
    if (button) {
        gtk_toggle_button_set_state (button, nState);
    }
}

/*
 * SetMenuButton
 *
 * Based on the string and state, update the menu associated with that
 * string to the state.
 */
void SetMenuButton (char *szButton, int nState) 
{
    GtkCheckMenuItem *check = NULL;

    printf ("check_menu_set_state - %d\n", nState);

    /* --- Set the menu item  --- */

    if (!strcmp (szButton, "bold")) {
        check = GTK_CHECK_MENU_ITEM(menuBold);
    } else if (!strcmp (szButton, "italics")) {
        check = GTK_CHECK_MENU_ITEM(menuItalics);
    } else if (!strcmp (szButton, "underline")) {
        check = GTK_CHECK_MENU_ITEM(menuUnderline);
    }
    if (check) {
        gtk_check_menu_item_set_state (check, nState);
    }
}



/*
 * CreateToolbar
 *
 * Create a toolbar.
 */
void CreateToolbar (GtkWidget *vbox_main)
{
    /* --- Create the toolbar and add it to the window --- */
    toolbar = gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_ICONS);
    gtk_box_pack_start (GTK_BOX (vbox_main), toolbar, FALSE, TRUE, 0);
    gtk_widget_show (toolbar);

    /* --- Create "open" button --- */
    gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
			   "Open", "Open", "",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_open),
			   (GtkSignalFunc) OpenLog,
                           NULL);

}

