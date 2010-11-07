/*
 * File: Minesweeper.c
 * Auth: Eric Harlow
 *
 * Similar to the windows minesweeper game.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <gtk/gtk.h>
#include "misc.h"
#include "bitmaps.h"
#include "digits.h"

extern void StartTimer ();
extern void StopTimer ();

extern void CreateMenu (GtkWidget *window, GtkWidget *vbox_main);

void SetStartButtonIcon (gchar **xpm_data);

/*
 * --- Want the buttons to be a particular size.
 */
#define BUTTON_WIDTH 18
#define BUTTON_HEIGHT 19


/* 
 * --- Each of the digits showing the number of bombs
 *     has a colorful bitmap showing the count.  This
 *     array make the looking up of the digits happen
 *     very quickly.
 */
gpointer digits[] = { 
    NULL, 
    xpm_one, 
    xpm_two,
    xpm_three,
    xpm_four,
    xpm_five,
    xpm_six,
    xpm_seven,
    xpm_eight
};



/*
 * --- These are the available button states.  
 *
 * Unknown - Empty.  Don't know what it could be.
 * Flagged - User has put a flag on it, suspecting
 *           there's a bomb underneath.
 * Question - not implemented.
 * Down - Button has been pressed.
 */
enum {
    BUTTON_UNKNOWN,
    BUTTON_FLAGGED,
    BUTTON_QUESTION,
    BUTTON_DOWN
};

/*
 * --- data structure to keep track of the minesweeper buttons.
 */
typedef struct {
    int       buttonState;  /* --- Column to place the button --- */
    GtkWidget *widget;      /* --- Handle to the button --- */
    int       nBombsNearby; /* --- How many are near this cell? --- */
    int       bHasBomb;     /* --- Does it have a bomb? --- */
    int       nRow;         /* --- Row of button --- */
    int       nCol;         /* --- Column of button --- */
} typMinesweeperButton;

/* 
 * --- Default values for the size of the grid 
 *     and number of bombs.
 */
static int nRows = 10;
static int nCols = 10;
static int nTotalBombs = 10;


/*
 * --- Use a 2-d array for the grid.  This is just
 *     the maximum size. 
 */
#define MAX_ROWS 35
#define MAX_COLS 35
/*
 * Globals.
 */

/* --- Just allocate the MAX for now.  This really should be dynamic, 
 *     but using a two dimensional array is easy when dealing with a grid.
 */
typMinesweeperButton mines[MAX_COLS][MAX_ROWS];

/* --- Flags for the game --- */
int bGameOver = FALSE;            /* --- Game is over? --- */
int bResetGame = FALSE;           /* --- Game is over? --- */
int nBombsLeft;                   /* --- Undiscovered bombs --- */

GtkWidget *table = NULL;          /* --- Table with the grid in it. --- */
GtkWidget *button_start;          /* --- Start button --- */
GtkWidget *label_bombs;           /* --- Label showing # of bombs left --- */
GtkWidget *label_time;            /* --- Label showing # of bombs left --- */
GtkWidget *vbox;                  /* --- main window's vbox --- */

/*
 * --- Prototypes.
 */
void DisplayHiddenInfo (typMinesweeperButton *mine);
void CreateMinesweeperButtons (GtkWidget *table, int c, int r, int flag);
void FreeChildCallback (GtkWidget *widget);



/*
 * CheckForWin
 *
 * Check to see if the game has been won.  Game has been won when the 
 * number of unclicked squares equals the number of total bombs. 
 */
void CheckForWin ()
{
    int i, j;
    int nMines = 0;

    /* --- Run through all the squares --- */
    for (i = 0; i <  nCols; i++) {
        for (j = 0; j < nRows; j++) {

            /* --- If square is unclicked or has a flag on it... --- */
            if (mines[i][j].buttonState == BUTTON_UNKNOWN ||
                mines[i][j].buttonState == BUTTON_FLAGGED) {

                /* --- Could be a bomb. --- */
                nMines ++;
            }
        }
    }

    /* --- As many bombs as there are squares left? --- */
    if (nMines == nTotalBombs) {

        /* --- Stop the game.  We have a winner. --- */
        StopTimer ();
        SetStartButtonIcon ((gchar **) xpm_winner);
        bGameOver = TRUE;
    }
}


/*
 * AddImageToMine
 *
 * Change the image on the button to be that of the xpm.
 *
 * mine - square on the grid
 * xpm - image data
 */
