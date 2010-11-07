/*
 * Calculator.c
 *
 * Example showing different widgets in use.
 *
*/

#include "gtk/gtk.h"
#include <math.h>
#include <ctype.h>

static float num1 = 0;
static char lastChar = (char) 0;
static char prevCmd = (char) 0;
#define BUF_SIZE 88

/*
 * --- data structure to keep track of the calculator buttons.
*/
typedef struct {

    char      *szLabel;    /* --- Label display on button --- */
    int       row;         /* --- Row to place the button --- */
    int       col;         /* --- Column to place the button --- */
    GtkWidget *widget;     /* --- Handle to the button --- */

} typCalculatorButton;


/* 
 * --- This is the button list.  Each button is documented here so 
 *     we can access it.
*/
typCalculatorButton buttonList [] = {
    {"C",   1, 0, NULL},      /* --- Clear --- */
    {"CE",  1, 1, NULL},      /* --- Clear --- */
    {"/",   1, 3, NULL},      /* --- Division --- */

    {"7",   2, 0, NULL},      /* --- Digit --- */
    {"8",   2, 1, NULL},      /* --- Digit --- */
    {"9",   2, 2, NULL},      /* --- Digit --- */
    {"*",   2, 3, NULL},      /* --- Multiplication --- */
    {"%",   2, 4, NULL},      /* --- Percent --- */

    {"4",   3, 0, NULL},      /* --- Digit --- */
    {"5",   3, 1, NULL},      /* --- Digit --- */
    {"6",   3, 2, NULL},      /* --- Digit --- */
    {"-",   3, 3, NULL},      /* --- Subtraction --- */
    {"1/x", 3, 4, NULL},      /* --- 1/x --- */

    {"1",   4, 0, NULL},      /* --- Digit --- */
    {"2",   4, 1, NULL},      /* --- Digit --- */
    {"3",   4, 2, NULL},      /* --- Digit --- */
    {"+",   4, 3, NULL},      /* --- Addition --- */
    {"sqrt",4, 4, NULL},      /* --- Square root --- */

    {"+/-", 5, 0, NULL},      /* --- Negate value --- */
    {"0",   5, 1, NULL},      /* --- zero --- */
    {".",   5, 2, NULL},      /* --- Decimal --- */
    {"=",   5, 3, NULL},      /* --- Equals/total --- */
    {"x^2", 5, 4, NULL},      /* --- Squared --- */
};

/*
 * --- Number of buttons in the data structure.  
*/
int nButtons = sizeof (buttonList) / 
               sizeof (typCalculatorButton);

/* --- This is the LCD panel - the results --- */
GtkWidget *label;


/*
 * CloseAppWindow
 *
 * The window is closing down, end the gtk loop
*/
gint CloseAppWindow (GtkWidget *widget, gpointer data)
{
    gtk_main_quit ();

    return (FALSE);
}


/*
 * TrimTrailingZeros
 *
 * Get rid of trailing zeros 
 * Takes the string and removes the trailing zeros.
*/
void TrimTrailingZeros (char *szDigits)
{
    int nIndex;
    int bDecimal = FALSE;
    int nPos = -1;

    /* --- Loop through the string. --- */
    for (nIndex = 0; nIndex < strlen (szDigits); nIndex++) {

        /* --- Is this a decimal? --- */
        if (szDigits[nIndex] == '.') {
             bDecimal = TRUE;
        }

        /* --- If we're on the right side of the decimal... --- */
        if (bDecimal) {

            /* --- A zero?  Hmm... from this point on? --- */
            if (szDigits[nIndex] == '0') {

               /* --- If we don't have a point yet... --- */
               if (nPos < 0) {
 
                   /* --- Save this as a point. --- */
                   nPos = nIndex;
               }
            } else {

               /* --- Clear it.  Bad point. --- */
               nPos = -1;
            }
        }
    }

    /* --- Truncate the field. --- */
    if (nPos > 0) {
        szDigits[nPos] = (char) 0;
    }
}


/*
 * TrimLeadingZeros
 *
 * Trim the leading zeros.
 * 
 * Converts numbers like "0000012" to "12"
*/
void TrimLeadingZeros (char *szDigits)
{
    int nPos;

    if (szDigits == NULL) return;

    /* --- While we have a combination a digit in front --- */
    for (nPos = 0; (szDigits[nPos] && szDigits[nPos] == '0'); nPos++) {

        /* --- If the digit is a zero and next char is a digit --- */
        if (isdigit (szDigits[nPos+1])) {

            /* --- Blank the field. --- */  
            szDigits[nPos] = ' ';
        } 
    }
}


