#include <gtk/gtk.h>
#include <sys/stat.h>
#include <unistd.h>
#include "notepad.h"

static GtkWidget *text;

/* Increase the size of the buffer for better performance.  
 * It's small so that the progress bar shows on large 
 * files.  If the buffer is too big, the file gets loaded
 * too quickly and I don't get to show the progress bar
 * in action.
 */
#define BUF_SIZE 256


/*
 * ClearFile
 *
 * Called to clear all the text in the text widget
 */
void ClearFile (GtkWidget *widget, gpointer data)
{
    gtk_editable_delete_text (GTK_EDITABLE (text), 0, -1);
}

/*
 * TextCut
 *
 * Cut the selected text from the text widget and 
 * put it in the clipboard.
 */
void TextCut (GtkWidget *widget, gpointer data)
{
    gtk_editable_cut_clipboard (GTK_EDITABLE (text));
}


/*
 * TextCopy
 *
 * Copy the selected text in the widget into the 
 * clipboard.
 */
void TextCopy (GtkWidget *widget, gpointer data) 
{
    gtk_editable_copy_clipboard (GTK_EDITABLE (text));
}


/*
 * TextPaste
 *
 * Paste the text in the clipboard into the text widget
 */
void TextPaste (GtkWidget *widget, gpointer data)
{
    gtk_editable_paste_clipboard (GTK_EDITABLE (text));
}



/*
 * ImportFile
 *
 * Import an rtf file.
 */
void ImportFile (char *szFilename)
{
    char szOutFile[88];

    strcpy (szOutFile, szFilename);
    szOutFile [strlen (szFilename) - 3] = (char) 0;
    strcat (szOutFile, "tmp");

    ConvertRTF (szFilename, szOutFile);

    LoadFile (szOutFile);
}


/*
 * LoadFile
 *
 * sFilename - file to load
 *
 * Reads in the file and places it in the text widget.
 */
void LoadFile (char *sFilename)
{
    char buffer[BUF_SIZE];
    int nchars;
    FILE *infile;
    struct stat fileStatus;
    long fileLen = 0;

    /* --- Freeze the widget --- */
    gtk_text_freeze (GTK_TEXT (text));

    /* --- Empty the widget --- */
    gtk_editable_delete_text (GTK_EDITABLE (text), 0, -1);

    /* --- Get file information --- */
    stat (sFilename, &fileStatus);
    fileLen = fileStatus.st_size;

    StartProgress ();

    /* --- Open the file --- */
    infile = fopen (sFilename, "r");
      
    /* --- If we open the file. --- */
    if (infile) {
      
        /* --- Read a chunk... --- */
        while ((nchars = fread (buffer, 1, BUF_SIZE, infile)) > 0) {

            /* --- Update the progress bar --- */
            UpdateProgress (ftell (infile), fileLen);

            /* --- Insert the text --- */
            gtk_text_insert (GTK_TEXT (text), NULL, NULL,
                   NULL, buffer, nchars);
          
            /* --- Less than buffer means EOF --- */
            if (nchars < BUF_SIZE)
                break;
        }
      
        /* --- Close file  --- */
        fclose (infile);
    }
    EndProgress ();
      
    /* --- Un-freeze the text widget - it'll repaint now --- */
    gtk_text_thaw (GTK_TEXT (text));
}



/*
 * SaveFile
 *
 * sFilename - filename to load
 *
 * Save the file.
 */
void SaveFile (char *sFilename)
{
    FILE *outfile;
    char *buffer;
    int nchars;

    gtk_text_freeze (GTK_TEXT (text));

    /* --- Open the file --- */
    outfile = fopen (sFilename, "w");
      
    if (outfile) {
      
        /* --- Get the string from the widget --- */
        buffer = gtk_editable_get_chars (
                     GTK_EDITABLE (text), 
                     (gint) 0,
                     (gint) gtk_text_get_length (GTK_TEXT (text)));

        /* --- Write the buffer to disk --- */
        nchars = fwrite (buffer, sizeof (char), strlen (buffer), outfile);
 
        /* --- Close the file --- */
        fclose (outfile);

        if (nchars != strlen (buffer)) {

            ShowMessage ("Save", "Error: Can't write file.");
        }
 
        /* --- Free up the memory --- */
        g_free (buffer);

    } else {

        ShowMessage ("Save", "Error: Can't save file.");
    }
      
    gtk_text_thaw (GTK_TEXT (text));
}


/*
 * CreateText
 *
 * Creates the text widget for the editor.
 * 
 */
void CreateText (GtkWidget *window, GtkWidget *container)
{
    GtkWidget *table;
    GtkWidget *hscrollbar;
    GtkWidget *vscrollbar;

    /* --- Create a table to put text widget and scroll bars --- */
    table = gtk_table_new (2, 2, FALSE);

    /* --- Add table to container --- */
    gtk_container_add (GTK_CONTAINER (container), table);

    /* --- No gaps so the scrollbars look like they are a 
           part of the widget. --- */
    gtk_table_set_row_spacing (GTK_TABLE (table), 0, 2);
    gtk_table_set_col_spacing (GTK_TABLE (table), 0, 2);

    /* --- Add packing table to the container and make visible --- */
    gtk_widget_show (table);

    /* --- Create the text widget --- */
    text = gtk_text_new (NULL, NULL);

    /* --- Allow it to be edited --- */
    gtk_text_set_editable (GTK_TEXT (text), TRUE);

    /* --- Insert the text widget into the table --- */
    gtk_table_attach (GTK_TABLE (table), text, 0, 1, 0, 1,
            GTK_EXPAND | GTK_SHRINK | GTK_FILL,
            GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0);

    /* --- Make it visible --- */
    gtk_widget_show (text);

    /* --- Add a horizontal scroll bar --- */
    hscrollbar = gtk_hscrollbar_new (GTK_TEXT (text)->hadj);
    gtk_table_attach (GTK_TABLE (table), hscrollbar, 0, 1, 1, 2,
            GTK_EXPAND | GTK_FILL | GTK_SHRINK, GTK_FILL, 0, 0);
    gtk_widget_show (hscrollbar);

    /* --- Add a vertical scroll bar --- */
    vscrollbar = gtk_vscrollbar_new (GTK_TEXT (text)->vadj);
    gtk_table_attach (GTK_TABLE (table), vscrollbar, 1, 2, 0, 1,
            GTK_FILL, GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0);
    gtk_widget_show (vscrollbar);

}


/*
 * GetText
 *
 * Get the text that's in the widget.
 */
char *GetText ()
{
    char *buffer;

    /* --- Get the string from the widget --- */
    buffer = gtk_editable_get_chars (
                 GTK_EDITABLE (text), 
                 (gint) 0,
                 (gint) gtk_text_get_length (GTK_TEXT (text)));
 
    return (buffer);
}


/*
 * GetTextWidet
 *
 * Return the widget that's used to display the text.  This
 * is just encapsulating the variable.
 */
GtkWidget *GetTextWidget ()
{
    return (text);
}