void AddImageToMine (typMinesweeperButton *mine, char *xpm[])
{
    GtkWidget *widget;

    /* --- Create a widget from the xpm file --- */
    widget = CreateWidgetFromXpm (table, (gchar **) xpm);

    /* --- Put the bitmap in the button --- */
    gtk_container_add (GTK_CONTAINER (mine->widget), widget);

#if 0
    /* --- deref image so when button is destroyed, image is destroyed. --- */
    // NOTE: No longer appears necessary.
    gdk_pixmap_unref ((GdkPixmap *) widget);
#endif
}


/*
 * UpdateSeconds
 *
 * Refresh the seconds display in the toolbar.
 *
 * nSeconds - how many seconds to display.
 */
void UpdateSeconds (int nSeconds)
{
    char buffer[44];

    /* --- Change the label to show new time --- */
    sprintf (buffer, "Time: %d", nSeconds);
    gtk_label_set (GTK_LABEL (label_time), buffer);
}


/*
 * DisplayBombCount
 *
 * Show number of bombs remaining.
 */
void DisplayBombCount ()
{
    char buffer[33];

    /* --- You have XX bombs left --- */
    sprintf (buffer, "Bombs: %d", nBombsLeft);
    gtk_label_set (GTK_LABEL (label_bombs), buffer);
}


/*
 * FreeChild
 *
 * Free all the children of the widget
 * This is called when the button has to display a new image. 
 * The old image is destroyed here.
 */
void FreeChild (GtkWidget *widget) 
{
    /* --- Free button children --- */
    gtk_container_foreach (
               GTK_CONTAINER (widget), 
               (GtkCallback) FreeChildCallback,
               NULL);

}

/*
 * delete_event
 *
 * The window is closing down, end the gtk loop
 *
 */
void delete_event (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();
}


/*
 * ShowBombs
 *
 * They clicked on a bomb, so now we have to show them were the
 * bombs really are.  (At least those they didn't find already.)
 * We display the bombs they didn't find as well as the bombs
 * they think they found that were not bombs.
 */ 
void ShowBombs (typMinesweeperButton *minefound)
{
    int i, j;
    typMinesweeperButton *mine; 

    /* --- Run through all the squares --- */
    for (i = 0; i <  nCols; i++) {
        for (j = 0; j < nRows; j++) {

            /* --- Get the datastructure --- */
            mine = &mines[i][j];

            /* --- If there's a button here and there's a bomb --- */
            /* --- underneath --- */
            if (mine->buttonState == BUTTON_UNKNOWN &&
                mine->bHasBomb) {

                /* --- Display the bomb. --- */
                DisplayHiddenInfo (mine);

            /* --- If they marked it as a bomb and there is  
             *     no bomb here... --- */
            } else if (mine->buttonState == BUTTON_FLAGGED &&
                       !mine->bHasBomb) {

                /* --- Free the flag --- */
                FreeChild (mine->widget);

                /* --- Show the X at the location --- */
                AddImageToMine (mine, xpm_bigx);
            }
        }
    }
}


/*
 * OpenNearbySquares
 *
 * Open up all squares around this square.
 *
 * col, row - position to open up the square 
 * Open all squares near this one - X represents
 * the current square.
 *
 *      |---|---|----
 *      |   |   |   |
 *      -------------
 *      |   | X |   |
 *      -------------
 *      |   |   |   |
 *      |---|---|----
 */
void OpenNearbySquares (int col, int row)
{
    int i, j;

    /* --- Look one column back and one column ahead --- */
    for (i = MAX (col-1, 0); i <= MIN (col+1, nCols-1); i++) {

        /* --- Check one row back and one row ahead --- */
        for (j = MAX (row-1, 0); j <= MIN (row+1, nRows-1); j++) {

            /* --- Display what's underneath --- */
            DisplayHiddenInfo (&mines[i][j]);
        }
    }
}


/*
 * DisplayHiddenInfo
 *
 * Display what's hidden underneath the button.
 * Could be a bomb - 
 * Could be a square with a count of the bombs
 *  nearby.
 * Could be an empty square
 */
