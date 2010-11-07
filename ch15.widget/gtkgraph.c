/*
 * File: GtkGraph.c
 * Auth: Eric Harlow
 *
 * Simple gtk graphing widget
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "gtkgraph.h"

static GtkWidgetClass *parent_class = NULL;

/* 
 * forward declarations: 
 */
static void gtk_graph_class_init (GtkGraphClass *class);
static void gtk_graph_init (GtkGraph *graph);
static void gtk_graph_realize (GtkWidget *widget);
static void gtk_graph_draw (GtkWidget *widget, GdkRectangle *area);
static void gtk_graph_size_request (GtkWidget *widget,
                                    GtkRequisition *req);
static gint gtk_graph_expose (GtkWidget *widget, GdkEventExpose *event);
static void gtk_graph_destroy (GtkObject *object);


/*
 * gtk_graph_get_type
 *
 * Internal class.  Used to defined the GtkGraph class to GTK
 */
guint gtk_graph_get_type (void)
{
  static guint graph_type = 0;

    /* --- If not created yet --- */
    if (!graph_type) {

        /* --- Create a graph_info object --- */
        GtkTypeInfo graph_info =
            {
	      "GtkGraph",
	      sizeof (GtkGraph),
	      sizeof (GtkGraphClass),
	      (GtkClassInitFunc) gtk_graph_class_init,
	      (GtkObjectInitFunc) gtk_graph_init,
	      (GtkArgSetFunc) NULL,
              (GtkArgGetFunc) NULL,
            };
  
        /* --- Tell GTK about it - get a unique identifying key --- */
        graph_type = gtk_type_unique (gtk_widget_get_type (), &graph_info);
    }
    return graph_type;
}


/*
 * gtk_graph_class_init
 *
 * Override any methods for the graph class that are needed for
 * the graph class to behave properly.  Here, the functions that
 * cause painting to occur are overridden.
 *
 * class - object definition class.
 */
static void gtk_graph_class_init (GtkGraphClass *class)
{
    GtkObjectClass *object_class;
    GtkWidgetClass *widget_class;

    /* --- Get the widget class --- */
    object_class = (GtkObjectClass *) class;
    widget_class = (GtkWidgetClass *) class;
    parent_class = gtk_type_class (gtk_widget_get_type ());

    /* --- Override object destroy --- */
    object_class->destroy = gtk_graph_destroy;

    /* --- Override these methods --- */
    widget_class->realize = gtk_graph_realize;
    widget_class->draw = gtk_graph_draw;
    widget_class->size_request = gtk_graph_size_request;
    widget_class->expose_event = gtk_graph_expose;
}

/*
 * gtk_graph_init
 * 
 * Called each time a graph item gets created.
 * This initializes fields in our structure.
 */
static void gtk_graph_init (GtkGraph *graph)
{
    GtkWidget *widget;

    widget = (GtkWidget *) graph;
   
    /* --- Initial values --- */
    graph->values = NULL;
    graph->num_values = 0;
}


/*
 * gtk_graph_new
 * 
 * Create a new GtkGraph item
 */
GtkWidget* gtk_graph_new (void)
{
  return gtk_type_new (gtk_graph_get_type ());
}


/*
 * gtk_graph_realize
 *
 * Associate the widget with an x-window.
 *
 */
static void gtk_graph_realize (GtkWidget *widget)
{
  GtkGraph *darea;
  GdkWindowAttr attributes;
  gint attributes_mask;

  /* --- Check for failures --- */
  g_return_if_fail (widget != NULL);
  g_return_if_fail (GTK_IS_GRAPH (widget));

  darea = GTK_GRAPH (widget);
  GTK_WIDGET_SET_FLAGS (widget, GTK_REALIZED);

  /* --- attributes to create the window --- */
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.x = widget->allocation.x;
  attributes.y = widget->allocation.y;
  attributes.width = widget->allocation.width;
  attributes.height = widget->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.visual = gtk_widget_get_visual (widget);
  attributes.colormap = gtk_widget_get_colormap (widget);
  attributes.event_mask = gtk_widget_get_events (widget) | GDK_EXPOSURE_MASK;

  /* --- We're passing in x, y, visual and colormap values --- */
  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  /* --- Create the window --- */
  widget->window = gdk_window_new (gtk_widget_get_parent_window (widget), 
                                   &attributes, attributes_mask);
  gdk_window_set_user_data (widget->window, darea);

  widget->style = gtk_style_attach (widget->style, widget->window);
  gtk_style_set_background (widget->style, widget->window, GTK_STATE_NORMAL);
}

