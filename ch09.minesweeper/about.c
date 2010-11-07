/*
 * GtkDialog
 */

#include <gtk/gtk.h>

extern void ShowMessage (char *szTitle, char *szMessage);

/*
 * ShowAbout
 *
 * Show the about dialog.  Reuse existing code.
 */
void ShowAbout ()
{
    ShowMessage ("About...",
                  "Minesweeper v.06\n - "
                  "Eric Harlow\n");

}