void DisplayHiddenInfo (typMinesweeperButton *mine)
{
    /* --- If it's already down, just return --- */
    if (mine->buttonState == BUTTON_DOWN) {
        gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (mine->widget), TRUE);
        return;
    }

    /* --- If the button is flagged, don't fix it for them --- */
    if (mine->buttonState == BUTTON_FLAGGED) {

        /* --- They said there's a bomb here - so don't  
         *     open it up even if logically, there 
         *     can't be a bomb.                    
         */
        gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (mine->widget), FALSE);

    } else {

        /* --- Put the button in the "down" state --- */
        mine->buttonState = BUTTON_DOWN;
        gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (mine->widget), TRUE);

        /* --- If there's a bomb at the location --- */
        if (mine->bHasBomb) {

            /* --- Show the bomb at the location --- */
            AddImageToMine (mine, xpm_bomb);

        /* --- No bombs, but there are bombs near this one --- */
        } else if (mine->nBombsNearby) {

            /* --- Show the count of nearby bombs. --- */
            AddImageToMine (mine, digits[mine->nBombsNearby]);

        } else {

            /* --- Hmm.  Clicked here, but no bombs and no count. --- */
            /*     Open up all squares near here - may cascade. --- */
            OpenNearbySquares (mine->nCol, mine->nRow);
        }
    }
}


/*
 * ResetGame
 *
 * Reset the game so it can be replayed.  Reset bomb
 * count and create a nice empty field of bombs.
 */
void ResetGame (int nGridColumns, int nGridRows, int nBombs, int bNewButtons)
{

    /* --- Reset the number of bombs in grid  --- */
    nTotalBombs = nBombs;

    /* --- Reset the number of bombs undiscovered --- */
    nBombsLeft = nBombs;

    /* --- Create the Minesweeper buttons. --- */
    CreateMinesweeperButtons (table, nGridColumns, nGridRows, bNewButtons);

    /* --- Stop the timer. --- */
    StopTimer ();

    UpdateSeconds (0);

    SetStartButtonIcon ((gchar **) xpm_smile);
}


/*
 * FreeChildCallback
 *
 * Free the widget.
 */
void FreeChildCallback (GtkWidget *widget)
{
    gtk_widget_destroy (widget);
}


/*
 * SetStartButtonIcon
 *
 * Set the start button to have the image based on 
 * the data passed in.  Usually, this is going to be 
 * either the happy face or the frown.
 */
void SetStartButtonIcon (gchar **xpm_data)
{
    GtkWidget *widget;

    /* --- Create a widget from the xpm --- */
    widget = CreateWidgetFromXpm (button_start, xpm_data);

    /* --- Free any children the button has --- */
    FreeChild (button_start);

    /* --- Make this the current image --- */
    gtk_container_add (GTK_CONTAINER (button_start), widget);
}


/*
 * start_button_clicked
 *
 * Event handler for the clicking of the start 
 * button
 */
void start_button_clicked (GtkWidget *widget, gpointer *data)
{
    SetStartButtonIcon ((gchar **) xpm_smile);
    ResetGame (nCols, nRows, nTotalBombs, FALSE);
}


/*
 * mine_button_clicked
 *
 * Event handler for one of the mine buttons being
 * clicked.
 *
 * widget - button pressed.
 * data - button label.
 */
void mine_button_clicked (GtkWidget *widget, gpointer *data)
{
    typMinesweeperButton *mine;

    mine = (typMinesweeperButton *) data;

    //printf ("button clicked - game over = %d\n", bGameOver);
    /* --- If the game is over --- */
    if (bGameOver) {

        /* --- Let the button as it was. --- */
        gtk_toggle_button_set_state (GTK_TOGGLE_BUTTON (widget),
                            (mine->buttonState == BUTTON_DOWN));
        return;
    }

    /* --- If game is being reset --- */
    if (bResetGame) return;

    /* --- Start the time ... now --- */
    StartTimer ();

    /* --- If they clicked on a mine button... --- */
    if (mine->bHasBomb) {

        /* --- Game over, dude! --- */
        bGameOver = TRUE;

        /* --- Smiley face is frowning. --- */
        SetStartButtonIcon ((gchar **) xpm_frown);

        /* --- Display all unseen bombs. --- */
        StopTimer ();
        ShowBombs (mine);

    } else {

        /* --- Create a label for the cell and put the  --- */
        /* --- number of nearby bombs in it. --- */
        DisplayHiddenInfo (mine);
        CheckForWin ();
    }
}


/*
 * button_press
 *
 * The button press could be a right mouse button
 * which needs to be handled by rotating the 
 * image in the button.
 */
