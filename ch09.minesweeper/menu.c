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
#include "misc.h"

extern void ShowAbout ();
extern void start_button_clicked (GtkWidget *widget, gpointer *data);

void SetGrid (int nColumns, int nRows, int nBombs);


/* 
 * --- Global variables
 */
GtkWidget           *win_main;
GtkAccelGroup *accel_group;
GtkWidget           *toolbar;


/*
 * menu_New
 *
 * Called when the "new" menu is selected. 
 * Creates a new game for the user.
 */
void menu_New (GtkWidget *widget, gpointer data)
{
    /* --- Parameters are not used... --- */

    /* --- Pretend the start button was clicked. --- */
    start_button_clicked (NULL, NULL);
}

/*
 * funcBeginner
 *
 * Picked the "beginner" option from the menu.
 * Create a small grid.
 */
void funcBeginner (GtkWidget *widget, gpointer data)
{
    if (GTK_CHECK_MENU_ITEM (widget)->active) {
        SetGrid (10, 10, 10);
    }
}

/*
 * funcIntermediate
 *
 * Picked the "intermediate" from the menu.
 * Creates a medium sized grid.
 */
void funcIntermediate (GtkWidget *widget, gpointer data)
{
    if (GTK_CHECK_MENU_ITEM (widget)->active) {
        SetGrid (20, 15, 40);
    }
}

/*
 * funcAdvanced
 *
 * User picked the advanced option in the menu.
 * Make the largest grid with the most bombs.
 */
void funcAdvanced (GtkWidget *widget, gpointer data)
{
    /* --- If this item is now active --- */
    if (GTK_CHECK_MENU_ITEM (widget)->active) {

        /* --- Set the grid size --- */
        SetGrid (30, 20, 100);
    }
}

/*
 * menu_Quit
 *
 * Choose quit in the menu. 
 * Exit the game.
 */
void menu_Quit (GtkWidget *widget, gpointer data)
{
    gtk_main_quit ();
}

/*
 * menu_About ()
 *
 * User chose the "about" menu item.
 * Show information about the application.
 */
void menu_About (GtkWidget *widget, gpointer data)
{

    ShowAbout ();
}

/*
 * CreateMainWindow
 *
 * Create the main window and the menu/toolbar associated with it
 */
void CreateMenu (GtkWidget *window, GtkWidget *vbox_main)
{
    GtkWidget *menubar;
    GtkWidget *menu;
    GtkWidget *menuitem;
    GSList *group = NULL;

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
    menu = CreateBarSubMenu (menubar, "Game");

    menuitem = CreateMenuItem (menu, "New", "^N", 
                     "New Game", 
                     GTK_SIGNAL_FUNC (menu_New), NULL);

    menuitem = CreateMenuItem (menu, NULL, NULL, 
                     NULL, NULL, NULL);

    menuitem = CreateMenuRadio (menu, "Beginner", &group,
                     GTK_SIGNAL_FUNC (funcBeginner), NULL);

    menuitem = CreateMenuRadio (menu, "Intermediate", &group,
                     GTK_SIGNAL_FUNC (funcIntermediate), NULL);

    menuitem = CreateMenuRadio (menu, "Advanced", &group,
                     GTK_SIGNAL_FUNC (funcAdvanced), NULL);

    menuitem = CreateMenuItem (menu, NULL, NULL, 
                     NULL, NULL, NULL);

    menuitem = CreateMenuItem (menu, "Quit", "", 
                     "What's more descriptive than quit?", 
                     GTK_SIGNAL_FUNC (menu_Quit), "quit");

    /* -----------------
       --- Help menu ---
       ----------------- */
    menu = CreateBarSubMenu (menubar, "Help");

    menuitem = CreateMenuItem (menu, "About Minesweeper", NULL, 
                     "About the minesweeper", 
                     GTK_SIGNAL_FUNC (menu_About), "about");


}




