/*
 * Timer.c
 *
 * Auth: Eric Harlow
 *
 * Routines to update something every second.
 */

#include <gtk/gtk.h>


static int nSeconds = 0;                 
static gint timer = 0;
static int bTimerRunning = FALSE;

void UpdateSeconds (int);

/*
 * TimerCallback
 *
 * Every second, this will be called to update the time
 * in the clock window.
 */
gint TimerCallback (gpointer data)
{
    /* --- Another second has gone by --- */
    nSeconds++;

    UpdateSeconds (nSeconds);

    return 1; // NOTE: Needs to be non-zero otherwise timer seems to be stopped.
}


/*
 * StartTimer
 *
 * Starts up the time.  Happens when the user first
 * clicks on a button.
 */
void StartTimer ()
{

    /* --- If the timer isn't already running --- */
    if (!bTimerRunning) {

        /* --- Start at zero --- */
        nSeconds = 0;

        /* --- Call function 'TimerCallback' every 1000ms --- */
        timer = gtk_timeout_add (1000, TimerCallback, NULL);

        /* --- Timer is running. --- */
        bTimerRunning = TRUE;
    }
}

/*
 * StopTimer
 *
 * Stops the timer.  User probably hit a bomb.
 */
void StopTimer ()
{
    /* --- If the time is running. --- */
    if (bTimerRunning) {

        /* --- Stop the timer. --- */
        gtk_timeout_remove (timer);
 
        /* --- Fix the flag. --- */
        bTimerRunning = FALSE;
    }
}
