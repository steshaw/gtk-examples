/*
 * File: molecule.c
 * Auth: Eric Harlow
 *
 * Linux GUI app. development
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "atom.h"
#include "matrix3d.h"

typedef struct {

    GdkDrawable *pixmap;
    GdkGC *gc;

} typGraphics;


typedef struct {
    typAtom *atom1;    /* --- First atom in the bond --- */
    typAtom *atom2;    /* --- Second atom in the bond --- */
    int bPainted;      /* --- painted. --- */
} typBond;


/*
 * Prototypes.
 */
int ShowLines ();
int ShowLabels ();
int ShowRGB ();
void QSortMolecules (typAtom *atomlist, int *sortindex, int lo0, int hi0);
void MoleculeRepaint ();
GdkGC *GetAtomColor (typAtom *atom);
void SortMolecules (typAtom *atomlist, int *sortindex);
void FindBB ();
char *GetNextValue (char *sTmp, int *nIndex1);
typMatrix3D *NewMatrix3D ();
void DrawMolecule (typGraphics *g);
void DrawBond (typGraphics *g, typAtom *atom1, typAtom *atom2);
char *FindDigit (char *szBuffer);
char *ExtractDigit (char *sTmp, int *pnValue);
void SetAtomColor (typAtom *atom, typGraphics *g);
typGraphics *NewGraphics ();
void paint (typGraphics *g);
int mouseDrag (int x, int y);


#define    MAX_ATOMS 500
#define    MAX_BONDS 500

    /* --- Matrices --- */
    typMatrix3D *mat = NULL;
    typMatrix3D *amat = NULL;
    typMatrix3D *tmat = NULL;

    /* --- Bounding box --- */
    double    xmin, xmax, ymin, ymax, zmin, zmax;
    
    /* --- Molecule items --- */
    int    nAtoms = 0;
    int    nBonds = 0;
    typAtom atomlist[MAX_ATOMS];
    typBond bondlist[MAX_BONDS];
    int sortindex[MAX_ATOMS];

    /* --- Fonts, colors, drawing area --- */
    GdkGC *penBlack = NULL;
    GdkGC *penBlue = NULL;
    GdkGC *penRed = NULL;
    GdkGC *penGreen = NULL;
    GdkGC *penGray = NULL;
    GdkGC *penDarkGray = NULL;
    GdkGC *penWhite = NULL;
    GdkGC *penYellow = NULL;
    GdkGC *penOrange = NULL;
    GtkWidget *drawing_area;
    GdkFont *font;

    /* --- Misc. --- */
    typGraphics *g = NULL;
    int    prevx;
    int    prevy;
    int nMoleculeRadius = 2;
    int nScreenWidth = 200;
    int nScreenHeight = 200;

/*
 * GetPen
 *
 * Get a GdkGC (pen) based on the colors passed 
 * in.  This is used to change the color of the 
 * items being drawn.
 *
 * nRed - Red component of pen
 * nGreen - Green component of pen
 * nBlue - Blue component of pen
 */
GdkGC *GetPen (int nRed, int nGreen, int nBlue)
{
    GdkColor *c;
    GdkGC *gc;

    c = (GdkColor *) g_malloc (sizeof (GdkColor));
    c->red = nRed;
    c->green = nGreen;
    c->blue = nBlue;

    gdk_color_alloc (gdk_colormap_get_system (), c);

    gc = gdk_gc_new (g->pixmap);

    gdk_gc_set_foreground (gc, c);

    return (gc);
}



/*
 * Init3d 
 *
 * Initialize the 3d items that we'll need to draw
 */
void Init3d ()
{
static int bInit = 0;

    /* --- Already initialized --- */
    if (bInit) return;

    bInit = TRUE;

    /* --- Create new matrices --- */
    mat = NewMatrix3D ();
    amat = NewMatrix3D ();
    tmat = NewMatrix3D ();

    /* --- Create pens for drawing --- */
    penBlack = GetPen (0, 0, 0);
    penRed = GetPen (0xffff, 0, 0);
    penGreen = GetPen (0, 0xffff, 0);
    penBlue = GetPen (0, 0, 0xffff);
    penGray = GetPen (0xafff, 0xafff, 0xafff);
    penDarkGray = GetPen (0x6fff, 0x6fff, 0x6fff);
    penYellow = GetPen (0xffff, 0xffff, 0x0000);
    penOrange = GetPen (0xffff, 0x99ff, 0x0000);
    penWhite = GetPen (0xffff, 0xffff, 0xffff);

    /* --- Get a font --- */
    font = gdk_font_load ("-adobe-helvetica-medium-r-normal--*-120-*-*-*-*-*-*");
    if (font == NULL) {
        printf ("NULL font, trying another\n");
        font = gdk_font_load ("-adobe-helvetica-*-120-*-*-*-*-*-*");
    }
}