/*
 * Command
 *
 * Returns true if the character is a two digit command.
*/
int Command (char ch)
{
    switch (ch) {
        case '+':
        case '-':
        case '/':
        case '*':
        case '=':
            return (TRUE);
    }
    return (FALSE);
}

/*
 * FloatingPointChar
 *
 * Returns true if the character is any of [0123456789.]
*/
int FloatingPointChar (char ch)
{

    return (isdigit (ch) || ch == '.');
}


/*
 * key_press
 *
 * Handle the button "key_press" event.
 *
 * Function looks for the keystroke in the calculator 
 * data structure and (if a match is found) presses the 
 * button that matches the keystroke for the user.  It
 * keeps our code small since we only have to handle the
 * button_clicked events.
*/
void key_press (GtkWidget *widget, 
                GdkEventKey *event, 
                gpointer data)
{
    int nIndex;

    /* --- Search through the buttons --- */
    for (nIndex = 0; nIndex < nButtons; nIndex++) {

        /* --- If the keystroke is the first character of a button AND --- */
        /* --- the button label length is one. --- */      
        if (event->keyval == buttonList[nIndex].szLabel[0] && 
            buttonList[nIndex].szLabel[1] == (char) 0) {

            /* --- Set focus to that button --- */
            gtk_widget_grab_focus (buttonList[nIndex].widget);

            /* --- Make like the button was clicked to do processing. --- */
            gtk_button_clicked (GTK_BUTTON (buttonList[nIndex].widget));
            return;
        }
    }
}


/*
 * HandleDigit
 *
 * Digit button was pressed, deal with it.  How it
 * is dealt with depends on the situation.
*/
void HandleDigit (char *str, char ch)
{
    char *labelText;
    char buffer[BUF_SIZE];
    int  len;

    /* --- And they just did a command --- */
    if (Command (lastChar)) {

        /* --- Clear the digit field --- */
        gtk_label_set (GTK_LABEL (label), "");

        /* --- If they did a computation --- */
        if (lastChar == '=') {

            /* --- Clear out the command --- */
            lastChar = (char) 0;
            prevCmd = (char) 0;
        }
    }

    /* --- Get the buffer in the led --- */
    gtk_label_get (GTK_LABEL (label), &labelText);
    strcpy (buffer, labelText);

    /* --- Add the new character on it. --- */
    len = strlen (buffer);
    buffer[len] = (gchar) ch;
    buffer[len+1] = (gchar) 0;
   
    /* --- Trim leading zeros. --- */
    TrimLeadingZeros (buffer);

    /* --- Add digit to field. --- */
    gtk_label_set (GTK_LABEL (label), (char *) buffer);
}


/*
 * MaybeUnary
 *
 * str
 * 
 * Check to see if the user hit a unary operator button - 
 * like %, sqrt, 1/x, etc that should be dealt with NOW
 * not later.
*/
void MaybeUnaryOperation (char *str)
{
    char *labelText;
    char buffer[BUF_SIZE];
    float num2;

    /* --- Get number in the field. --- */
    gtk_label_get (GTK_LABEL (label), &labelText);
    num2 = atof (labelText);

    /* --- Percentage? --- */
    if (strcmp (str, "%") == 0) {
        num2 = num2 / 100;

    /* --- Trying for 1/x? --- */
    } else if (strcmp (str, "1/x") == 0) {

        /* --- Can't divide by zero. --- */
        if (num2 == 0) {
            /*Error (); */
            return;
        }
        num2 = 1 / num2;

    /* --- Calculate sqrt --- */
    } else if (strcmp (str, "sqrt") == 0) {
        num2 = sqrt ((double) num2);

    /* --- Calculate square --- */
    } else if (strcmp (str, "x^2") == 0) {
        num2 = num2 * num2;
    }

    /* --- Put the number back. --- */
    sprintf (buffer, "%f", (float) num2);
    TrimTrailingZeros (buffer);
    TrimLeadingZeros (buffer);
    gtk_label_set (GTK_LABEL (label), buffer);
}


void HandleBinaryOperation ()
{
    char buffer[BUF_SIZE];
    char *labelText;
    float num2;

    /* --- Get number in the field. --- */
    gtk_label_get (GTK_LABEL (label), &labelText);
    num2 = atof (labelText);

    /* --- Calculate based on previous command. --- */
    switch (prevCmd) {
        case '+':
            num1 = num1 + num2;               
            break;

        case '-':
            num1 = num1 - num2;               
            break;

        case '*':
            num1 = num1 * num2;               
            break;

        case '/':
            num1 = num1 / num2;               
            break;

        case '=':
            num1 = num2;
            break;

        default:
            num1 = num2;
            break;
    }

    /* --- Put the number back. --- */
    sprintf (buffer, "%f", (float) num1);
    TrimTrailingZeros (buffer);
    TrimLeadingZeros (buffer);
    gtk_label_set (GTK_LABEL (label), buffer);
}


