/*
 * GtkDialog
 */

#include <gtk/gtk.h>


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
