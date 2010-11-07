/*
 * Sample GUI application front end.
 *
 * Auth: Eric Harlow
 * File: frontend.c
 *
 */

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <gtk/gtk.h>

/*
 * --- Function prototypes
 */
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
void Repaint ();


/* 
 * --- Global variables
 */
GtkWidget           *win_main;
GtkTooltips         *tooltips;
GtkAccelGroup       *accel_group;
GtkWidget           *toolbar;

static GtkWidget           *tool_cpu1;
static GtkWidget           *tool_cpu5;
static GtkWidget           *tool_cpu15;
static GtkWidget           *tool_network;
static GtkWidget           *tool_network_avg;


/*
 * --- Bitmap for "network"
 */ 
static const gchar *xpm_network[] = {
"16 16 4 1",
"  c None",
"B c #888888",
"                ",
"                ",
"  BBB           ",
"  BBBBBBB       ",
"  BBB   B       ",
"        B       ",
"        B       ",
"        B       ",
"        B       ",
"        B       ",
"        B       ",
"        B  BBB  ",
"        BBBBBB  ",
"           BBB  ",
"                ",
"                ",
};


/*
 * --- Bitmap for "network"
 */ 
static const gchar *xpm_network_avg[] = {
"16 16 4 1",
"  c None",
"B c #000000",
"                ",
"                ",
"  BBB           ",
"  BBBBBBB       ",
"  BBB   B       ",
"        B       ",
"        B       ",
"        B       ",
"        B       ",
"        B       ",
"        B       ",
"        B  BBB  ",
"        BBBBBB  ",
"           BBB  ",
"                ",
"                ",
};


/*
 * --- Bitmap for "cpu" over last minute
 */
static const char *xpm_cpu1[] = {
"16 16 2 1",
"  c None",
"B c #FF0000",
"                ",
"   B B B B B B  ",
"  BBBBBBBBBBBBB ",
" BB           B ",
"  B     B     BB",
" BB    BB     B ",
"  B     B     BB",
" BB     B     B ",
"  B     B     BB",
" BB     B     B ",
"  B     B     BB",
" BB    BBB    B ",
"  B           BB",
"  BBBBBBBBBBBBB ",
"   B B B B B B  ",
"                ",
};


/*
 * --- Bitmap for "cpu" over 5 minutes
 */
static const char *xpm_cpu5[] = {
"16 16 2 1",
"  c None",
"B c #00FF00",
"                ",
"   B B B B B B  ",
"  BBBBBBBBBBBBB ",
" BB           B ",
"  B   BBBBB   BB",
" BB   B       B ",
"  B   B       BB",
" BB   BBBB    B ",
"  B       B   BB",
" BB       B   B ",
"  B   B   B   BB",
" BB    BBB    B ",
"  B           BB",
"  BBBBBBBBBBBBB ",
"   B B B B B B  ",
"                ",
};


/*
 * --- Bitmap for "cpu" over 15 minutes
 */
static const char *xpm_cpu15[] = {
"16 16 2 1",
"  c None",
"B c #0000FF",
"                ",
"   B B B B B B  ",
"  BBBBBBBBBBBBB ",
" BB           B ",
"  B  B  BBBBB BB",
" BB BB  B     B ",
"  B  B  B     BB",
" BB  B  BBBB  B ",
"  B  B      B BB",
" BB  B      B B ",
"  B  B  B   B BB",
" BB BBB  BBB  B ",
"  B           BB",
"  BBBBBBBBBBBBB ",
"   B B B B B B  ",
"                ",
};


/*
 * RepaintGraph
 *
 * Called when the user toggles buttons on the 
 * toolbar to change the information being displayed
 * in the graph.  Called "Repaint" to force a repaint
 * of the graph.
 */
void RepaintGraph (GtkWidget *widget, gpointer data)
{
    Repaint ();
}

/*
 * NetworkButtonDown
 *
 * Is the network button down on the toolbar.
 */
int NetworkButtonDown ()
{
    return (GTK_TOGGLE_BUTTON (tool_network)->active);
}

