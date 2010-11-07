
#include <gtk/gtk.h>

typedef struct {
    char *szName;        // --- Molecule name
    double x;            // --- Original coordinates
    double y;            //
    double z;            //
    double tx;           // --- Translated coordinates
    double ty;           //
    double tz;           //
    GSList *bondList;    // --- Atoms it has a bond with
} typAtom;