/*
 * ReadMolecule
 *
 * Read in a .pdb molecule with the given filename.
 * Store the information in the data structures 
 * defined.
 */
void ReadMolecule (char *sFilename)
{
    FILE *fp;
    char buffer[120];
    float x, y, z;
    int nIndex1;
    int nIndex2;
    char szName[120];
    char *sTmp;
    typAtom *atom;
    char szTmp[20];

    Init3d ();

    nAtoms = 0;
    nBonds = 0;

    /* --- Reset matrices if reading in new file --- */
    if (mat) {
        unit (mat);
        unit (amat);
        unit (tmat);
    }

    nMoleculeRadius = 2;

    /* --- Open the file for reading --- */
    fp = fopen (sFilename, "r");

    /* --- Read in a line from the file --- */
    while (fgets (buffer, sizeof (buffer), fp)) {

        /* --- Why it's an 'atom' --- */
        if (strncmp (buffer, "ATOM", 4) == 0) {

            /* --- Read in the atom based on the known file  
             *      structure 
             */
            strncpy (szName, &buffer[12], 4);
            szName[4] = 0;
            strncpy (szTmp, &buffer[30], 8);
            szTmp[8] = 0;
            x = atof (szTmp);
            strncpy (szTmp, &buffer[38], 8);
            szTmp[8] = 0;
            y = atof (szTmp);
            strncpy (szTmp, &buffer[46], 8);
            szTmp[8] = 0;
            z = atof (szTmp);

            /* --- Atoms are base 1 - index first. --- */
            nAtoms++;

            /* --- Populate the data structure --- */
            atom = &atomlist[nAtoms];
            atom->x = x;
            atom->y = y;
            atom->z = z;
            atom->szName = strdup (szName);
            atom->bondList = NULL;
            sortindex[nAtoms-1] = nAtoms;
            
        /* --- Does this describe a bond? --- */
        } else if (strncmp (buffer, "CONECT", 6) == 0) {

            /* --- Get the first bond atom --- */
            sTmp = GetNextValue (&buffer[6], &nIndex1);

            /* --- Get the next bond atom --- */
            while (sTmp = GetNextValue (sTmp, &nIndex2)) {

                /* --- This bond is this nIndex1 to nIndex2 --- */
                bondlist[nBonds].atom1 = &atomlist[nIndex1];
                bondlist[nBonds].atom2 = &atomlist[nIndex2];

                /* --- Of course the atom should know which 
                 *   bonds it is a part of.
                 */
                atomlist[nIndex1].bondList = 
                     g_slist_append (atomlist[nIndex1].bondList,  
                                     &bondlist[nBonds]);

                /* --- Both atoms should know... --- */
                atomlist[nIndex2].bondList = 
                     g_slist_append (atomlist[nIndex2].bondList,  
                                     &bondlist[nBonds]);

                /* --- Just another bond.. James Bond. --- */
                nBonds++;
            }
        }
    }

    /* --- Find bounding box --- */
    FindBB ();

    /* --- Sort the molecules --- */
    SortMolecules (atomlist, sortindex);

    MoleculeRepaint ();
}


/*
 * FindBB
 *
 * Find the minimum bounding box that would fit all the 
 * atoms in the molecule
 */
void FindBB ()
{
    int        i;
    typAtom    *atom;

    /* --- At first, the box consisted of a single atom --- */
    /* --- Atoms start at 1 --- */
    atom = &atomlist[1];

    xmin = atom->x;
    xmax = atom->x;

    ymin = atom->y;
    ymax = atom->y;

    zmin = atom->z;
    zmax = atom->z;

    /* --- Then we added a whole bunch of them --- */
    for (i = 2; i <= nAtoms; i++) {

        atom = &atomlist[i];
        if (atom->x < xmin) xmin = atom->x;
        if (atom->x > xmax) xmax = atom->x;
        if (atom->y < ymin) ymin = atom->y;
        if (atom->y > ymax) ymax = atom->y;
        if (atom->z < zmin) zmin = atom->z;
        if (atom->z > zmax) zmax = atom->z;
    }

    /* --- And now we have our bounding box. --- */
}
   