/*
 * gtk_graph_size
 *
 * Custom method to set the size of the graph.
 */
void gtk_graph_size (GtkGraph *graph, int size)
{
    g_return_if_fail (graph != NULL);
    g_return_if_fail (GTK_IS_GRAPH (graph));

    graph->num_values = size;
    graph->values = g_realloc (graph->values, sizeof (gint) * size);
}

/*
 * gtk_graph_set_value
 *
 * Custon method to set the siz
 */
void gtk_graph_set_value (GtkGraph *graph, int index, int value)
{
    g_return_if_fail (graph != NULL);
    g_return_if_fail (GTK_IS_GRAPH (graph));
    g_return_if_fail (index < graph->num_values && index >= 0);

    graph->values[index] = value;
}


/*
 * gtk_graph_draw
 *
 * Draw the widget.
 */
static void gtk_graph_draw (GtkWidget *widget, GdkRectangle *area)
{
    GtkGraph *graph;
    int width;
    int height;
    int column_width;
    int max = 0;
    int i;
    int bar_height;

    /* --- Check for obvious problems --- */
    g_return_if_fail (widget != NULL);
    g_return_if_fail (GTK_IS_GRAPH (widget));

    /* --- Make sure it's a drawable widget --- */
    if (GTK_WIDGET_DRAWABLE (widget)) {

        graph = GTK_GRAPH (widget);
        if (graph->num_values == 0) {
            return;
        }
 
        /* --- Get height and width --- */
        width = widget->allocation.width - 1;
        height = widget->allocation.height - 1;

        /* --- Calculate width of the columns --- */
        column_width = width / graph->num_values;

        /* --- Find the max value --- */
        for (i = 0; i < graph->num_values; i++) {
            if (max < graph->values[i]) {
                max = graph->values[i];
            }
        }

        /* --- Display each bar graph --- */
        for (i = 0; i < graph->num_values; i++) {

            bar_height = (graph->values[i] * height) / max;

            gdk_draw_rectangle (widget->window, 
                                widget->style->fg_gc[GTK_STATE_NORMAL],
                                TRUE,
                                (i * column_width),
                                height-bar_height,
                                (column_width-2),
                                bar_height);
        }
    }
}


/*
 * gtk_graph_size_request
 *
 * How big should the widget be?
 * It can be modified. 
 */
static void gtk_graph_size_request (GtkWidget *widget,
                                    GtkRequisition *req)
{

    req->width = 200;
    req->height = 200;
}

/*
 * gtk_graph_expose
 *
 * The graph widget has been exposed and needs to be painted.
 *
 */
static gint gtk_graph_expose (GtkWidget *widget, GdkEventExpose *event)
{
    GtkGraph *graph;

    /* --- Do error checking --- */
    g_return_val_if_fail (widget != NULL, FALSE);
    g_return_val_if_fail (GTK_IS_GRAPH (widget), FALSE);
    g_return_val_if_fail (event != NULL, FALSE);

    if (event->count > 0) {
        return (FALSE);
    }

    /* --- Get the graph widget --- */
    graph = GTK_GRAPH (widget);

    /* --- Clear the window --- */
    gdk_window_clear_area (widget->window, 0, 0, 
                           widget->allocation.width,
                           widget->allocation.height);

    /* --- Draw the graph --- */
    gtk_graph_draw (widget, NULL);
}

static void gtk_graph_destroy (GtkObject *object)
{

    GtkGraph *graph;

    /* --- Check type --- */
    g_return_if_fail (object != NULL);
    g_return_if_fail (GTK_IS_GRAPH (object));

    /* --- Convert to graph object --- */
    graph = GTK_GRAPH (object);

    /* --- Free memory --- */
    g_free (graph->values);

    /* --- Call parent destroy --- */
    GTK_OBJECT_CLASS (parent_class)->destroy (object);
}
