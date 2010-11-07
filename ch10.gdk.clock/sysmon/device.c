/*
 * File: device.c
 * Auth: Eric Harlow
 *
 * Linux development
 */

#include <stdio.h>
#include <strings.h>
#include <gtk/gtk.h>
#include "device.h"

static GSList *devlist = NULL;

/*
 * LookupDevice
 *
 * Lookup the device in the list of devices
 */
typDevice *LookupDevice (char *sName)
{
    GSList    *node;
    typDevice *dev;

    /* --- Go through the list of nodes. --- */
    for (node = devlist; node; node = node->next) {

        /* --- Get the data --- */
        dev = (typDevice *) node->data;

        /* --- This what we're looking for? --- */
        if (!strcmp (dev->sName, sName)) {
            return (dev);
        }
    }
    return (NULL);
}


/*
 * AddDevice
 *
 * Add a new device to the list of devices and
 * return it so it can be initialized.
 */
typDevice *AddDevice ()
{
    typDevice *dev;

    /* --- Create the new device --- */
    dev = (typDevice *) g_malloc (sizeof (typDevice));

    /* --- Add the item to the list --- */
    devlist = g_slist_append (devlist, dev);

    /* --- Return the new item --- */
    return (dev);
}


/*
 * UpdateDevice
 *
 * Updates the information about a device.  If the 
 * device does not currently exist, it's created and
 * initialized.
 *
 * sDevName - device name.  "eth0:", "cpu1", etc.
 * nValue - value at this time.
 * nType - DELTA/ACTUAL.  
 *         ACTUAL means the value is "as-is"
 *         DELTA means that the value is relative to
 *               the previous value and needs to be
 *               computed knowing that.
 * nMax - This is the normal max value to be displayed.
 */
void UpdateDevice (char *sDevName, long nValue, int nType, long nMax)
{
    typDevice  *dev;
    int        i;

    /* --- No name?  Get outta here! --- */
    if (sDevName == NULL) return;

    /* --- Look up the device by it's name --- */
    dev = LookupDevice (sDevName);

    /* --- Do we have information about the device? --- */
    if (dev) {

        /* --- Just add a value. --- */
        NewValue (dev, nValue, FALSE);
    } else {
        /* --- Don't have the device! --- */

        /* --- Create a new device. --- */
        dev = AddDevice ();

        /* --- Initialize the values. --- */
        dev->sName = strdup (sDevName);
        dev->nType = nType;
        dev->nMax = nMax;

        /* --- Clear out values. --- */
        for (i = 0; i < MAX_VALUES; i++) {

            dev->nValues[i] = (typHistoryValue) 0;
        }    

        /* --- Update with current position --- */
        NewValue (dev, nValue, TRUE);
    }
}


/*
 * UpdateExistingDevice
 *
 * If we know that the device exists, we use this
 * routine since it requires fewer parameters for me
 * to type and for the computer to push/pull from the
 * stack.
 */
void UpdateExistingDevice (char *sDevName, long nValue)
{
    typDevice  *dev;

    /* --- Name has to have a value. --- */
    if (sDevName == NULL) return;

    /* --- Look up the device name --- */
    dev = LookupDevice (sDevName);

    /* --- If the device exists.. --- */
    if (dev) {

        /* --- Add the value. --- */
        NewValue (dev, nValue, FALSE);
    }
}


/*
 * NewValue
 * 
 * Routine to add a value to the list of values 
 * being stored for the device.  Also must shift the
 * values over by one every time we add another value.
 *
 * dev - device to add a value to
 * value - value to add to the device.
 * bInit - Is this the first value to be added to the device?
 */
void NewValue (typDevice *dev, 
              typHistoryValue value,
              int bInit)
{
    int    i;

    /* --- Shift values down --- */
    for (i = MAX_VALUES-2; i >=0; i--) {

        dev->nValues[i+1] = dev->nValues[i];
    }    

    /* --- If this is NOT initialization --- */
    if (!bInit) {

        /* --- Add new value to the end --- */
        if (dev->nType == DELTA) {

            /* --- New value is a delta - diff between
             *     current value and previous value.
             */
            dev->nValues[0] = value - dev->nLast;
        } else {
            /* --- Use value as-is. --- */
            dev->nValues[0] = value;
        }
    }

    /* --- Store last value. --- */
    dev->nLast = value;

    /* --- Calculate the max value. --- */
    if (dev->nValues[0] > dev->nMax) {
        dev->nMax = dev->nValues[0];
    }
}


