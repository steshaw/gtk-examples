/*
 * File: sortlist.c
 */

#include <stdio.h>
#include <string.h>
#include <glib.h>


/*
 * Compare Names
 *
 * A Comparison function for inserting names alphabetically.
 */
gint CompareNames (gconstpointer sName1, gconstpointer sName2)
{
    return ((gint) strcmp ((char *) sName1, (char *) sName2));
}


/*
 * PrintNames
 *
 * A print function to display the data elements 
 * in the list.
 */
PrintNames (gpointer data, gpointer user_data)
{
    gchar    *message;
    GSList   *list;

    /* --- Convert the data to a string --- */
    message = (gchar *) data;

    /* --- Display the string --- */
    g_print ("%s position is ...\n", message);
}


/*
 * main
 * 
 * program begins here.
 */
int main (int argc, char *argv[])
{

    int    nIndex;
    GSList    *list, *node;
    char    buffer[88];

    /* --- Must be set to NULL --- */
    list = NULL;

    /* --- Insert the names  --- */
    list = g_slist_insert_sorted (list, "Fred", CompareNames);
    list = g_slist_insert_sorted (list, "Joe", CompareNames);
    list = g_slist_insert_sorted (list, "Susie", CompareNames);
    list = g_slist_insert_sorted (list, "Frank", CompareNames);
    list = g_slist_insert_sorted (list, "Wilma", CompareNames);
    list = g_slist_insert_sorted (list, "Mary", CompareNames);

    strcpy (buffer, "Wilma");
    node = g_slist_find (list, buffer);
    if (node) {
        g_print ("found..\n");
    } else {
        g_print ("not found..\n");
    }

    /* --- Loop through the list --- */
    for (nIndex = 0; node = g_slist_nth (list, nIndex); nIndex++) {

        /* --- Dump out the contents and look up the index --- */
        g_print ("%s position %d\n", 
                 (char *) node->data, 
                 g_slist_index (list, node->data));
    }

    g_print ("-----\n");

    /* --- Another way to print each of the data elements. --- */ 
    g_slist_foreach (list, (GFunc) PrintNames, list);
}
