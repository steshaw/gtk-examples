/*
 * File: frontend.c
 * Auth: Eric Harlow
 *
 * GUI front end to the molecule viewer.
 *
 */

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <gtk/gtk.h>

/*
 * --- Function prototypes
 */
void GetFilename (char *sTitle, void (*callback) (char *));
void MoleculeRepaint ();
void MoleculeShowLines (int bValue);
void ReadMolecule (char *);
GtkWidget *CreateDrawingArea ();
static void CreateMainWindow ();
void CreateToolbar (GtkWidget *vbox_main);
void SetToolbarButton (char *szButton, int nState);
void SelectMenu (GtkWidget *widget, gpointer data);
void DeSelectMenu (GtkWidget *widget, gpointer data);
void SetMenuButton (char *szButton, int nState) ;
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


/* 
 * --- Global variables
 */
GtkWidget           *win_main;
GtkTooltips         *tooltips;
GtkAccelGroup       *accel_group;
GtkWidget           *toolbar;
GtkWidget           *tool_lines;
GtkWidget           *tool_rgb;
GtkWidget           *tool_labels;


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
 * --- Bitmap for "lines"
 */
static const char *xpm_lines[] = {
"16 16 2 1",
"  c None",
"B c #000000000000",
"                ",
"                ",
"  BB       BB   ",
"  BB       BB   ",
"   B       B    ",
"   B      B     ",
"    B    B      ",
"    B   B       ",
"     B B        ",
"     BB         ",
"     BBBB       ",
"         BB     ",
"           BBB  ",
"            BB  ",
"                ",
"                ",
};

/*
 * --- Bitmap for "color"
 */
static const char *xpm_rgb[] = {
"16 16 4 1",
"  c None",
"R c #FF0000",
"G c #00FF00",
"B c #0000FF",
"                ",
"     BBBRRR     ",
"   BBBBBRRRRR   ",
"  BBBBBBRRRRRR  ",
"  BBBBBBRRRRRR  ",
"  BBBBBBRRRRRR  ",
" BBBBBBBRRRRRRR ",
" BBBBBBBRRRRRRR ",
" BBBBBGGGGRRRRR ",
" BBBGGGGGGGGRRR ",
"  GGGGGGGGGGGG  ",
"  GGGGGGGGGGGG  ",
"  GGGGGGGGGGGG  ",
"   GGGGGGGGGG   ",
"     GGGGGGG    ",
"                ",
};


/*
 * --- Bitmap for "Labels"
 */
static const char *xpm_labels[] = {
"16 16 4 1",
"  c None",
"R c #FF0000",
"G c #00FF00",
"B c #000000",
"                ",
"       BB       ",
"     BBBBBB     ",
"    BBB  BBB    ",
"   BB      BB   ",
"   BB      BB   ",
"  BB        BB  ",
"  BB        BB  ",
"  BBBBBBBBBBBB  ",
"  BBBBBBBBBBBB  ",
"  BB        BB  ",
"  BB        BB  ",
"  BB        BB  ",
"  BB        BB  ",
"  BB        BB  ",
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
void EndProgram ()
{
    gtk_main_quit ();
}


/*
 * main
 * 
 * --- Program begins here
 */
int main(int argc, char *argv[])
{
    /* --- Initialize GTK --- */
    gtk_init (&argc, &argv);

    /* --- Initialize tooltips. --- */
    tooltips = gtk_tooltips_new ();

    /* --- Create window(s) --- */
    CreateMainWindow ();

    /* --- Read in the default molecule --- */
    ReadMolecule ("molecule.pdb");

    /* --- Main event handling loop. --- */
    gtk_main();

    return 0;
}



/*
 * ShowLabels
 *
 * Is the "Show Labels" toolbar button pressed in to
 * indicate that the user wanted to display the 
 * atom labels?
 */
int ShowLabels ()
{

    return (GTK_TOGGLE_BUTTON (tool_labels)->active);
}

/*
 * ShowRGB
 *
 * Is the "Show RGB" button on the toolbar pressed in?
 * This indicates whether the atoms should be displayed
 * in single color or multicolor mode.
 */
int ShowRGB ()
{

    return (GTK_TOGGLE_BUTTON (tool_rgb)->active);
}


/*
 * ShowLines
 *
 * Indicates whether the lines should be drawn between
 * the atoms to show the bonds.
 */
int ShowLines ()
{

    return (GTK_TOGGLE_BUTTON (tool_lines)->active);
}


/*
 * FileOpen
 * 
 * Open a pdb file for displaying
 */
void FileOpen (GtkWidget *widget, gpointer data)
{
    GetFilename ("Open Molecule", ReadMolecule);
}



/*
 * CreateMainWindow
 *
 * Create the main window and the menu/toolbar associated with it
 */
static void CreateMainWindow ()
{
    GtkWidget *widget;
    GtkWidget *vbox_main;
    GtkWidget *menubar;
    GtkWidget *menu;
    GtkWidget *menuitem;

    /* --- Create the top window and size it. --- */
    win_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_usize(win_main, 360, 260);
    gtk_window_set_title (GTK_WINDOW (win_main), "Molecule Viewer");
    gtk_container_border_width (GTK_CONTAINER (win_main), 0);

    /* --- Create accel table --- */
    accel_group = gtk_accel_group_new();
    gtk_accel_group_attach (accel_group, GTK_OBJECT (win_main));

    /* --- Top level window should listen for the destroy --- */
    gtk_signal_connect (GTK_OBJECT (win_main), "destroy",
              GTK_SIGNAL_FUNC(EndProgram), NULL);

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
                     GTK_SIGNAL_FUNC (FileOpen), "open");

    menuitem = CreateMenuItem (menu, NULL, NULL, 
                     NULL, NULL, NULL);

    menuitem = CreateMenuItem (menu, "Quit", "", 
                     "What's more descriptive than quit?", 
                     GTK_SIGNAL_FUNC (EndProgram), "quit");

    /* --- Create the toolbar --- */
    CreateToolbar (vbox_main);

    widget = CreateDrawingArea ();
    gtk_box_pack_start (GTK_BOX (vbox_main), widget, TRUE, TRUE, 0);
}




/*
 * CreateToolbar
 *
 * Create the toolbar with the specified toolbar 
 * options.
 */
void CreateToolbar (GtkWidget *vbox_main)
{

    /* --- Create the toolbar and add it to the window --- */
    toolbar = gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_ICONS);
    gtk_box_pack_start (GTK_BOX (vbox_main), toolbar, FALSE, TRUE, 0);
    gtk_widget_show (toolbar);

    /* --- Create "open" button --- */
    gtk_toolbar_append_item (GTK_TOOLBAR (toolbar),
			   "Open Dialog", "Open dialog", "",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_open),
			   (GtkSignalFunc) FileOpen,
                           NULL);

    /* --- Little gap --- */
    gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));


    tool_lines = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                           GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                           NULL,
                           "Lines", "Lines", "Lines",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_lines),
                           (GtkSignalFunc) MoleculeRepaint,
                           NULL);

    tool_rgb = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                           GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                           NULL,
                           "Color", "Color", "Color",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_rgb),
                           (GtkSignalFunc) MoleculeRepaint,
                           NULL);

    tool_labels = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                           GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                           NULL,
                           "Labels", "Labels", "Labels",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_labels),
                           (GtkSignalFunc) MoleculeRepaint,
                           NULL);


}