/*
 * button_clicked
 *
 * widget - button pressed.
 * data - button label.
 *
 * Button was pressed, handle it.
*/
void button_clicked (GtkWidget *widget, gpointer data)
{
    char ch = *((char *) data);
    char *str;
    
    /* --- Get the button label --- */
    str = (char *) data;

    /* --- Entering a number... --- */
    if (FloatingPointChar (ch) && strlen (str) == 1) {

        HandleDigit (str, ch);

    } else {

        /* --- Clear? --- */
        if (strcmp (str, "CE") == 0) {
            gtk_label_set (GTK_LABEL (label), "0");
            return;

        /* --- BIG clear? --- */
        } else if (strcmp (str, "C") == 0) {
            prevCmd = (char) 0;
            lastChar = (char) 0;
            gtk_label_set (GTK_LABEL (label), "0");
            return;

        } else {

            /* --- Maybe it's a unary operator? --- */
            MaybeUnaryOperation (str);
        }

        /* --- See if there's a binary operation to do --- */
        HandleBinaryOperation ();

        prevCmd = ch;
    }
    lastChar = ch;
}

/*
 * CreateButton
 *
 * Create a button, assign event handlers, and attach the button to the
 * table in the proper place.
*/
GtkWidget *CreateButton (GtkWidget *table, char *szLabel, int row, int column)
{
    GtkWidget *button;

    /* --- Create the button --- */
    button = gtk_button_new_with_label (szLabel);

    /* --- We care if the button is clicked --- */
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
                        GTK_SIGNAL_FUNC (button_clicked), szLabel);

    /* --- Put the button in the table in the right place. --- */
    gtk_table_attach (GTK_TABLE (table), button, 
                      column, column+1, 
                      row, row + 1, 
                      GTK_FILL | GTK_EXPAND, 
                      GTK_FILL | GTK_EXPAND, 
                      5, 5);

    /* --- Make the button visible --- */
    gtk_widget_show (button);

    /* --- return the button. --- */
    return (button);
}



/*
 * CreateCalculatorButtons
 *
 * Create the buttons on the calculator from the table we defined at the
 * beginning of this program.  The button pointers (handles) are stored
 * back in the table so they can be referenced later.
*/
void CreateCalculatorButtons (GtkWidget *table)
{
    int nIndex;

    /* --- Run through the list of buttons. --- */
    for (nIndex = 0; nIndex < nButtons; nIndex++) {

        /* --- Create a button --- */
        buttonList[nIndex].widget = 
                CreateButton (table, 
                              buttonList[nIndex].szLabel, 
                              buttonList[nIndex].row, 
                              buttonList[nIndex].col);
    }
}

/*
 * main
 *
 * Program begins here
*/
int main (int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *table;

    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);

    /* --- Create the calculator window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- Give the window a title. --- */
    gtk_window_set_title (GTK_WINDOW (window), "Calculator");

    /* --- Set the window size. --- */
    gtk_widget_set_usize (window, 200, 200);

    /* --- We care if a key is pressed --- */
    gtk_signal_connect (GTK_OBJECT (window), "key_press_event",
                        GTK_SIGNAL_FUNC (key_press), NULL);

    /* --- You should always remember to connect the delete event
     *     to the main window. --- */
    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
                        GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    /* --- Create a 5x5 table for the items in the calculator. --- */
    table = gtk_table_new (5, 5, TRUE); 

    /* --- Create the calculator buttons. --- */
    CreateCalculatorButtons (table);

    /* --- Create the calculator LED --- */
    label = gtk_label_new ("0");
    gtk_misc_set_alignment (GTK_MISC (label), 1, .5);

    /* --- Add label to the table --- */
    gtk_table_attach_defaults (GTK_TABLE (table), label, 
                              0, 4, 0, 1);
    gtk_widget_show (label);
  
    /* --- Make them visible --- */
    gtk_container_add (GTK_CONTAINER (window), table);
    gtk_widget_show (table);
    gtk_widget_show (window);

    /* --- Grab focus for the keystrokes --- */
    //gtk_widget_grab_focus (buttonList[0].widget);

    gtk_main ();
    return (0);
}