/*
 * NetworkAvgButtonDown
 *
 * Is the network button down on the toolbar.
 */
int NetworkAvgButtonDown ()
{
    return (GTK_TOGGLE_BUTTON (tool_network_avg)->active);
}

/*
 * CPUButton1Down
 *
 * Is the CPU-1 button down on the toolbar?
 */
int CPUButton1Down ()
{
    return (GTK_TOGGLE_BUTTON (tool_cpu1)->active);
}

/*
 * CPUButton5Down
 *
 * Is the CPU-5 button down on the toolbar?
 */
int CPUButton5Down ()
{
    return (GTK_TOGGLE_BUTTON (tool_cpu5)->active);
}

/*
 * CPUButton15Down
 *
 * Is the CPU-15 button down on the toolbar?
 */
int CPUButton15Down ()
{
    return (GTK_TOGGLE_BUTTON (tool_cpu15)->active);
}

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
    win_main = gtk_window_new (GTK_WINDOW_TOPLEVEL);
 
    /* --- Don't let the window be resized. --- */
    gtk_window_set_policy (GTK_WINDOW (win_main), FALSE, FALSE, FALSE);

    /* --- Title --- */
    gtk_window_set_title (GTK_WINDOW (win_main), "System monitor");
    gtk_container_border_width (GTK_CONTAINER (win_main), 0);

    /* --- Create accel table --- */
    accel_group = gtk_accel_group_new ();
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

    menuitem = CreateMenuItem (menu, "Quit", "", 
                     "What's more descriptive than quit?", 
                     GTK_SIGNAL_FUNC (EndProgram), "quit");

    /* --- Create the toolbar --- */
    CreateToolbar (vbox_main);

    /* --- Create area to display information --- */
    widget = CreateDrawingArea ();
    gtk_box_pack_start (GTK_BOX (vbox_main), widget, TRUE, TRUE, 0);
}


/*
 * CreateToolbar
 *
 * Create the toolbar buttons that will show or
 * hide items from being graphed in the drawing area.
 */
void CreateToolbar (GtkWidget *vbox_main)
{
    /* --- Create the toolbar and add it to the window --- */
    toolbar = gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL, GTK_TOOLBAR_ICONS);
    gtk_box_pack_start (GTK_BOX (vbox_main), toolbar, FALSE, TRUE, 0);
    gtk_widget_show (toolbar);

    /* --- Show network packet information --- */
    tool_network = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                           GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                           NULL,
                           "Network", "Network", "Network",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_network),
                           (GtkSignalFunc) RepaintGraph,
                           NULL);

    /* --- Show network packet information --- */
    tool_network_avg = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                           GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                           NULL,
                           "Network (avg)", "Network (avg)", "Network (avg)",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_network_avg),
                           (GtkSignalFunc) RepaintGraph,
                           NULL);

    /* --- Little gap --- */
    gtk_toolbar_append_space (GTK_TOOLBAR (toolbar));

    /* --- Show cpu utilization in last minute --- */
    tool_cpu1 = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                           GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                           NULL,
                           "CPU 1", "CPU 1", "CPU 1",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_cpu1),
                           (GtkSignalFunc) RepaintGraph,
                           NULL);

    /* --- Show cpu utilization in last five minutes --- */
    tool_cpu5 = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                           GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                           NULL,
                           "CPU 5", "CPU 5", "CPU 5",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_cpu5),
                           (GtkSignalFunc) RepaintGraph,
                           NULL);

    /* --- Show cpu utilization in last fifteen minutes --- */
    tool_cpu15 = gtk_toolbar_append_element (GTK_TOOLBAR (toolbar),
                           GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
                           NULL,
                           "CPU 15", "CPU 15", "CPU 15",
                           CreateWidgetFromXpm (vbox_main, (gchar **) xpm_cpu15),
                           (GtkSignalFunc) RepaintGraph,
                           NULL);
}


/*
 * main
 * 
 * --- Program begins here
 */
int main(int argc, char *argv[])
{
    gtk_init (&argc, &argv);

    tooltips = gtk_tooltips_new();

    CreateMainWindow ();

    gtk_main();

    return 0;
}
