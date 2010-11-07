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

//
// --- Function prototypes
//
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
GtkWidget *CreateMenuCheck (GtkWidget *menu, 
                            char *szName, 
                            GtkSignalFunc func, 
                            gpointer data);
GtkWidget *CreateSubMenu (GtkWidget *menubar, char *szName);
GtkWidget *CreateBarSubMenu (GtkWidget *menu, char *szName);

// 
// --- Global variables
//
GtkWidget           *menuBold;
GtkWidget           *menuItalics;
GtkWidget           *menuUnderline;
GtkWidget           *win_main;
GtkTooltips         *tooltips;
GtkAcceleratorTable *accelerator_table;
GtkWidget           *toolbar;
GtkWidget           *tool_bold;
GtkWidget           *tool_italics;
GtkWidget           *tool_underline;
GtkWidget           *toolbar;

//
// --- Bitmap for "new"
// 
static const gchar *xpm_new[] = {
"16 16 3 1",
"  c None",
"B c #000000000000",
"W c #FFFFFFFFFFFF",
"                ",
"  BBBBBBBBB     ",
"  BWWWWWWWBB    ",
"  BWWWWWWWBWB   ",
"  BWWWWWWWBBBB  ",
"  BWWWWWWWWWWB  ",
"  BWWWWWWWWWWB  ",
"  BWWWWWWWWWWB  ",
"  BWWWWWWWWWWB  ",
"  BWWWWWWWWWWB  ",
"  BWWWWWWWWWWB  ",
"  BWWWWWWWWWWB  ",
"  BWWWWWWWWWWB  ",
"  BWWWWWWWWWWB  ",
"  BBBBBBBBBBBB  ",
"                ",
};

//
// --- Bitmap for "open"
// 
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

//
// --- Bitmap for "cut"
//
static const char *xpm_cut[] = {
"16 16 2 1",
"  c None",
"B c #000000000000",
"                ",
"                ",
"     B   B      ",
"     B   B      ",
"      B B       ",
"      B B       ",
"       B        ",
"       B        ",
"      B B       ",
"    BBB BBB     ",
"   B  B B  B    ",
"   B  B B  B    ",
"   B  B  BB     ",
"    BB          ",
"                ",
"                ",
};

// ---
// --- Bitmap for "paste"
// ---
static const char *xpm_copy[] = {
"16 16 4 1",
"  c None",
"B c #000000000000",
"W c #FFFFFFFFFFFF",
"G c #666666666666",
"                ",
" BBBBBBB        ",
" BWWWWBWB       ",
" BWGGWBBB       ",
" BWWWWWWB       ",
" BWGGGGWB       ",
" BWWWWBBBBBBB   ",
" BWGGGBWWWWBWB  ",
" BWWWWBWGGWBBB  ",
" BBBBBBWWWWWWB  ",
"      BWGGGGWB  ",
"      BWWWWWWB  ",
"      BWGGGGWB  ",
"      BWWWWWWB  ",
"      BBBBBBBB  ",
"                ",
};

// ---
// --- Bitmap for "bold"
// ---
static const char *xpm_bold[] = {
"16 16 2 1",
"  c None",
"B c #000000000000",
"                ",
" BBBBBBBBBB     ",
"  BBBBBBBBBBB   ",
"  BBBBBBBBBBBB  ",
"  BBB     BBBB  ",
"  BBB      BBB  ",
"  BBB     BBBB  ",
"  BBBBBBBBBBB   ",
"  BBBBBBBBBBB   ",
"  BBB     BBBB  ",
"  BBB      BBB  ",
"  BBB     BBBB  ",
"  BBBBBBBBBBBB  ",
" BBBBBBBBBBBB   ",
" BBBBBBBBBB     ",
"                ",
};

// ---
// --- Bitmap for "italics"
// ---
static const char *xpm_italics[] = {
"16 16 2 1",
"  c None",
"B c #000000000000",
"                ",
"                ",
"     BBBBBBBBBB ",
"         BB     ",
"         BB     ",
"        BB      ",
"        BB      ",
"        BB      ",
"       BB       ",
"       BB       ",
"      BB        ",
"      BB        ",
"      BB        ",
"     BBB        ",
" BBBBBBBBBB     ",
"                ",
};

