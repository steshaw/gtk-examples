#include <gtk/gtk.h>
#include <strings.h>
#include <stdio.h>


/*
 * CreateBarBitmap
 *
 * Create a pixmap with the characteristics desired.  
 * 
 * height - height to make the pixmap
 * width - width to make the pixmap
 * size - how big the bar should be
 * sColor - color of the filled in area.
 */
char **CreateBarBitmap (int height, int width, int size, char *sColor)
{
    char **sBitmap;
    char *newbuffer;
    char buffer[88];
    int i;

    /* --- Create the room for the data --- */
    sBitmap = g_malloc ((height + 1 + 2) * sizeof (gchar *));
    
    /* --- Create pixmap header - height/width/colors/#chars --- */
    sprintf (buffer, "%d %d 2 1", width, height);
    sBitmap[0] = g_strdup (buffer);

    /* --- Define the "none" color --- */
    sBitmap[1] = g_strdup ("  c None");

    /* --- Define the filled in color --- */
    sprintf (buffer, "X c %s", sColor);
    sBitmap[2] = g_strdup (buffer);

    /* --- Fill in the buffer with the size --- */
    strcpy (buffer, " ");
    for (i = 0; i < size; i++) {

        strcat (buffer, "X");
    }
   
    /* --- Leave the remainder not filled in --- */
    while (i < width) {
        strcat (buffer, " ");
        i++;
    }

    /* --- Pad with blank --- */
    strcat (buffer, " ");

    /* --- Copy the string --- */
    newbuffer = g_strdup (buffer);

    /* --- Make it the data for all the strings --- */
    for (i = 3; i < height+3; i++) {
        sBitmap[i] = newbuffer;
    }

    /* --- Return the creation --- */
    return (sBitmap);
}


/*
 * FreeBarBitmap
 *
 * Free all the memory that was allocated to create the
 *
 * bitmap.
 */
void FreeBarBitmap (char **bitmap)
{
    g_free (bitmap[0]);
    g_free (bitmap[1]);
    g_free (bitmap[2]);
    g_free (bitmap[3]);
    g_free (bitmap);
}


