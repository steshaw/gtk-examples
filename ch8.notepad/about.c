/*
 * About.c
 */

#include <gtk/gtk.h>
#include "notepad.h"


/*
 * ShowAbout
 *
 * Show the about dialog.  Reuse existing code.
 */
void ShowAbout ()
{
    ShowMessage ("About...",
                  "GtkNotepad v.07\n - "
                  "Eric Harlow\n");

}