/*
 * TransformPoints
 *
 * Take the atoms and rotate them to their displayed
 * position so they can be drawn.  
 *
 * mat - The matrix used to calculate the new atom position
 */
void TransformPoints (typMatrix3D *mat)
{
    int    i;

    for (i = 1; i <= nAtoms; i++) {

        Transform (mat, &atomlist [i]);
    }
}



/*
 * paint
 *
 * paint the screen
 */
void paint (typGraphics *g)
{
    double xfac;
    double f1;
    double f2;

    /* --- What's the range (delta x, delta y, delta z) --- */
    double xw = xmax - xmin;
    double yw = ymax - ymin;
    double zw = zmax - zmin;

    /* --- Make sure everything is allocated --- */
    Init3d ();

    /* --- Calculate the factor to scale the molecule --- */
    if (yw > xw) xw = yw;
    if (zw > xw) xw = zw;
    f1 = nScreenWidth / xw;
    f2 = nScreenHeight / xw;
    xfac = .7 * (f1 < f2 ? f1 : f2);

    /* --- First make the matrix the unit matrix --- */
    unit (mat);

    /* --- Translate it around the origin.  This moves the  
     *     molecule to be centered around the axis as we're 
     *     moving it based on the half of the bounding box width
     */
    translate (mat, -(xmin + xmax) / 2,
                   -(ymin + ymax) / 2,
                   -(zmin + zmax) / 2);

    /* --- Rotate the image around the axis.  amat is the  
     *     matrix that represents how much to rotate the 
     *     molecule.
     */
    mult (mat, amat);

    /* --- Scale the molecule based on the screen width --- */
    scale3 (mat, xfac, -xfac, 16 * xfac / nScreenWidth);

    /* --- Translate the molecule out based on the screen width and height --- */
    translate (mat, nScreenWidth / 2, nScreenHeight / 2, 10);

    /* --- Calculate the new position of all the points --- */
    TransformPoints (mat);

    /* --- Draw the molecule based on translated coordinates --- */
    DrawMolecule (g);
}


/* 
 * SortMolecules
 *
 * Call the sorting function
 */
void SortMolecules (typAtom *atomlist, int *sortindex)
{
    QSortMolecules (atomlist, sortindex, 0, nAtoms-1);
}

/*
 * QSortMolecules
 *
 * This is just a quicksort of the atoms in the molecule. 
 *
 * Note: Rather than sort the atomlist itself, the sortindex
 * is the item being sorted.  Manipulating integers is just
 * quick than moving structures around in memory - especially
 * since we have to do this in real time over and over again.
 *
 * atomlist - list of atoms to sort
 * sortlist - index of indexes.
 */
void QSortMolecules (typAtom *atomlist, int *sortindex, int lo0, int hi0) 
{
    int    nTmp;
    int    lo = lo0;
    int    hi = hi0;
    int    mid;    

    if (hi0 > lo0) {

        mid = atomlist[sortindex[(lo0 + hi0) / 2]].tz;

        while (lo <= hi) {

            while (lo < hi0 && atomlist[sortindex[lo]].tz < mid) {
                lo++;
            }

            while (hi > lo0 && atomlist[sortindex[hi]].tz > mid) {
                hi--;
            }

            if (lo <= hi) {

                nTmp = sortindex[lo];
                sortindex[lo] = sortindex[hi];
                sortindex[hi] = nTmp;
                lo++;
                hi--;
            }
        }
        if (lo0 < hi) QSortMolecules (atomlist, sortindex, lo0, hi);
        if (lo < hi0) QSortMolecules (atomlist, sortindex, lo, hi0);
    }
}


/*
 * DrawMolecule
 *
 * Draw the molecule.  'nuff said.
 */
