/*
 * Auth: Eric Harlow
 * File: keys.c
 * Linux GUI Application development
 */

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "defender.h"
#include "defproto.h"


/* 
 * Movement 
 */
int keyLeft = 0;     
int keyRight = 0;
int keyUp = 0;
int keyDown = 0;

/*
 * Firing
 */
int keyFire = 0;

/*
 * AddKey
 *
 * Add the key to the keys that are being kept 
 * track of.  We only keep track of a few of the keys 
 * and we ignore the rest.
 */
void AddKey (GdkEventKey *event)
{

    switch (event->keyval) {

        /* --- Left arrow --- */
        case GDK_Left:
            keyLeft = TRUE;
            break;

        /* --- Right arrow --- */
        case GDK_Right:
            keyRight = TRUE;
            break;

        /* --- Up arrow --- */
        case GDK_Up:
            keyUp = TRUE;
            break;

        /* --- Down arrow --- */
        case GDK_Down:
            keyDown = TRUE;
            break;
 
        /* --- Space --- */
        case ' ':
            keyFire = TRUE;
            break;

        /* --- Ignore the rest --- */
        default:
            break;
    }
}


/*
 * RemoveKey
 *
 * If we're keeping track of a key and the key 
 * goes up, we flag it as not being pressed.
 */
void RemoveKey (GdkEventKey *event)
{
    switch (event->keyval) {

        case GDK_Left:
            keyLeft = FALSE;
            break;

        case GDK_Right:
            keyRight = FALSE;
            break;

        case GDK_Up:
            keyUp = FALSE;
            break;

        case GDK_Down:
            keyDown = FALSE;
            break;
 
        case ' ':
            keyFire = FALSE;
            break;

        default:
            break;
    }
}


/*
 * HandleKeysPressed
 *
 * When it comes time to move everyone, this 
 * routine is called to move/fire based on the
 * keys that are currently pressed.
 */
void HandleKeysPressed ()
{

    /* 
     * Try and move in each direction
     */
    if (keyLeft) {
        HeroMove (MOVE_LEFT);
    }

    if (keyRight) {
        HeroMove (MOVE_RIGHT);
    }

    if (keyUp) {
        HeroMove (MOVE_UP);
    }

    if (keyDown) {
        HeroMove (MOVE_DOWN);
    }

    /*
     * Try and fire.
     *
     * Of course, once we fire, we clear the flag so 
     * the player has to hit the key for multiple shots.
     * Otherwise they could just hold down the key.
     */
    if (keyFire) {

        /* --- Zap --- */
        HeroFire ();

        /* --- Oh my, key is good for only one shot. --- */
        keyFire = FALSE;
    }
}
