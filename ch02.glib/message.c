/* 
 * message.c
 *
 * Example showing message functions
 */

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Surfer Print
 *
 * Override function for the g_print function. 
 */
void SurferPrint (const gchar *buf)
{
    printf ("Dude, ");
    fputs (buf, stdout);
}

/*
 * Surfer Message
 *
 * Override for the g_message function
 */
void SurferMessage (const gchar *buf)
{
    printf ("Dude, ya got a message -> ");
    fputs (buf, stdout);
}

/*
 * SurferWarning
 *
 * Override for the g_warning function
 */
void SurferWarning (const gchar *buf)
{
    printf ("Bad news Dude. -> ");
    fputs (buf, stdout);
}

/*
 * SurferError
 *
 * Override for the g_error function
 */
void SurferError (const gchar *buf)
{
    printf ("Major wipe out, dude. -> ");
    fputs (buf, stdout);
}

/*
 * ShowParams
 *
 * Show the options available for running the program.
 */
void ShowParams ()
{
    printf ("Must pass in parameter.  Valid parameters are:\n");
    printf (" 'surfer' - use surfer message handling.\n");
    printf (" 'normal' - use normal message handling.\n ");
    exit (0);
}

/*
 * main
 *
 * Program begins here
 */
int main (int argc, char *argv[])
{

    /* --- Not enough args? --- */
    if (argc <= 1) {

        ShowParams ();
    }

    /* --- Normal speech? --- */
    if (strcmp (argv[1], "normal") == 0) {

        /* --- Do nothing - just verify that parameter is valid. --- */

    /* --- Surfer speech?  --- */
    } else if (strcmp (argv[1], "surfer") == 0) {

        /* --- Seems that they want surfer speech for the errors. --- */
        g_set_error_handler (SurferError);
        g_set_warning_handler (SurferWarning);
        g_set_message_handler (SurferMessage);
        g_set_print_handler (SurferPrint);
    } else {

        /* --- Can only pick 'normal' or 'surfer' --- */
        ShowParams ();
    }

    /* 
     * --- Show functions at work.  If we have custom handlers, 
     * --- the message will be intercepted.
     */

    g_print ("Here's a print\n");
    g_message ("Here's a message\n");
    g_warning ("Here's a warning\n");
    g_error ("Here's an error\n");
    
}