void DrawMolecule (typGraphics *g)
{
    int nIndex;
    int nDiameter;
    typBond *bond;
    GSList *list;
    typAtom *atom;
    int i;
    GdkGC *pen;

    /* --- Make sure everything's ready --- */
    Init3d ();

    /* --- Sort the molecules --- */
    SortMolecules (atomlist, sortindex);

    /* --- Get the molecule diameter --- */
    nDiameter = nMoleculeRadius + nMoleculeRadius;

    /* --- If we're showing bonds --- */
    if (ShowLines()) {

        /* --- Clear out the flag that indicates that this 
         *     particular bond has been painted.
         */
        for (i = 0; i < nBonds; i++) {

            bondlist[i].bPainted = FALSE;
        }
    }

    /* --- Display all the atoms in the list --- */
    for (i = 0; i < nAtoms; i++) {

        /* --- Use the sort list - draw from farthest back 
         *     moving up.
         */
        nIndex = sortindex[i];

        /* --- Get the atom the index refers to --- */
        atom = &atomlist[nIndex];

        /* --- This atom has a color --- */
        pen = GetAtomColor (atom);

        /* --- Draw a circle with the atom color --- */
        gdk_draw_arc (g->pixmap, pen, TRUE, 
                      atom->tx - 3, atom->ty - 3, 7, 7, 0, 360 * 64);

        /* --- If they want labels --- */
        if (ShowLabels ()) {

            /* --- Show the label --- */
            if (atom->szName) {
                gdk_draw_string (g->pixmap, font,
                             pen, atom->tx + 5, atom->ty,
                             atom->szName);              
            }
        }

        /* --- If they want lines (bonds) --- */
        if (ShowLines()) {

            /* --- Show all bonds for this atom --- */
            for (list = atom->bondList; list; list = list->next) {

                /* --- Get the bond from the list --- */
                bond = (typBond *) list->data;

                /* --- If it hasn't been painted yet.. --- */
                if (bond->bPainted == FALSE) {

                    /* --- Paint the bond (draw the line) --- */
                    DrawBond (g, bond->atom1, bond->atom2);

                    /* --- Bond marked as painted --- */
                    bond->bPainted = TRUE;
                }
            }
        }
    }

}


/*
 * DrawBond
 *
 * Draw a bond between these two atoms.
 *
 * g - our data structure that has information used to draw
 * atom1 - first atom in the bond pair
 * atom2 - second atom in the bond pair.
 */ 
void DrawBond (typGraphics *g, typAtom *atom1, typAtom *atom2)
{
    gdk_draw_line (g->pixmap, penBlack,
                   (int) (atom1->tx), 
                   (int) (atom1->ty),
                   (int) (atom2->tx), 
                   (int) (atom2->ty));
          

}


/*
 * GetNextValue
 *
 * Parses the string looking for the next numeric value 
 * in the string.  Returns the remainder as a string.  The value 
 * is five chars wide.
 *
 * sTmp - The current string
 * nIndex - pointer to a value which will be read in.
 *
 * Returns a new string minus the value read in.
 */
char *GetNextValue (char *sTmp, int *nIndex)
{

    char buffer[12];
    strncpy (buffer, sTmp, 5);
    buffer[5] = 0;

    *nIndex = atoi (buffer);

    if (*nIndex == 0) {

        return (NULL);
    } else {

        /* --- Return new string --- */
        return (&sTmp[5]);
    }
}


/*
 * FindDigit
 *
 * Look for a digit in the string buffer
 *
 * szBuffer - string we are looking in for a numeric 
 *            value
 */
char *FindDigit (char *szBuffer)
{

    /* --- If it's NULL, there's nothing here --- */
    if (szBuffer == NULL) {
        return (NULL);
    }

    /* --- While we have string and no number... --- */
    while (*szBuffer && !isdigit (*szBuffer)) {

        szBuffer++;
    }

    /* --- Return the buffer --- */
    return (szBuffer);
}

/*
 * ExtractDigit
 *
 * Extract the numeric data from the string and return the 
 * remainder of the string
 *
 * sTmp - string to extract numeric value.
 * pnValue - value to set
 *
 * returns remainder of the string
 */