// ---
// --- Bitmap for "underline"
// ---
static const char *xpm_underline[] = {
"16 16 2 1",
"  c None",
"B c #000000000000",
"                ",
"                ",
"   BBBB  BBBB   ",
"    BB    BB    ",
"    BB    BB    ",
"    BB    BB    ",
"    BB    BB    ",
"    BB    BB    ",
"    BB    BB    ",
"    BB    BB    ",
"    BB    BB    ",
"    BBBBBBBB    ",
"     BBBBBB B   ",
"                ",
"  BBBBBBBBBBBB  ",
"                ",
};


//
// --- External variables
//


//
// EndProgram
//
// Exit from the program
//
gint EndProgram ()
{
    // --- End gtk event loop processing
    gtk_main_quit ();

    // --- Ok to close the app.
    return (FALSE);
}


//
// main
// 
// --- Program begins here
//
int main (int argc, char *argv[])
{
    // --- Initialize GTK
    gtk_init (&argc, &argv);

    // --- Initialize tooltips.
    tooltips = gtk_tooltips_new();

    // --- Create the window with menus/toolbars.
    CreateMainWindow ();

    gtk_main();

    return 0;
}




//
// PrintFunc
// 
// Callback to illustrate when a menu/toolbar item is being selected.
//
void PrintFunc (GtkWidget *widget, gpointer data)
{
    g_print ("%s\n", data);
}


//
// CreateMainWindow
//
// Create the main window and the menu/toolbar associated with it
//
static void CreateMainWindow ()
{
    GtkWidget *vbox_main;
    GtkWidget *menubar;
    GtkWidget *menu;
    GtkWidget *menuitem;
    GtkWidget *menufont;
    GtkWidget *toolbar;
    GtkWidget *button;

    // --- Create the top window and size it.
    win_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_usize(win_main, 360, 260);
    gtk_window_set_title (GTK_WINDOW (win_main), "Menu test");
    gtk_container_border_width (GTK_CONTAINER (win_main), 0);

    // --- Create accel table
    accelerator_table = gtk_accelerator_table_new();
    gtk_window_add_accelerator_table(GTK_WINDOW(win_main), accelerator_table);

    // --- Top level window should listen for the delete_event
    gtk_signal_connect (GTK_OBJECT (win_main), "delete_event",
              GTK_SIGNAL_FUNC (EndProgram), NULL);

    // --- Create v-box for menu, toolbar
    vbox_main = gtk_vbox_new (FALSE, 0);

    // --- Put up v-box
    gtk_container_add (GTK_CONTAINER (win_main), vbox_main);
    gtk_widget_show (vbox_main);
    gtk_widget_show (win_main);
  
    // --- Menu Bar
    menubar = gtk_menu_bar_new ();
    gtk_box_pack_start (GTK_BOX (vbox_main), menubar, FALSE, TRUE, 0);
    gtk_widget_show (menubar);

    // -----------------
    // --- File menu ---
    // -----------------
    menu = CreateBarSubMenu (menubar, "File");

    menuitem = CreateMenuItem (menu, "New", "^N", 
                     "Create a new item", 
                     GTK_SIGNAL_FUNC (PrintFunc), "new");

    menuitem = CreateMenuItem (menu, "Open", "^O", 
                     "Open an existing item", 
                     GTK_SIGNAL_FUNC (PrintFunc), "open");

    menuitem = CreateMenuItem (menu, "Save", "^S", 
                     "Save current item", 
                     GTK_SIGNAL_FUNC (PrintFunc), "save");

    menuitem = CreateMenuItem (menu, "Save As...", "", 
                     "Save current item with new name", 
                     GTK_SIGNAL_FUNC (PrintFunc), "save as");

    menuitem = CreateMenuItem (menu, NULL, NULL, 
                     NULL, NULL, NULL);

    menuitem = CreateMenuItem (menu, "Quit", "", 
                     "What's more descriptive than quit?", 
                     GTK_SIGNAL_FUNC (PrintFunc), "quit");

    // -----------------
    // --- Edit menu ---
    // -----------------
    menu = CreateBarSubMenu (menubar, "Edit");

    menuitem = CreateMenuItem (menu, "Cut", "^X", 
                     "Remove item and place into clipboard", 
                     GTK_SIGNAL_FUNC (PrintFunc), "cut");

    menuitem = CreateMenuItem (menu, "Copy", "^C", 
                     "Place a copy of the item in the clipboard", 
                     GTK_SIGNAL_FUNC (PrintFunc), "copy");

    menuitem = CreateMenuItem (menu, "Paste", "^V", 
                     "Paste item", 
                     GTK_SIGNAL_FUNC (PrintFunc), "paste");

    // ---------------------
    // --- Font sub menu ---
    // ---------------------
    menufont = CreateSubMenu (menu, "Font");

    menuBold = CreateMenuCheck (menufont, 
        "Bold", GTK_SIGNAL_FUNC (SelectMenu), "bold");

    menuItalics = CreateMenuCheck (menufont, 
        "Italics", GTK_SIGNAL_FUNC (SelectMenu), "italics");

    menuUnderline = CreateMenuCheck (menufont, 
        "Underline", GTK_SIGNAL_FUNC (SelectMenu), "underline");

    // --- Create the toolbar
    CreateToolbar (vbox_main);

}



