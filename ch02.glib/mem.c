
#include <stdio.h>
#include <string.h>
#include <glib.h>

/*
 * CompareNames
 *
 * Comparsion function used to sort names
 */
gint CompareNames (gconstpointer name1, gconstpointer name2)
{
	return ((gint) strcmp (name1, name2));
}

/*
 * TraverseTree
 *
 * Function called from tree to display the node's
 * information
 */
gint TraverseTree (gpointer key, gpointer value, gpointer data)
{
	char	*sKey = key;
	char	*sValue = value;

	g_print ("Key: %s,  Value: %s\n", sKey, sValue);
	return FALSE;
}

int main (int argc, char *argv[])
{

	GTree	*tree;
        char *bad_pointer;

	tree = g_tree_new (CompareNames);

	g_tree_insert (tree, "Fred", "Loud");
	g_tree_insert (tree, "Mary", "Obnoxious");
	g_tree_insert (tree, "Sue", "Drunk");
	g_tree_insert (tree, "John", "Quiet");
	g_tree_insert (tree, "Shelley", "Civil");
	g_tree_insert (tree, "Mark", "Strange");
	g_tree_insert (tree, "Renato", "Mighty");

        g_mem_profile ();

        bad_pointer = g_malloc (100 * sizeof (char));
        bad_pointer[101] = 'a';
        g_free (bad_pointer);
        g_free (bad_pointer);
}