char *ExtractDigit (char *sTmp, int *pnValue)
{
    int nIndex = 0;
    char buffer[88];

    /* --- If it's NULL, there's no value. --- */
    if (sTmp == NULL) {
        return (NULL);
    }

    /* --- While we are finding digits --- */
    while (*sTmp && isdigit (*sTmp)) {

        /* --- Add to the string --- */
        buffer[nIndex] = *sTmp;       
        nIndex++;
        sTmp++;
    }

    /* --- Mark the end of the string --- */
    buffer[nIndex] = (char) 0;

    /* --- Convert string to number --- */
    *pnValue = atoi (buffer);

    /* --- Return the remainder of the string (if any) --- */
    if (*sTmp) return (sTmp);

    /* --- Nope.. all done --- */
    return (NULL);
}




/*
 * GetAtomName
 *
 */
void GetAtomName (char *szName, typAtom *atom)
{
    int i;
    int j = 0;

    for (i = 0; i < 2; i++) {

        if (isalpha (atom->szName[i])) {

            szName[j++] = atom->szName[i];
        }
    }
    szName[j] = 0;
}

/*
 * GetAtomColor
 *
 * Gets the color of the atom based on some of the 
 * characteristics of the atom.
 *
 * The molecules are either drawn black or drawn in 
 * color depending on a flag set by a toolbar button.  
 *
 * If the flag is set...
 * If the name begins with a C (carbon) that we draw
 * it in black.  If the name begins with 'N' (nitrogen)
 * then we draw it in blue.  If the name begins with 'O'
 * (oxygen) then we draw in in red.  Everything else is 
 * drawn in gray.
 *
 * atom - atom to have it's color chosen
 */
GdkGC *GetAtomColor (typAtom *atom)
{
    char szName[10];

    /* --- Don't know the name. --- */
    if (atom->szName == NULL) {
        return (penBlack);
    }
 
    /* --- Don't show colors --- */
    if (!ShowRGB ()) {
        return (penBlack);
    }

    GetAtomName (szName, atom);

    if (!strcmp (szName, "CL")) {
         return (penGreen);
    } else if (!strcmp (szName, "C")) {
         return (penBlack);
    } else if (!strcmp (szName, "S")) {
         return (penYellow);
    } else if (!strcmp (szName, "P")) {
         return (penOrange);
    } else if (!strcmp (szName, "N")) {
         return (penBlue);
    } else if (!strcmp (szName, "O")) {
         return (penRed);
    } else if (!strcmp (szName, "H")) {
         return (penWhite);
    } else {
         return (penDarkGray);
    }
}




/*
 * NewGraphics
 *
 * Allocate space for our graphics object
 */
typGraphics *NewGraphics ()
{
    typGraphics *gfx;

    gfx = (typGraphics *) g_malloc (sizeof (typGraphics));
    gfx->gc = NULL;
    gfx->pixmap = NULL;

    return (gfx);
}


/* 
 * Create a new backing pixmap of the appropriate size 
 */
static gint configure_event (GtkWidget *widget, GdkEventConfigure *event)
{
    /* --- Structure doesn't exist? --- */
    if (g == NULL) {

        /* --- Create one --- */
        g = NewGraphics ();
    }

    /* --- Existing pixmap?  --- */
    if (g->pixmap) {

        /* --- Free it --- */
        gdk_pixmap_unref (g->pixmap);
    } 

    /* --- Create a new pixmap --- */
    g->pixmap = gdk_pixmap_new (widget->window,
                                widget->allocation.width,
                                widget->allocation.height,
                                -1);

    /* --- Get height and width to clear screen --- */
    nScreenWidth = widget->allocation.width;
    nScreenHeight = widget->allocation.height;

    /* --- Create a new pixmap --- */
    gdk_draw_rectangle (g->pixmap,
                        widget->style->white_gc,
                        TRUE,
                        0, 0,
                        widget->allocation.width,
                        widget->allocation.height);

    /* --- Redraw molecule --- */
    MoleculeRepaint ();

    return TRUE;
}


/*
 * expose_event
 *
 * Called when an area on the screen has been exposed 
 * and we have to repaint it.  The area is repainted from
 * the background area we have. 
 */
static gint expose_event (GtkWidget *widget, GdkEventExpose *event)
{

    gdk_draw_pixmap(widget->window,
            widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
            g->pixmap,
            event->area.x, event->area.y,
            event->area.x, event->area.y,
            event->area.width, event->area.height);

    return FALSE;
}