//
// ButtonClicked
//
// If the button on the toolbar was pressed, set the equivalent item in 
// the menu to keep them in sync.  The data has information about which
// item was pressed.
//
void ButtonClicked (GtkWidget *widget, gpointer data)
{
    int nState = GTK_TOGGLE_BUTTON (widget)->active;

    SetMenuButton ((char *) data, nState);
}


//
// SelectMenu
//
// Select the check menu item based on the string (data) being passed
// in to the function.
//
void SelectMenu (GtkWidget *widget, gpointer data)
{
    GtkToggleButton *button = NULL;
    char *szButton;

    // --- Get state of the menu
    int nState = GTK_CHECK_MENU_ITEM (widget)->active;
 
    // --- Show param and button state
    szButton = (char *) data;
    printf ("select menu %s - %d\n", szButton, nState);

    // --- Toggle the toolbar button 

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

//
// SetMenuButton
//
// Based on the string and state, update the menu associated with that
// string to the state.
//
void SetMenuButton (char *szButton, int nState) 
{
    GtkCheckMenuItem *check = NULL;

    printf ("check_menu_set_state - %d\n", nState);

    // --- Set the menu item 

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



//
// CreateToolbar
//
// Create a toolbar.
//
void CreateToolbar (GtkWidget *vbox_main)
{
  GtkWidget *widget;

    // --- Create the toolbar and add it to the window
    toolbar = gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_ICONS);
    gtk_box_pack_start (GTK_BOX (vbox_main), toolbar, FALSE, TRUE, 0);
    gtk_widget_show (toolbar);

    // --- Create "new" button
    gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
			   NULL, "New window", NULL,
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_new),
			   (GtkSignalFunc) ButtonClicked,
                           NULL);

    // --- Create "open" button
    gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
			   "Open Dialog", "Open dialog", "",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_open),
			   (GtkSignalFunc) ButtonClicked,
                           NULL);

    // --- Little gap
    gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));

    // --- Show the Cut button
    gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
			   "Cut", "Cut", "",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_cut),
			   (GtkSignalFunc) ButtonClicked,
                           NULL);
    gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
			   "Paste", "Paste", "",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_copy),
			   (GtkSignalFunc) ButtonClicked,
                           NULL);

    // --- Add a gap
    gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));

    // --- Create a dummy font combobox.
    widget = CreateCombobox ();
    gtk_toolbar_append_widget (GTK_TOOLBAR (toolbar),
                             widget, 
                             "Font", "Pick a font");

    // --- A little gap
    gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));

     //
     // --- Create a toggle button for the Bold
     //
    tool_bold = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                           GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                           NULL,
                           NULL, "Bold", NULL,
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_bold),
			   (GtkSignalFunc) ButtonClicked,
                           "bold");

     //
     // --- Create a toggle button for the Italics
     //
    tool_italics = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                           GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                           NULL,
                           "Italics", "Italics", "Italics",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_italics),
			   (GtkSignalFunc) ButtonClicked,
                           "italics");

     //
     // --- Create a toggle button for the underline
     //
     tool_underline = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                           GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                           NULL,
                           "Underline", "Underline", "Underline",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_underline),
			   (GtkSignalFunc) ButtonClicked,
                           "underline");

}

