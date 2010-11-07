/*
 * File: tree.c
 */

#include <stdio.h>
#include <string.h>
#include <glib.h>

/*
 * CompareNames
 *
 * Compare function used so sort the tree
 */
gint CompareNames (gconstpointer name1, gconstpointer name2)
{
	return (strcmp (name1, name2));
}

/*
 * TraverseTree
 *
 * Function that displays the information at the
 * node.
 */
gint TraverseTree (gpointer key, gpointer value, gpointer data)
{
	char	*sKey = key;
	char	*sValue = value;

	g_print ("Key: %s,  Value: %s\n", sKey, sValue);
	return FALSE;
}

/*
 * main
 *
 * Program begins here.
 */
int main (int argc, char *argv[])
{

	GTree	*tree;

	tree = g_tree_new (CompareNames);

	g_tree_insert (tree, "Fred", "Loud");
	g_tree_insert (tree, "Mary", "Obnoxious");
	g_tree_insert (tree, "Sue", "Drunk");
	g_tree_insert (tree, "John", "Quiet");
	g_tree_insert (tree, "Shelley", "Civil");
	g_tree_insert (tree, "Mark", "Strange");
	g_tree_insert (tree, "Renato", "Mighty");

        g_print ("Looking up Fred - value %s\n", 
		 (gchar *) g_tree_lookup (tree, "Fred"));

	g_print ("Tree height: %d\n", g_tree_height (tree));
	g_print ("Tree nodes: %d\n", g_tree_nnodes (tree));

	g_print ("Tree:\n ");

	g_tree_traverse (tree, TraverseTree, G_IN_ORDER, NULL);
}