/*
 * quit
 *
 * exit gtk message loop
 */
void quit ()
{
    gtk_exit (0);
}


/*
 * MoleculeRepaint
 *
 * Called by us when the user uses the mouse to move the 
 * molecule.  This causes the molecule to be repainted on
 * the screen.
 */ 
void MoleculeRepaint ()
{
    GdkRectangle  update_rect;

    Init3d ();

    /* --- clear pixmap --- */
    gdk_draw_rectangle (g->pixmap,
                      penGray,
		      TRUE,
		      0, 0,
		      drawing_area->allocation.width,
		      drawing_area->allocation.height);

    /* --- Draw molecule in the background --- */
    paint (g);

    /* --- The whole screen --- */
    update_rect.x = 0;
    update_rect.y = 0;
    update_rect.width = drawing_area->allocation.width;
    update_rect.height = drawing_area->allocation.height;

    /* --- Call the expose event - which copies the background 
     *     into the widget
     */
    gtk_widget_draw (drawing_area, &update_rect);
}


/*
 * motion_notify
 *
 * Called when the user moves the mouse on the screen
 */
gint motion_notify (GtkWidget *widget, GdkEventMotion *event)
{
    int x, y;
    GdkModifierType state;

    /* --- If it's a hint... (combining several events) --- */
    if (event->is_hint) {

        /* --- Get new position --- */
        gdk_window_get_pointer (event->window, &x, &y, &state);
    } else {

        /* --- Get new position --- */
        x = event->x;
        y = event->y;
        state = event->state;
    }

    /* --- If the mouse button is down --- */
    if (state & GDK_BUTTON1_MASK && g->pixmap != NULL) {

        /* --- Calculate the mouse drag effect on the  
         *     molecule.
         */
        mouseDrag (x, y);
    } 

    /* --- Keep track of the position of the mouse --- */
    prevx = x;
    prevy = y;

    return TRUE;
}


/*
 * mouseDrag
 *
 * Calculate the new rotation of the molecule based on how 
 * the user drags the mouse over the molecule.
 *
 * x - x position of mouse
 * x - y position of mouse
 */
int mouseDrag (int x, int y)
{

    /* --- Calculate the x difference --- */
    double xtheta = (prevy - y) * (360.0f / nScreenWidth);

    /* --- Calculate the y difference --- */
    double ytheta = (x - prevx) * (360.0f / nScreenHeight);

    /* --- Unit matrix --- */
    unit (tmat);

    /* --- Rotate by the -x- the mouse moved --- */
    xrot (tmat, xtheta);

    /* --- Rotate by the -y- the mouse moved --- */
    yrot (tmat, ytheta);

    /* --- Combine into existing rotation to get new rotation --- */
    mult (amat, tmat);

    /* --- Redraw with new rotation --- */
    MoleculeRepaint ();

    return TRUE;
}



/*
 * main
 *
 * Program begins here.
 */
GtkWidget *CreateDrawingArea ()
{
    GtkWidget *window;

    /* --- Create a top level window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- Create the drawing area  --- */
    drawing_area = gtk_drawing_area_new ();

    /* --- Set the size --- */
    gtk_drawing_area_size (GTK_DRAWING_AREA (drawing_area), 300, 300);

    /* --- Make drawing area visible --- */
    gtk_widget_show (drawing_area);

    /* --- Signals used to handle background pixmap --- */
    gtk_signal_connect (GTK_OBJECT (drawing_area), "expose_event",
                        (GtkSignalFunc) expose_event, NULL);

    /* --- Window is created and resized --- */
    gtk_signal_connect (GTK_OBJECT(drawing_area),"configure_event",
                        (GtkSignalFunc) configure_event, NULL);

    /* --- Need to know about mouse movements. --- */
    gtk_signal_connect (GTK_OBJECT (drawing_area), "motion_notify_event",
                        (GtkSignalFunc) motion_notify, NULL);

    /* --- Events to listen for --- */
    gtk_widget_set_events (drawing_area, GDK_EXPOSURE_MASK
                           | GDK_LEAVE_NOTIFY_MASK
                           | GDK_BUTTON_PRESS_MASK
                           | GDK_POINTER_MOTION_MASK
                           | GDK_POINTER_MOTION_HINT_MASK);

    return (drawing_area);
}