void button_press (GtkWidget *widget, GdkEventButton *event, gpointer *data)
{
    typMinesweeperButton *mine;

    /* --- Ignore if game is already over with --- */
    if (bGameOver) {
        return;
    }

    /* --- Which mine was clicked? --- */
    mine = (typMinesweeperButton *) data;

    /* --- Make sure it's a button event --- */
    if (event->type == GDK_BUTTON_PRESS) {
 
        /* --- Was it the right mouse button? --- */
        if (event->button == 3) {

            switch (mine->buttonState) {

                case BUTTON_UNKNOWN:

                    /* --- Free button children --- */
                    FreeChild (widget);

                    mine->buttonState = BUTTON_FLAGGED;
                    AddImageToMine (mine, xpm_flag);
                    nBombsLeft --;
                    break;

                case BUTTON_FLAGGED:

                    /* --- Free button children --- */
                    FreeChild (widget);

                    /* --- Free button children --- */
                    mine->buttonState = BUTTON_UNKNOWN;
                    nBombsLeft ++;
                    break;
            }
            DisplayBombCount ();
            CheckForWin ();
        }
    }
}


/*
 * CreateButton
 *
 * Create a button, assign event handlers, and attach the button to the
 * table in the proper place.
 */
GtkWidget *CreateButton (GtkWidget *table, 
                         typMinesweeperButton *mine, 
                         int row, 
                         int column)
{
    GtkWidget *button;

    /* --- Create the button --- */
    button = gtk_toggle_button_new ();

    /* --- Init the button fields --- */
    mine->buttonState = BUTTON_UNKNOWN;
    mine->nRow = row;
    mine->nCol = column;

    /* --- We care if the button is clicked --- */
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
                        GTK_SIGNAL_FUNC (mine_button_clicked), mine);

    /* --- We care about other mouse events. --- */
    gtk_signal_connect (GTK_OBJECT (button), "button_press_event",
                        GTK_SIGNAL_FUNC (button_press), mine);

    /* --- Put the button in the table in the right place. --- */
    gtk_table_attach (GTK_TABLE (table), button, 
                      column, column+1, 
                      row + 1, row + 2, 
                      GTK_FILL | GTK_EXPAND, 
                      GTK_FILL | GTK_EXPAND, 
                      0, 0);

    /* --- Set the button to a uniform size --- */
    gtk_widget_set_usize (button, BUTTON_WIDTH, BUTTON_HEIGHT);

    /* --- Make the button visible --- */
    gtk_widget_show (button);

    /* --- return the button. --- */
    return (button);
}



/* 
 * CountNearbyBombs
 *
 * Count the number of bombs this square is next to.
 */
int CountNearbyBombs (int col, int row)
{
    int i, j;
    int nCount = 0;

    /* --- Every square that would be at most 1 square away --- */
    for (i = MAX (col-1, 0); i <= MIN (col+1, nCols-1); i++) {
        for (j = MAX (row-1, 0); j <= MIN (row+1, nRows-1); j++) {

            /* --- If it's got a bomb --- */
            if (mines[i][j].bHasBomb) {

               /* --- Keep track of the count. --- */
               nCount++;
            }
        }
    }
    return (nCount);
}


/*
 * CreateMinesweeperButtons
 * 
 * Create the buttons on the minesweeper from the table we defined at the
 * beginning of this program.  The button pointers (handles) are stored
 * back in the table so they can be referenced later.
 */
