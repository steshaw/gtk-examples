/*
 * File: DisplayFonts.c
 * 
 * Show all fonts available on the system.
 */

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>

/*
 * If they have more than this many fonts, they have
 * too many fonts anyways.
 */
#define MAX_FONTS 30000

/*
 * main.c
 *
 */
int main (int argc, char *argv[])
{
    int nFonts;
    char **szaFontNames;
    int i;

    /* --- Initialize GTK.  Needed for the GDK_DISPLAY call --- */
    gtk_init (&argc, &argv);

    /* --- Get the font names --- */
    szaFontNames = XListFonts (GDK_DISPLAY (), "*", MAX_FONTS, &nFonts);

    /* --- Check number retrieved --- */
    if (nFonts == MAX_FONTS) {

        /* --- They got a lot of fonts on their system. --- */
        printf ("Many fonts on your system.  Not displaying all.");
    }

    /* --- Display all the fonts --- */
    for (i = 0; i < nFonts; i++) {

        /* --- Get the name --- */
        printf ("%s\n", szaFontNames[i]);
    }

    XFreeFontNames (szaFontNames);

    return (0);
}
