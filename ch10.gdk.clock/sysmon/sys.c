/*
 * File: sys.c
 * Auth: Eric Harlow
 *
 * Linux development
 */

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include "device.h"

#define SEPARATORS " |\n"
void GetPacketInfo ();

/*
 * GetPacketInfo
 *
 * Get information about the packets going across the 
 * ethernet.  Packet information is in /proc/net/dev if
 * the machine supports the /proc file system.
 */
void GetPacketInfo ()
{

    char *szFilename = "/proc/net/dev";
    char szBuffer[132];
    int  nLineNo = 0;
    FILE *fp;
    char *sToken;
    char *sDevName;
    int  nWord;
    long nIncoming;
    long nOutgoing;
    typDevice *dev;

    /* --- Open the file to get the information --- */
    fp = fopen (szFilename, "r");
    if (fp) {
        
        /* --- While we have information to read --- */
        while (!feof (fp)) {
        
            nLineNo++;

            /* --- Read in a line of text --- */
            fgets (szBuffer, sizeof (szBuffer), fp);

            /* --- After 3rd line are the devices --- */
            if (nLineNo >= 3) {

                nWord = 0;
                sDevName = NULL;
                sToken = strtok (szBuffer, SEPARATORS);
                while (sToken) {
                    switch (nWord) {
                        case 0:
                            sDevName = sToken;
                            break;
                        case 1:
                            nIncoming = atoi (sToken);
                            break;
                        case 6:
                            nOutgoing = atoi (sToken);
                            break;
                    }

                    nWord++;
                    sToken = strtok (NULL, SEPARATORS);
                }
 
                /* --- Got a name --- */
                if (sDevName) {

                    /* --- Look it up --- */
                    dev = LookupDevice (sDevName);

                    /* --- Add/Update it --- */
                    if (dev) {
                        UpdateExistingDevice (sDevName, 
                             (long) (nIncoming + nOutgoing));
                    } else {
                        UpdateDevice (sDevName, 
                             (long) (nIncoming + nOutgoing), DELTA, 1);
                    }
                }
                szBuffer[0] = (char) 0;
            }
        }

        /* --- All done --- */
        fclose (fp);
    } else {

        /* --- Uh... error! --- */
        printf ("Unable to open file %s.", szFilename);
    }
}



/* 
 * GetCPUInfo
 *
 * Read in the /proc/loadavg file and parse for the cpu information.
 */
void GetCPUInfo ()
{
    static char *szFile = "/proc/loadavg";
    char szBuffer[88];
    FILE *fp;
    float cpu1, cpu2, cpu3;
    long lcpu1, lcpu2, lcpu3;
    typDevice *dev;

    /* --- Open the file with cpu info --- */
    fp = fopen (szFile, "r");

    /* --- If it's opened... --- */
    if (fp) {

        fgets (szBuffer, sizeof (szBuffer), fp);

        /* --- Get CPU utilization numbers --- */
        sscanf (szBuffer, "%f %f %f", &cpu1, &cpu2, &cpu3);

        /* --- Want range of 1-100+ instead of .01 to 1.0+ --- */
        lcpu1 = cpu1 * 100;
        lcpu2 = cpu2 * 100;
        lcpu3 = cpu3 * 100;

        /* --- Look up device --- */
        dev = LookupDevice ("cpu1");
        if (dev) {

            /* --- Already exists, update devices --- */

            UpdateExistingDevice ("cpu1", lcpu1);
            UpdateExistingDevice ("cpu5", lcpu2);
            UpdateExistingDevice ("cpu15", lcpu3);
        } else {

            /* --- First time, create devices --- */

            UpdateDevice ("cpu1", lcpu1, ACTUAL, 100); 
            UpdateDevice ("cpu5", lcpu2, ACTUAL, 100); 
            UpdateDevice ("cpu15", lcpu3, ACTUAL, 100);
        }
        /* --- Clean up. --- */
        fclose (fp);
    } else {
        printf ("Unable to open file %s.\n", szFile);
    }
}



