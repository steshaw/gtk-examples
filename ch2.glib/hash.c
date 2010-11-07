

#include <stdio.h>
#include <string.h>
#include <glib.h>


/*
 * Compare Names
 *
 * A Comparison function for inserting names alphabetically.
 */
gint HashCompare (gconstpointer sName1, gconstpointer sName2)
{
    return (!strcmp ((char *) sName1, (char *) sName2));
}

/*
 * Make a hash from the first two characters.
 */
guint HashFunction (gconstpointer key)
{
    return (((char *) key)[0] + ((char *) key)[1]);
}

/*
 * PrintNames
 *
 * A print function to display the data elements 
 * in the list.
 */
PrintNames (gpointer key, gpointer value, gpointer user_data)
{
    g_print ("Key: %s, Value: %s\n", 
		(gchar *) key, (gchar *) value);
}

int main (int argc, char *argv[])
{

    int         nIndex;
    char        buffer[88];
    GHashTable	*hTable;

    hTable = g_hash_table_new (HashFunction, HashCompare);

    /* --- Insert the names  --- */
    g_hash_table_insert (hTable, "Fred", "Boring");
    g_hash_table_insert (hTable, "Mary", "Shifty");
    g_hash_table_insert (hTable, "Sue", "Nice");
    g_hash_table_insert (hTable, "John", "Strange");
    g_hash_table_insert (hTable, "Shelley", "Abnormal");
    g_hash_table_insert (hTable, "Markus", "Absent minded");
    g_hash_table_insert (hTable, "Renato", "Paranoid");
    g_hash_table_insert (hTable, "Renato", "Smart");
    g_hash_table_insert (hTable, "Renato", "Intelligent");
    g_hash_table_insert (hTable, "Renato", "Stubby toes");

    g_print ("Renato is %s\n", 
	     (gchar *) g_hash_table_lookup (hTable, "Renato"));

    g_print ("-----\n");

    /* --- Another way to print each of the data elements. --- */
    g_hash_table_foreach (hTable, (GHFunc) PrintNames, NULL);
}
