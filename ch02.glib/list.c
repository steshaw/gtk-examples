/*
 * list.c 
 *
 * Show linked lists in action
 */

#include <stdio.h>
#include <string.h>
#include <glib.h>

/*
 * Display List
 * 
 * Display the data items in the list.
 */
void DisplayList (GSList *list)
{
    int    nIndex;
    GSList *node;

    for (nIndex = 0; node = g_slist_nth (list, nIndex); nIndex++) {

        g_print ("%s\n", (char *) node->data);
    }
}


/*
 * ListLookup
 *
 * Look up the string in the linked list and display a message as
 * to whether the name was found.
 */
void ListLookup (GSList *list, char *szName)
{
    GSList *item;

    g_print ("Looking up: %s  ", szName);

    /* --- Run the search --- */
    item = g_slist_find (list, szName);

    /* --- If we found it. --- */
    if (item) {

        g_print ("--Found it--\n");
    } else {

        g_print ("**not found**\n");
    }
}

/*
 * main
 * 
 * Program begins here.
 */
int main (int argc, char *argv[])
{
    GSList      *list;
    char	szMary[] = "Mary";

    /* --- Empty list. --- */
    list = NULL;

    /* --- Insert items in this order --- */
    /* --- Fred, Susie, Frank, Wilma  --- */
    list = g_slist_append (list, "Fred");
    list = g_slist_append (list, "Susie");
    list = g_slist_append (list, "Frank");
    list = g_slist_append (list, "Wilma");

    /* --- Insert Joe at the front of the list. --- *.
    list = g_slist_prepend (list, "Joe");

    /* --- Insert Mary after the second position --- *.
    list = g_slist_insert (list, "Mary", 2);

    /* --- Show the list so far. --- */
    DisplayList (list);

    ListLookup (list, "Mike");
    ListLookup (list, szMary);
    ListLookup (list, "Mary");
    ListLookup (list, "Susan");
    ListLookup (list, "Tyler");

    g_print ("-------------------\n");
    g_print ("   Reversing\n");
    g_print ("-------------------\n");

    /* --- Reverse the list. --- */
    list = g_slist_reverse (list);

    DisplayList (list);

    /* --- Free data --- */
    g_slist_free (list);
}
