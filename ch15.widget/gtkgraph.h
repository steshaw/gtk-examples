/*
 * File: gtkgraph.h
 * Auth: Eric Harlow
 */
#ifndef __GTK_GRAPH_H__
#define __GTK_GRAPH_H__


#include <gdk/gdk.h>
#include <gtk/gtkvbox.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* 
 * --- Macros for conversion and type checking 
 */
#define GTK_GRAPH(obj) \
   GTK_CHECK_CAST (obj, gtk_graph_get_type (), GtkGraph)
#define GTK_GRAPH_CLASS(klass) \
   GTK_CHECK_CLASS_CAST (klass, gtk_graph_get_type, GtkGraphClass)
#define GTK_IS_GRAPH(obj) \
   GTK_CHECK_TYPE (obj, gtk_graph_get_type ())

/*
 * --- Defining data structures.
 */

typedef struct _GtkGraph		GtkGraph;
typedef struct _GtkGraphClass	GtkGraphClass;

/*
 * Here's the graph data
 */
struct _GtkGraph
{
    GtkWidget vbox;

    gint *values;
    gint num_values;
};


/*
 * Here's the class data.
 */
struct _GtkGraphClass
{
  GtkWidgetClass parent_class;
};


/*
 * Function prototypes
 */
GtkWidget* gtk_graph_new (void);
void gtk_graph_size (GtkGraph *graph, int size);
void gtk_graph_set_value (GtkGraph *graph, int index, int value);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __GTK_GRAPH_H__ */
