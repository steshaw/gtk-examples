/*
 * packtable.c
 * 
 * Illustration of how to pack tables.
 */

#include <gtk/gtk.h>

static char * cursor_hand[] = {
"32 32 3 1",
" 	c None",
".	c #000000",
"+	c #FFFFFF",
"                                ",
"                                ",
"            ...                 ",
"           .+++.   ...          ",
"           .+++.  .+++.   ...   ",
"   ...     .+++.  .+++.  .+++.  ",
"  .+++.    .+++. .+++.  .+++.   ",
"   .+++.   .+++. .+++. .+++.    ",
"    .+++.  .+++. .+++. .+++.    ",
"     .+++. .+++..+++. .+++.     ",
"      .+++..+++..+++. .+++.     ",
"       .+++.+++..+++..+++.      ",
"   ...  .++++++++++++++++.      ",
"  .+++. .+++++++++++++++.       ",
"  .++++..+++++++++++++++.       ",
"   ..+++++++++++++++++++.       ",
"     .++++++++++++++++++.       ",
"      .+++++++++++++++++.       ",
"       ..++++++++++++++.        ",
"         .++++++++++++.         ",
"          .++++++++++.          ",
"           ..........           ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                ",
"                                "};

static char * cursor_sight[] = {
"32 32 3 1",
" 	c None",
".	c #000000",
"+	c #FF0000",
"                                ",
" .............................  ",
" .             +             .  ",
" .             +             .  ",
" .           +++++           .  ",
" .             +             .  ",
" .        +++++++++++        .  ",
" .             +             .  ",
" .           +++++           .  ",
" .             +             .  ",
" .        +++++++++++        .  ",
" .    +   +    +    +   +    .  ",
" .    +   +  +++++  +   +    .  ",
" .  + + + + +  +  + + + + +  .  ",
" .  + + + + +  +  + + + + +  .  ",
" .+++++++++++++++++++++++++++.  ",
" .  + + + + +  +  + + + + +  .  ",
" .  + + + + +  +  + + + + +  .  ",
" .    +   +  +++++  +   +    .  ",
" .    +   +    +    +   +    .  ",
" .        +++++++++++        .  ",
" .             +             .  ",
" .           +++++           .  ",
" .             +             .  ",
" .        +++++++++++        .  ",
" .             +             .  ",
" .           +++++           .  ",
" .             +             .  ",
" .             +             .  ",
" .............................  ",
"                                ",
"                                ",
"                                "};

GdkColor white = {0, 0xffff, 0xffff, 0xffff};
GdkColor black = {0, 0x0000, 0x0000, 0x0000};
GdkColor red = {0, 0xffff, 0x0000, 0x0000};
GdkColor blue = {0, 0x0000, 0x0000, 0xffff};


void
create_bitmap_and_mask_from_xpm (GdkBitmap **bitmap, GdkBitmap **mask, gchar **xpm)
{
	int height, width, colors;
	char pixmap_buffer [(32 * 32)/8];
	char mask_buffer [(32 * 32)/8];
	int x, y, pix;
	int transparent_color, black_color;
	
	sscanf (xpm [0], "%d %d %d %d", &height, &width, &colors, &pix);

	g_assert (height == 32);
	g_assert (width  == 32);
	g_assert (colors == 3);

	transparent_color = ' ';
	black_color = '.';
	
	for (y = 0; y < 32; y++){
		for (x = 0; x < 32;){
			char value = 0, maskv = 0;
			
			for (pix = 0; pix < 8; pix++, x++){
				if (xpm [4+y][x] != transparent_color){
					maskv |= 1 << pix;

					if (xpm [4+y][x] != black_color){
						value |= 1 << pix;
					}
				}
			}
			pixmap_buffer [(y * 4 + x/8)-1] = value;
			mask_buffer [(y * 4 + x/8)-1] = maskv;
		}
	}
	*bitmap = gdk_bitmap_create_from_data (NULL, pixmap_buffer, 32, 32);
	*mask   = gdk_bitmap_create_from_data (NULL, mask_buffer, 32, 32);
}


/*
 *
 */
gint CloseAppWindow (GtkWidget *widget, gpointer *data)
{
    gtk_main_quit ();
    return (FALSE);
}
    
/*
 * CreateTable
 *
 * Creates a top level window with several options.  Since 
 * this is going to be done many times, it's best just made
 * into a function. 
 */ 
void CreateTable (char *szTitle, gint xoptions, gint yoptions) 
{
    GtkWidget *window;
    GtkWidget *handButton;
    GtkWidget *sightButton;
    GtkWidget *crossButton;
    GtkWidget *table;
    GdkBitmap *bitmap;
    GdkBitmap *mask;
    GdkCursor *cursor;

    /* --- Create the top level window --- */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

    /* --- Give the window a title. --- */
    gtk_window_set_title (GTK_WINDOW (window), szTitle);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
			GTK_SIGNAL_FUNC (CloseAppWindow), NULL);

    /* --- Give the window a border --- */
    gtk_container_border_width (GTK_CONTAINER (window), 10);
    
    /* --- Create a 4x4 table --- */
    table = gtk_table_new (2, 2, TRUE);

    handButton = gtk_button_new_with_label ("Hand");
    gtk_table_attach (GTK_TABLE (table), handButton, 0, 1, 0, 1, 
                      xoptions, yoptions, 0, 0);
    gtk_widget_show (handButton);

    crossButton = gtk_button_new_with_label ("Cross");
    gtk_table_attach (GTK_TABLE (table), crossButton, 0, 1, 1, 2, 
                      xoptions, yoptions, 0, 0);
    gtk_widget_show (crossButton);

    sightButton = gtk_button_new_with_label ("Gun sight");
    gtk_table_attach (GTK_TABLE (table), sightButton, 1, 2, 0, 1, 
                      xoptions, yoptions, 0, 0);
    gtk_widget_show (sightButton);

    /* --- Add the table to the window --- */
    gtk_container_add (GTK_CONTAINER (window), table);

    /* --- Make the table visible --- */
    gtk_widget_show (table);

    /* --- Make the window visible --- */
    gtk_widget_show (window);

    create_bitmap_and_mask_from_xpm (&bitmap, &mask, cursor_hand);
    cursor = gdk_cursor_new_from_pixmap (bitmap, mask, &white, &black, 16, 8);
    gdk_window_set_cursor (handButton->window, cursor);

    create_bitmap_and_mask_from_xpm (&bitmap, &mask, cursor_sight);
    cursor = gdk_cursor_new_from_pixmap (bitmap, mask, &red, &blue, 16, 16);
    gdk_window_set_cursor (sightButton->window, cursor);

    cursor = gdk_cursor_new (GDK_CROSS);
    gdk_window_set_cursor (crossButton->window, cursor);
}


/*
 * --- Main.
 *
 * Program begins here.
 *
 * Rather than duplicate code, the CreateTable function is 
 * called.  It does the work for us - all we do is pass in 
 * how we want the window to appear.
 */
int main (int argc, char *argv[])
{
    
    /* --- GTK initialization --- */
    gtk_init (&argc, &argv);

    /* --- No flags set --- */
    CreateTable ("Nothing", 0, 0);

    /* --- Start the gtk loop. --- */
    gtk_main ();

    exit (0);
}
