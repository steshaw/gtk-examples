/*
 * Sample GUI application front end.
 *
 * Auth: Eric Harlow
 *
 */

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <gtk/gtk.h>
#include "notepad.h"


/* 
 * --- Global variables
 */
GtkWidget           *win_main;
GtkAccelGroup       *accel_group;
GtkWidget           *toolbar;

/*
 * --- Bitmap for "new"
 */ 
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
 * --- Bitmap for "cut"
 */
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

/*
 * --- Bitmap for "copy"
 */
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


/*
 * --- Bitmap for "paste"
 */
static const char *xpm_paste[] = {
"16 16 5 1",
"  c None",
"B c #000000000000",
"W c #FFFFFFFFFFFF",
"G c #666666666666",
"Y c #888888880000",
"                ",
"       BB       ",
"  BBBBBYYBBBBB  ",
"  BYYBBBBBBYYB  ",
"  BYYYYYYYYYYB  ",
"  BYYYYYYYYYYB  ",
"  BYYYYBBBBBBBB ",
"  BYYYYBWWWWBWB ",
"  BYYYYBWGGWBBB ",
"  BYYYYBWWWWWWB ",
"  BYYYYBWGGGGWB ",
"  BYYYYBWWWWWWB ",
"  BYYYYBWGGGGWB ",
"  BBBBBBWWWWWWB ",
"       BBBBBBBB ",
"                ",
};


/*
 * CreateMenu
 *
 * Create the main window and the menu/toolbar associated with it
 */
void CreateMenu (GtkWidget *window, GtkWidget *vbox_main)
{
    GtkWidget *menubar;
    GtkWidget *menu;
    GtkWidget *menuitem;

    win_main = window;

    /* --- Create accel table --- */
    accel_group = gtk_accel_group_new ();
    gtk_accel_group_attach (accel_group, GTK_OBJECT (window));

    /* --- Menu Bar --- */
    menubar = gtk_menu_bar_new ();
    gtk_box_pack_start (GTK_BOX (vbox_main), menubar, FALSE, TRUE, 0);
    gtk_widget_show (menubar);

    /* -----------------
       --- File menu ---
       ----------------- */
    menu = CreateBarSubMenu (menubar, "File");

    menuitem = CreateMenuItem (menu, "New", "^N", 
                     "Create a new item", 
                     GTK_SIGNAL_FUNC (menu_New), "new");

    menuitem = CreateMenuItem (menu, "Open", "^O", 
                     "Open an existing item", 
                     GTK_SIGNAL_FUNC (menu_Open), "open");

    menuitem = CreateMenuItem (menu, "Import RTF", "", 
                     "Import RTF file", 
                     GTK_SIGNAL_FUNC (menu_ImportRTF), "import rtf");

    menuitem = CreateMenuItem (menu, "Save", "^S", 
                     "Save current item", 
                     GTK_SIGNAL_FUNC (menu_Save), "save");

    menuitem = CreateMenuItem (menu, "Save As...", "", 
                     "Save current item with new name", 
                     GTK_SIGNAL_FUNC (menu_SaveAs), "save as");

    menuitem = CreateMenuItem (menu, NULL, NULL, 
                     NULL, NULL, NULL);

    menuitem = CreateMenuItem (menu, "Quit", "", 
                     "What's more descriptive than quit?", 
                     GTK_SIGNAL_FUNC (menu_Quit), "quit");

    /* -----------------
       --- Edit menu ---
       ----------------- */
    menu = CreateBarSubMenu (menubar, "Edit");

    menuitem = CreateMenuItem (menu, "Cut", "^X", 
                     "Remove item and place into clipboard", 
                     GTK_SIGNAL_FUNC (menu_Cut), "cut");

    menuitem = CreateMenuItem (menu, "Copy", "^C", 
                     "Place a copy of the item in the clipboard", 
                     GTK_SIGNAL_FUNC (menu_Copy), "copy");

    menuitem = CreateMenuItem (menu, "Paste", "^V", 
                     "Paste item", 
                     GTK_SIGNAL_FUNC (menu_Paste), "paste");

   
    /* ---------------------
       --- Find sub menu ---
       --------------------- */
    menu = CreateBarSubMenu (menubar, "Search");

    menuitem = CreateMenuItem (menu, "Find", "^F", 
                     "Find item", 
                     GTK_SIGNAL_FUNC (menu_Find), "paste");

    /* ---------------------
       --- Help sub menu ---
       --------------------- */
    menu = CreateBarSubMenu (menubar, "Help");

    menuitem = CreateMenuItem (menu, "About", "", 
                     "About", 
                     GTK_SIGNAL_FUNC (menu_About), "about");

    /* --- Create the toolbar --- */
    CreateToolbar (vbox_main);
}


/*
 * CreateToolbar
 * 
 * Create the toolbar for commonly used options
 */
void CreateToolbar (GtkWidget *vbox_main)
{

    /* --- Create the toolbar and add it to the window --- */
    toolbar = gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_ICONS);
    gtk_box_pack_start (GTK_BOX (vbox_main), toolbar, FALSE, TRUE, 0);
    gtk_widget_show (toolbar);

    /*            -------
     * --- Create | new | button
     *            -------
     */
    gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
			   NULL, "New window", NULL,
                           CreateWidgetFromXpm (win_main, (gchar **) xpm_new),
			   (GtkSignalFunc) menu_New,
                           NULL);

    /*            --------
     * --- Create | open | button
     *            --------
     */
    gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
			   "Open Dialog", "Open dialog", "",
                           CreateWidgetFromXpm (win_main, (gchar **) xpm_open),
			   (GtkSignalFunc) menu_Open,
                           NULL);

    /* --- Little gap --- */
    gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));

    /*            -------
     * --- Create | cut | button
     *            -------
     */
    gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
			   "Cut", "Cut", "",
                           CreateWidgetFromXpm (win_main, (gchar **) xpm_cut),
			   (GtkSignalFunc) menu_Cut,
                           NULL);

    /*            --------
     * --- Create | copy | button
     *            --------
     */
    gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
			   "Copy", "Copy", "",
                           CreateWidgetFromXpm (win_main, (gchar **) xpm_copy),
			   (GtkSignalFunc) menu_Copy,
                           NULL);

    /*            ---------
     * --- Create | paste | button
     *            ---------
     */
    gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
			   "Paste", "Paste", "",
                           CreateWidgetFromXpm (win_main, (gchar **) xpm_paste),
			   (GtkSignalFunc) menu_Paste,
                           NULL);


}