void CreateMinesweeperButtons (GtkWidget *table, 
                               int nGridColumns, 
                               int nGridRows,
                               int bNewButtons)
{
    int ci;
    int ri;
    int nBombs;
    typMinesweeperButton *mine; 

    /* --- Update the global variables --- */
    nCols = nGridColumns;
    nRows = nGridRows;

    bGameOver = FALSE;
    bResetGame = TRUE;

    /* --- Update bomb count. --- */
    DisplayBombCount ();

    /* --- Check each button --- */
    for (ci = 0; ci < nCols; ci++) {
        for (ri = 0; ri < nRows; ri++) {

            /* --- The button has nothing at all. --- */
            mine = &mines[ci][ri];
            mine->bHasBomb = 0;
            mine->buttonState = BUTTON_UNKNOWN;

            /* --- Widget assoc to the mine? --- */
            if (bNewButtons) {

                /* --- Create a button --- */
                mine->widget = CreateButton (table, mine, ri, ci);
            } else {

                /* --- Reuse button --- */

                /* --- Free any existing xpm/label --- */
                FreeChild (mine->widget);

                /* --- Put button up. --- */
                gtk_toggle_button_set_state (
                           GTK_TOGGLE_BUTTON (mine->widget), 
                           FALSE);
            }
        }
    }

    /* --- Place the bombs. --- */
    nBombs = nTotalBombs;

    /* --- While we have bombs to deliver --- */
    while (nBombs > 0) {

        /* --- Calculate a row/col position --- */
        ci = rand () % nCols;
        ri = rand () % nRows;

        /* --- If no bomb exists, create one! --- */
        if (mines[ci][ri].bHasBomb == 0) {
            mines[ci][ri].bHasBomb = 1;
            nBombs--;
        }
    }

    /* --- Once all bombs have been distributed, calculate 
     *     how many bombs are adjacent to each button.
     */

    /* --- Check every button --- */
    for (ci = 0; ci < nCols; ci++) {
        for (ri = 0; ri < nRows; ri++) {

            mine = &mines[ci][ri];
           
            /* --- How many buttons? --- */
            mine->nBombsNearby = CountNearbyBombs (ci, ri);
        }
    }
    bResetGame = FALSE;
}


/*
 * SetGrid
 *
 * Sets the game grid to the size specified with the
 * number of bombs.
 */
void SetGrid (int nGridColumns, int nGridRows, int nBombs)
{
    /* --- If the packing table exists. --- */
    if (table) {

        /* --- Destroy it and all the buttons. --- */
        gtk_widget_destroy (table);
    } 

    /* --- Create a table for the buttons --- */
    table = gtk_table_new (nGridColumns, nGridRows, FALSE); 

    /* --- Add it to the vbox --- */
    gtk_box_pack_start (GTK_BOX (vbox), table, FALSE, FALSE, 0);

    /* --- Show the table. --- */
    gtk_widget_realize (table);

    /* --- Do a game reset with the numbers --- */
    ResetGame (nGridColumns, nGridRows, nBombs, TRUE);

    /* --- Show the table. --- */
    gtk_widget_show (table);
}



/*
 * main
 *
 * Program begins here
 */
int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *hbox;          

    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);

    /* --- Create the top window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- Don't allow window to be resized.  --- */
    gtk_window_set_policy (GTK_WINDOW (window), FALSE, FALSE, TRUE);

    /* --- Give the window a title. --- */
    gtk_window_set_title (GTK_WINDOW (window), "Minesweeper");

    /* --- You should always remember to connect the destroy event 
     *     to the main window.
     */
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
                        GTK_SIGNAL_FUNC (delete_event), NULL);
    
    vbox = gtk_vbox_new (FALSE, 1);
    gtk_widget_show (vbox);

    /* --- Create the application menu --- */
    CreateMenu (window, vbox);

    /* --- Horizontal box for score/start button --- */
    hbox = gtk_hbox_new (TRUE, 1);
    gtk_widget_show (hbox);

    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

    /*
     * --- Bombs left on the page is a label 
     */ 

    /* --- Add label with # of bombs left. --- */
    label_bombs = gtk_label_new ("");
    gtk_box_pack_start (GTK_BOX (hbox), label_bombs, FALSE, FALSE, 0);
    gtk_widget_show (label_bombs);

    /*
     * --- Create the start button with smilely face on it 
     */
    button_start = gtk_button_new ();

    /* --- We care if the button is clicked --- */
    gtk_signal_connect (GTK_OBJECT (button_start), 
                        "clicked",
                        GTK_SIGNAL_FUNC (start_button_clicked), 
                        NULL);

    gtk_box_pack_start (GTK_BOX (hbox), button_start, FALSE, FALSE, 0);
    gtk_widget_show (button_start);

    /*
     * --- Time on the right 
     */ 

    /* --- Add label with # of bombs left. --- */
    label_time = gtk_label_new ("");
    gtk_box_pack_start (GTK_BOX (hbox), label_time, FALSE, FALSE, 0);
    gtk_widget_show (label_time);

    /* --- Make them visible --- */
    gtk_widget_show (vbox);

    /* --- Add application vbox to main window --- */
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (window);

    /* --- Create the *smile* and put it on the start button --- */
    SetStartButtonIcon ((gchar **) xpm_smile);

    /* --- Create 10x10 grid. --- */
    SetGrid (10, 10, 10);

    /* --- Start gtk event processing --- */ 
    gtk_main ();
    exit (0);
}
