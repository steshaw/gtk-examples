
/*
 * File: defender.c
 * Auth: Eric Harlow
 *
 * Sort of like the 1980's game defender, but not a 
 * complete version.
 *
 */

#include <stdlib.h>
#include <gtk/gtk.h>
#include "defender.h"
#include "defproto.h"
#include "mutant.h"
#include "math.h"

/*
 * How many people and aliens should the game start
 * out with?
 */
#define START_ALIENS 10
#define START_PEOPLE 10

/*
 * These are for the acceleration, of the player/hero, the
 * effect of friction to slow him down, and the maximum
 * velocity that can be attained.
 */
#define FRICTION .5
#define MAX_SPEED 16
#define ACCELERATION 3.5    

/*
 * Of course, the laser has to be configurable too.  The 
 * laser beam has a speed (how fast it travels out) and 
 * a length (primarily for show).
 */
#define LASER_LENGTH 60
#define LASER_SPEED 60

/*
 * Range of the game.
 */
#define X_RANGE 2000

/*
 * How high the mountain peaks are.
 */
#define MAX_PEAK 150

/*
 * The number of mountain peaks generated.
 */
#define NUM_PEAKS 10 

/*  
 * --- Trigger happiness 
 *
 * The mutants are much more trigger happy than the landers
 * are. 
 */
#define LANDER_TRIGGER_PCT 3
#define MUTANT_TRIGGER_PCT 6

/*
 * During an explosion, we use eight sprites spiraling away
 * from where the item exploded.  These are the eight 
 * directions.
 */
int x_exp[] = {1, 1, 0, -1, -1, -1, 0, 1};
int y_exp[] = {0, 1, 1, 1, 0, -1, -1, -1};

 

/*
 * How big is the radar screen.
 */
#define RADAR_WIDTH 200
#define RADAR_HEIGHT 50

/*
 * How big are other things in the screen
 */
#define GAME_HEIGHT 220
#define BOTTOM_HEIGHT 30
#define PERSON_HEIGHT (RADAR_HEIGHT+GAME_HEIGHT+7)

/*
 * Colors used for drawing
 */
GdkGC *penGreen = NULL;
GdkGC *penWhite = NULL;
GdkGC *penPurple = NULL;
GdkGC *penRed = NULL;

/*
 * variables used in calculations
 */
int nShipAdjustment;
int nRelativeAdjustment;
int nScreenWidth;

/*
 * Define the sprites for the cast of characters.
 */
typSprite sprite_man[1]     = { { xpm_man, NULL, NULL, 0, 0 } };
typSprite sprite_ship1[1]   = { { xpm_ship1, NULL, NULL, 0, 0 } };
typSprite sprite_ship2[1]   = { { xpm_ship2, NULL, NULL, 0, 0 } };
typSprite sprite_lander[1]  = { { xpm_lander, NULL, NULL, 0, 0 } };
typSprite sprite_mutant[1]  = { { xpm_mutant, NULL, NULL, 0, 0 } };
typSprite sprite_missile[1] = { { xpm_missile, NULL, NULL, 0, 0 } };

/*
 * The ship could be pointing left or right (sprite_ship1,
 * sprite_ship2) - however, sprite_ship is what's used to 
 * draw it.  If the ship changes direction, then this 
 * should also be adjusted.
 */
typSprite *sprite_ship = sprite_ship1;


/*
 * List of mountain peaks.
 */
GList *terrainList = NULL;

/*
 * All the units in the game
 */
GList *unitList = NULL;

/*
 * The hero.
 */
typUnit *hero = NULL;


/*
 * Prototypes.
 */
GdkColor *NewColor (long red, long green, long blue);
GdkGC *GetPen (GdkColor *c);
void GetWidthHeight (gchar **xpm, int *width, int *height);
void DisplaySprite (GtkWidget *drawing_area, typSprite *sprite, int x, int y);


/*
 * UnitScreenX
 *
 * Convert the unit's X coordinates to screen coordinates 
 * relative to the hero's coordinates.
 */
int UnitScreenX (typUnit *unit)
{
    int xPos;

    /* --- Adjust -x- if out of range --- */
    if (unit->x < 0) {
        unit->x += X_RANGE;
    } else if (unit->x > X_RANGE) {
        unit->x -= X_RANGE;
    }

    /* --- Make it relative --- */
    xPos = (int) (unit->x - nRelativeAdjustment);

    /* --- Readjust -x- if out of range --- */
    if (xPos < 0) xPos += X_RANGE;
    if (xPos > X_RANGE) xPos -= X_RANGE;

    return (xPos);
}


/*
 * ScreenX
 *
 * Take the game -x- value and convert it to the screen 
 * -x- value. 
 */
int ScreenX (int x)
{
    int xPos;

    /* --- Adjust if out of range --- */
    if (x < 0) {

        x += X_RANGE;

    } else if (x > X_RANGE) {

        x -= X_RANGE;
    }

    /* --- Make it absolute --- */
    xPos = (int) (x - nRelativeAdjustment);

    /* --- Readajust if out of range --- */
    if (xPos < (-(X_RANGE - nScreenWidth) / 2)) {

        xPos += X_RANGE;

    } else if (xPos > ((X_RANGE - nScreenWidth) / 2)) {

        xPos -= X_RANGE;
    }

    return (xPos);
}


/*
 * GameX
 *
 * Take a screen -x- value and convert it to the 
 * game coordinate -x- value.
 */
int GameX (int x)
{
    int xPos;

    /* --- Relative to the hero --- */
    xPos = (int) (x + nRelativeAdjustment);
  
    /* --- Make sure it's not out of range --- */
    if (xPos < 0) xPos += X_RANGE;
    if (xPos > X_RANGE) xPos -= X_RANGE;

    return (xPos);
}


/*
 * Move 
 *
 * Move a unit in the X direction by vx (velocity in x) and
 * in the Y direction by vy.
 */
void Move (typUnit *unit)
{

    /* --- Move the unit --- */
    unit->y += unit->vy;
    unit->x += unit->vx;

    /* --- But keep it within the world --- */
    if (unit->x < 0) unit->x += X_RANGE;
    if (unit->x > X_RANGE) unit->x -= X_RANGE;
}


/*
 * LoadPixmaps
 *
 * Load the image into the sprite.
 */
void LoadPixmaps (GtkWidget *widget, typSprite *sprites)
{

    /* --- Create a pixmap from the xpm data --- */
    sprites->pixmap = gdk_pixmap_create_from_xpm_d (
                               widget->window,
                               &sprites->mask,
                               NULL,
                               sprites->xpm_data);

    /* --- Get the width and height --- */
    GetWidthHeight (sprites->xpm_data, 
                    &sprites->width, 
                    &sprites->height);

}

/*
 * LoadImages
 *
 * Load up the images so that we can display them in the
 * game and configure the colors.
 */
void LoadImages (GtkWidget *window)
{
    /* --- Load up the images --- */
    LoadPixmaps (window, sprite_man);
    LoadPixmaps (window, sprite_ship1);
    LoadPixmaps (window, sprite_ship2);
    LoadPixmaps (window, sprite_lander);
    LoadPixmaps (window, sprite_mutant);
    LoadPixmaps (window, sprite_missile);

    /* --- Get the colors defined --- */
    penRed = GetPen (NewColor (0xffff, 0x8888, 0x8888));
    penGreen = GetPen (NewColor (0, 0xffff, 0));
    penPurple = GetPen (NewColor (0xffff, 0, 0xffff));
    penWhite = GetPen (NewColor (0xffff, 0xffff, 0xffff));
}


/*
 * CreateHero
 *
 * Create the typUnit for the player and initialize
 * it with the player settings.
 */
typUnit *CreateHero ()
{
    /* --- Allocate the memory --- */
    hero = g_malloc (sizeof (typUnit));

    /* --- Initialize player information --- */
    hero->bDestroy = FALSE;
    hero->direction = 1;
    hero->type = HERO;
    hero->x = 0;
    hero->y = 150;
    hero->vx = 0;
    hero->vy = 0;
    hero->lockunit = NULL;

    /* --- Return object. --- */
    return (hero);
}


/*
 * HeroFile
 *
 * Our hero is opening fire!  Create a laser shot
 * and add it to the global list of units.
 */
void HeroFire ()
{
    typUnit *laser;

    /* --- Create the laser --- */
    laser = (typUnit *) g_malloc (sizeof (typUnit));

    /* --- The direction is the same as the spaceship --- */
    laser->direction = hero->direction;

    laser->type = LASER;

    /* 
     * Move the starting point of the laser in front of the 
     * spaceship. 
     */
    if (laser->direction > 0) {
        laser->x = hero->x + (sprite_ship->width / 2);
    } else {
        laser->x = hero->x - (sprite_ship->width / 2);
    }
    laser->y = hero->y + 4;

    laser->vx = LASER_SPEED * hero->direction;
    laser->vy = 0;
    laser->lockunit = NULL;
    laser->bDestroy = 0;

    /* --- Laser bolt lasts through two movements. --- */
    laser->life = 2;

    /* --- Add the laser to the list of units --- */
    unitList = g_list_append (unitList, laser);
}


/*
 * HeroMove
 *
 * Move the player in the direction specified. 
 */
void HeroMove (int direction)
{
    switch (direction) {

        case MOVE_UP:
            hero->y -= 3;
            break;

        case MOVE_DOWN:
            hero->y += 3;
            break;

        case MOVE_LEFT:
            /* 
             * Make sure the ship is pointing in the
             * correct direction.
             */
            sprite_ship = sprite_ship2;
            hero->direction = -1;

            /* --- Speed up the ship --- */
            hero->vx -= ACCELERATION;
            break;

        case MOVE_RIGHT:
            /* 
             * Make sure the ship is pointing in the
             * correct direction.
             */
            sprite_ship = sprite_ship1;
            hero->direction = 1;

            /* --- Speed up the ship --- */
            hero->vx += ACCELERATION;
            break;
    }
}


/*
 * ApplyFriction
 *
 * Slow down gradually and make sure the player cannot go 
 * into warp speed by holding down the acceleration.
 */
void ApplyFriction ()
{
    /* --- Slow ship down --- */
    if (hero->vx > FRICTION) {
        hero->vx -= FRICTION;
    } else if (hero->vx < -FRICTION) {
        hero->vx += FRICTION;
    } else {

        /* --- Speed less than friction, we stop --- */
        hero->vx = 0;
    }

    /* --- Don't let the maximum speed be exceeded --- */
    if (hero->vx > MAX_SPEED) hero->vx = MAX_SPEED;
    if (hero->vx < -MAX_SPEED) hero->vx = -MAX_SPEED;
}


/*
 * CreatePerson
 *
 * Create the little people on the planet.
 */
typUnit *CreatePerson ()
{
    typUnit *person;

    /* --- Allocate the memory --- */
    person = g_malloc (sizeof (typUnit));

    /* --- Initialize the person --- */
    person->bDestroy = FALSE;
    person->direction = 0;
    person->type = PERSON;
    person->x = rand () % X_RANGE;
    person->y = PERSON_HEIGHT;
    person->vx = 0;
    person->vy = 0;
    person->lockunit = NULL;

    return (person);
}



/*
 * PlacePeople
 *
 * Create people randomly and place them on the screen
 */
void PlacePeople ()
{
    int i;
    typUnit *person;

    /* --- Create all the little people --- */
    for (i = 0; i < START_PEOPLE; i++) {

        /* --- Create a person --- */
        person = CreatePerson ();

        /* --- Add it to the list of units --- */
        unitList = g_list_append (unitList, person);
    }
}


/*
 * CreateAlien
 *
 * Create an alien lander 
 */
typUnit *CreateAlien ()
{
    typUnit *alien;

    /* --- Get the memory --- */
    alien = g_malloc (sizeof (typUnit));

    /* --- Initialize the structure --- */
    alien->bDestroy = FALSE;
    alien->pctFiring = LANDER_TRIGGER_PCT;
    alien->type = LANDER;
    alien->x = rand () % X_RANGE;
    alien->y = rand () % 50 + RADAR_HEIGHT ;
    alien->vx = 0;
    alien->vy = 0;
    alien->lockunit = NULL;

    return (alien);
}


/*
 * CreateMissile
 *
 * Missiles are what the aliens fire.  They are given
 * a fixed direction and last a limited amount of time
 * before they disappear.
 */
typUnit *CreateMissile (typUnit *alien, typUnit *hero)
{
    float flength;
    typUnit *missile;

    /* --- Allocate the memory --- */
    missile = (typUnit *) g_malloc (sizeof (typUnit));

    /* --- Inialize the structure --- */
    missile->bDestroy = FALSE;
    missile->pctFiring = 0;
    missile->type = MISSILE;
    missile->x = alien->x;
    missile->y = alien->y;

    /* --- Calculate missile velocity --- */
    missile->vx = (float) DistanceBetween (missile, hero) * 
                          Direction (missile, hero);
    missile->vy = (float) (hero->y - alien->y);

    /* 
     * Adjust missile velocity
     */
    flength = sqrt (missile->vx * missile->vx + missile->vy * missile->vy);
    if (flength < .1) flength = .1;
    flength /= 3;
    missile->vx /= flength;
    missile->vy /= flength;

    missile->lockunit = NULL;

    /* 
     * --- This missile has a life, when it goes to zero, 
     *     it gets destroyed.
     */
    missile->life = 60;

    return (missile);
}


/*
 * AddExplosion
 *
 * A unit was destroyed - we create an explosion at the 
 * unit's location
 */
void AddExplosion (typUnit *unit)
{
    typUnit *frag;
    int i;

    /* --- Creating eight fragments --- */
    for (i = 0; i < 8; i++) {

        /* --- Create a fragment of the explosion --- */
        frag = (typUnit *) g_malloc (sizeof (typUnit));

        /* --- Initialize the fragment --- */
        frag->bDestroy = FALSE;
        frag->pctFiring = 0;
        frag->type = EXPLOSION;
        frag->x = unit->x;
        frag->y = unit->y;
  
        /* --- Moves quite fast --- */
        frag->vx = x_exp[i] * 5;
        frag->vy = y_exp[i] * 5;

        frag->lockunit = NULL;

        /* --- Short life --- */
        frag->life = 20;

        /* --- Add it to the list of units --- */
        unitList = g_list_append (unitList, frag);
    }
}


/*
 * PlaceAliens
 *
 * Create the aliens.  
 */
void PlaceAliens ()
{
    int i;
    typUnit *alien;

    /* --- Create each of the aliens --- */
    for (i = 0; i < START_ALIENS; i++) {

        /* --- Create one alien --- */
        alien = CreateAlien ();

        /* --- Add it to the units list --- */
        unitList = g_list_append (unitList, alien);
    }
}


/*
 * DistanceBetween
 *
 * Calculate the distance between two units - but
 * only use the X direction to calculate the distance.
 * The distance is not just the difference between
 * the units X coordinates, but since the world 
 * wraps around, we have to consider the fact that
 * the distance between 1 and X_RANGE - 1 is not 
 * (X_RANGE-1) - 1
 */
int DistanceBetween (typUnit *u1, typUnit *u2)
{
    int nDistance;
    int nDistance2;

    /* --- Figure out which is greater --- */
    if (u1->x < u2->x) {
  
        /* --- Calculate distance both ways --- */
        nDistance = u2->x - u1->x;
        nDistance2 = X_RANGE + u1->x - u2->x;

    } else {
        /* --- Calculate distance both ways --- */
        nDistance = u1->x - u2->x;
        nDistance2 = X_RANGE + u2->x - u1->x;
    }
 
    /* --- Pick smaller of the distances --- */
    return ((nDistance < nDistance2) ? nDistance : nDistance2);
}

/*
 * Direction
 *
 * What is the -x- directory between the two units?
 * It's either going to be -1, 1, or 0. 
 */
int Direction (typUnit *u1, typUnit *u2)
{
    int nDistance;
    int nDistance2;

    if (u1->x < u2->x) {
  
        /* --- Distance each way --- */
        nDistance = u2->x - u1->x;
        nDistance2 = X_RANGE + u1->x - u2->x;

    } else {
        /* --- Distance each way --- */
        nDistance2 = u1->x - u2->x;
        nDistance = X_RANGE + u2->x - u1->x;
    }

    /* --- Direction depends on closer distance --- */
    return ((nDistance < nDistance2) ? 1 : -1);
}



/*
 * AttemptFiring
 *
 * Have the alien fire at the hero.
 */
void AttemptFiring (typUnit *unit)
{
    typUnit *missile;

    /* --- Close enough to shoot at --- */
    if (DistanceBetween (hero, unit) < (nScreenWidth / 2)) {
        
        /* --- Random chance - less than unit's chance --- */
        if ((rand () % 100) < unit->pctFiring) {

            /* --- Create the missle going after the hero --- */
            missile = CreateMissile (unit, hero);

            /* --- Add to the list of units --- */
            unitList = g_list_append (unitList, missile);
        }
    }
}


/*
 * AIModule
 *
 * Contains the logic for each of the units to move. 
 * Some units like landers are looking for people to
 * pick up.  Mutuants hunt down the player.  Missiles
 * just go until they die, etc.
 */
void AIModule (typUnit *unit)
{
    typUnit *tmp;
    int bestdist = 50000;
    typUnit *closest = NULL;
    GList *node;

    /*
     * Alien lander AI logic
     */
    if (unit->type == LANDER) {

        /* --- if the alien's locked on a person --- */
        if (unit->lockunit) {

            /* --- Move it up. --- */
            closest = unit->lockunit;
            unit->y -= .5;
            closest->y -= .5;
            
            /* --- If alien's made it to the top --- */
            if (unit->y - (sprite_lander[0].height / 2) < RADAR_HEIGHT) {

                /* --- Assimilate human! --- */
                unit->y = RADAR_HEIGHT + (sprite_lander[0].height / 2);
                unit->type = MUTANT;
                unit->pctFiring = MUTANT_TRIGGER_PCT;
                unit->lockunit = NULL;
                closest->bDestroy = TRUE;
            }

            return;
        }

        /* --- Any people nearby to snatch? --- */
        for (node = unitList; node; node = node->next) {

            tmp = (typUnit *) node->data;           
            if (tmp->type == PERSON && tmp->lockunit == NULL) {
    
                /* --- Look for a closer person --- */
                if (DistanceBetween (unit, tmp) < bestdist) {
                    closest = tmp;
                    bestdist = DistanceBetween (unit, tmp);
                }
            }
        }

        /* --- We're locked onto a target --- */
        if (bestdist <= 1) {

            /* --- Scootch it over a bit --- */
            unit->vx = 0;
            unit->x = closest->x;

            /* 
             * --- Check for a lock... --- 
             */

            if ((unit->y + (sprite_lander[0].height / 2) + .8) < 
                (closest->y - (sprite_man[0].height / 2))) {

                /* --- Come down on it. --- */
                unit->y += .5;

            } else if ((unit->y + (sprite_lander[0].height / 2)) > 
                (closest->y - (sprite_man[0].height / 2))) {
            
                unit->y -= .5;
            } else {

                /* --- Lock it in --- */
                unit->lockunit = closest;
                closest->lockunit = unit;
                closest->life = 20;
            }

        /* --- Anything in reasonable range? --- */
        } else if (bestdist < 20) {

            /* --- Move towards it --- */
            unit->vx = Direction (unit, closest);
            unit->x += unit->vx;
        } else {

            /* 
             * --- Nothing nearby.  Move in a random direction. 
             */
            if (unit->vx == 0) {
                if ((rand () % 2) == 0) {
                    unit->vx = 1;
                } else {
                    unit->vx = -1;
                }
            }
            unit->x += unit->vx;
        }
    
        /*
         * See if there is anything worth shooting at.
         */
        AttemptFiring (unit);

    /*
     * Mutant AI logic
     */
    } else if (unit->type == MUTANT) {

        /* 
         * --- Lets go crazy.  Mutant moves almost randomly yet 
         *     slowly towards the player.
         */
        unit->vx = Direction (unit, hero) * ((rand () % 4) + 1);
        unit->vy = rand () % 5 - 2;
        
        /* 
         * If the hero is within smelling distance, move towards
         * player in the -y- direction.
         */
        if (DistanceBetween (unit, hero) < 200) {
            if (unit->y < hero->y) unit->vy++;
            if (unit->y > hero->y) unit->vy--;
        }

        /* --- Finally move the unit --- */
        Move (unit);

        /* --- Let the mutant attempt firing --- */
        AttemptFiring (unit);

    /*
     * --- Missiles and explosions
     */
    } else if ((unit->type == MISSILE) || 
               (unit->type == EXPLOSION)) {

        /* --- These have a life.  Decrement it. --- */ 
        unit->life --;

        /* --- Move it. --- */
        Move (unit);

        /* --- When it reaches zero, destroy it --- */
        if (unit->life <= 0) {
            unit->bDestroy = TRUE;
        }

    /*
     * Person AI.
     */
    } else if (unit->type == PERSON) {

        /* 
         * Only time person moves by itself is when it's falling
         * from the sky after the alien carrying it has been shot.
         */
        if (unit->lockunit == NULL && unit->y < PERSON_HEIGHT) {

            /* --- Move it down --- */
            unit->y += 2;
        }
    }
}


/*
 * GetSprite
 * 
 * Get the sprite for the unit. 
 */
typSprite *GetSprite (typUnit *unit)
{
    typSprite *sprite;

    /* --- Get sprite --- */
    switch (unit->type) {

        case HERO:
            sprite = sprite_ship;
            break;
 
        case PERSON:
            sprite = sprite_man;
            break;

        case LANDER: 
            sprite = sprite_lander;
            break;

        case MUTANT: 
            sprite = sprite_mutant;
            break;

        case MISSILE:
        case EXPLOSION:
            sprite = sprite_missile;
            break;

        default:
            sprite = NULL;
            break;
    }
    return (sprite);
}


/*
 * AnyoneBetween
 * 
 * Is any unit between these coordinates?  Used by the 
 * laser when computing if anything has been hit. 
 */
typUnit *AnyoneBetween (int x1, int y1, int x2, int y2)
{
    GList *node;
    typUnit *unit;
    typUnit *closestUnit = NULL;
    int closestX;
    typSprite *sprite;
    int screenX;

    /* --- Check each unit --- */
    for (node = unitList; node; node = node->next) {

         unit = (typUnit *) node->data;

         /* --- if it's a bad guy --- */
         if ((unit->type == LANDER) || 
             (unit->type == PERSON) ||
             (unit->type == MUTANT)) {

             /* --- Get sprite and screen position --- */
             screenX = UnitScreenX (unit);
             sprite = GetSprite (unit);

             /* --- If we have a sprite --- */
             if (sprite) {

                 /* --- If in range using -x- --- */
                 if ((screenX >= x1 && screenX <= x2) || 
                     (screenX <= x1 && screenX >= x2)) {

                     /* --- And in range using -y- --- */
                     if ((unit->y - (sprite->height / 2) < y1) &&
                         unit->y + (sprite->height / 2) > y1) {
    
                         /*  
                          * Haven't found a unit or this one
                          * is closer.
                          */
                         if ((closestUnit == NULL) || 
                             (abs (x1 - screenX) < abs (x1 - closestX))) {

                             /* --- This is closst so far --- */
                             closestUnit = unit;
                             closestX = screenX;
                         }
                     }
                 }
             }
         }
     }
 
     return (closestUnit);
}


/*
 * UnitTop
 * 
 * Calculate the maximum height of the unit based on the 
 * radar screen and the sprite size.
 */
int UnitTop (typUnit *unit)
{
    typSprite *sprite;

    /* --- Get the sprite --- */
    sprite = GetSprite (unit);

    /* --- Add 1/2 sprite size to radar size. --- */
    return (RADAR_HEIGHT + (sprite[0].height / 2));
}

/*
 * UnitBottom
 * 
 * Calculate how low the unit can appear on the screen. 
 * It's partially based on the unit's size.
 */
int UnitBottom (typUnit *unit)
{
    typSprite *sprite;

    sprite = GetSprite (unit);
    return (GetGameHeight () - (sprite[0].height / 2));
}


/*
 *
 */
void AdjustSpriteHeight (typUnit *unit)
{
    typSprite *sprite;
    int nTop;
    int nBottom;

    /* --- Get the sprite for the unit --- */
    sprite = GetSprite (unit);
    if (sprite == NULL) return;

    /* --- Calculate the top and bottom range of unit --- */
    nTop = UnitTop (unit);
    nBottom = UnitBottom (unit);

    /* --- Don't let them go too high or too low --- */
    if (unit->y < nTop) {
        unit->y = nTop;
    } else if (unit->y > nBottom) {
        unit->y = nBottom;
    }
}


/*
 * DisplayOtherUnits
 *
 * Display all the units on the screen. First, we need 
 * to move each of the units to their new positions. 
 * Some of this is done in the AI module. 
 * 
 */
void DisplayOtherUnits (GdkPixmap *pixmap, GtkWidget *drawing_area)
{
    typUnit *unit;
    typUnit *unitHit;
    GList *node;
    int xPos;
    int xPosEnd;
    typSprite *sprite;

    /* --- Each unit in the list --- */
    for (node = unitList; node; node = node->next) {
 
        /* --- Get the unit --- */
        unit = (typUnit *) node->data;

        /*  
         * --- Run the AI module on it to move it --- 
         */
        AIModule (unit);

        /* 
         * If the unit was destroyed by the AI, 
         * don't draw the unit.
         */
        if (unit->bDestroy) {
            continue;
        }

        /* 
         * If there's no sprite for the unit, 
         * we can't draw it now, can we?
         */
        sprite = GetSprite (unit);
        if (sprite == NULL) continue;
 
        /* --- Where on the screen is it going? --- */
        xPos = UnitScreenX (unit);

        /* --- Make sure unit doesn't go out of bounds --- */
        AdjustSpriteHeight (unit);

        /* --- Finally draw unit --- */
        DisplaySprite (drawing_area, sprite, 
                   (int) (xPos - sprite[0].width / 2), 
                   (int) (unit->y - sprite[0].height / 2));
    }

    /* 
     * --- once everyone is painted, fire the lasers.
     */

    for (node = unitList; node; node = node->next) {

        unit = (typUnit *) node->data;

        /* --- If this is a laser --- */
        if (unit->type == LASER) {
 
            /* --- Get starting and ending positions --- */
            xPos = ScreenX ((int) unit->x);
            xPosEnd = xPos + LASER_LENGTH * unit->direction;

            /* --- See if anything was hit --- */
            unitHit = AnyoneBetween ((int) xPos, (int) unit->y, 
                                     (int) xPosEnd, (int) unit->y);
            if (unitHit) {

                /* --- Something was hit --- */

                /* --- Laser shot only goes this far --- */
                xPosEnd = UnitScreenX (unitHit);

                /* --- Destroy the unit --- */
                unitHit->bDestroy = TRUE;
                unit->bDestroy = TRUE;

                /* --- Special effects of destruction --- */
                AddExplosion (unitHit);
            }

            /* --- Draw the laser --- */
            gdk_draw_line (pixmap, penWhite,
                           xPos, unit->y, 
                           xPosEnd,
                           unit->y);
            
            /* --- Get real coordinates of laser --- */
            unit->x = GameX (xPosEnd);

            /* --- If laser has gone too far... --- */
            if (DistanceBetween (unit, hero) > nScreenWidth / 2) {

                /* --- destroy it --- */
                unit->bDestroy = TRUE;
            }
        }
    }
}



/*
 * Terrain functions
 *
 */


/*
 * MountainCompare
 * 
 * Compare function for sorting the mountain peaks.
 */
gint MountainCompare (const typMountain *m1, const typMountain *m2)
{
    return (m1->start.x - m2->start.x);
}


/*
 * AddMountain
 *
 * Add a mountain peak to the list we use to keep track
 * of the mountain peaks.
 */
GList *AddMountain (GList *mountainList, int peakx, int peaky)
{
    typMountain *node;
 
    node = (typMountain *) g_malloc (sizeof (typMountain));
    node->start.x = peakx - peaky;
    node->start.y = 0;
    node->peak.x = peakx;
    node->peak.y = peaky;
    node->end.x = peakx + peaky;
    node->end.y = 0;
 
    return (g_list_insert_sorted (mountainList, node, (GCompareFunc)MountainCompare));
}


/*
 * AddPoint
 *
 * Add a mountain peak at (x, y)
 */
GList *AddPoint (GList *terrainList, int x, int y)
{
    typPoint *p;

    /* --- Allocate the memory --- */
    p = (typPoint *) g_malloc (sizeof (typPoint));

    /* --- Initialize the point --- */
    p->x = x;
    p->y = y;

    /* --- Add the point to the list --- */
    terrainList = g_list_append (terrainList, p);

    return (terrainList);
}


/*
 * GenerateTerrain
 *
 * Create random mountain peaks that are used to generate the
 * background terrain.
 */
void GenerateTerrain ()
{
    int peakx;
    int peaky;
    GList *mountainList = NULL;
    GList *node;
    typMountain *prevMountain;
    typMountain *mtn;
    int i;

    /* --- Compute the peaks --- */
    for (i = 0; i < NUM_PEAKS; i++) {
        peakx = rand () % X_RANGE;
        peaky = rand () % MAX_PEAK;
 
        mountainList = AddMountain (mountainList, peakx, peaky);
    }

    prevMountain = NULL;

    terrainList = AddPoint (terrainList, 0, 0);

    /* --- Compute the lines based on the peaks --- */
    for (node = mountainList; node; node = node->next) {
  
        mtn = (typMountain *) node->data;

        /* --- First mountain --- */
        if (prevMountain == NULL) {
            terrainList = AddPoint (terrainList, mtn->start.x, mtn->start.y);
            terrainList = AddPoint (terrainList, mtn->peak.x, mtn->peak.y);
            prevMountain = mtn;

        /* --- Don't cross paths --- */
        } else if (prevMountain->end.x < mtn->start.x) {
 
            terrainList = AddPoint (terrainList, 
                                    prevMountain->end.x, 
                                    prevMountain->end.y);
            terrainList = AddPoint (terrainList, mtn->start.x, mtn->start.y);
            terrainList = AddPoint (terrainList, mtn->peak.x, mtn->peak.y);
            prevMountain = mtn;

        /* --- previous mountain eats this one --- */
        } else if (prevMountain->end.x > mtn->end.x) {
            
            /* --- Do nothing yet --- */
        } else {

            /* --- Mountains intersect --- */
            terrainList = AddPoint (terrainList, 
                                    (prevMountain->end.x + mtn->start.x) / 2,
                                    (prevMountain->end.x - mtn->start.x) / 2);
            terrainList = AddPoint (terrainList, mtn->peak.x, mtn->peak.y);
            prevMountain = mtn;
        }
    }
    terrainList = AddPoint (terrainList, 
                                    prevMountain->end.x, 
                                    prevMountain->end.y);
    terrainList = AddPoint (terrainList, X_RANGE, 0);
}



void StartGame ()
{
    unitList = NULL;

    /* --- Create our hero's ship --- */
    hero = CreateHero ();

    /* --- Generate the map --- */
    GenerateTerrain ();

    /* --- Place the people --- */
    PlacePeople ();

    /* --- Place aliens --- */
    PlaceAliens ();   
}




/*
 * DrawMountainSlope
 *
 * Draw the mountains in the background of the game.
 */
void DrawMountainSlope (GdkPixmap *pixmap, 
                        typPoint *lastPt, 
                        typPoint *pt, 
                        int nTop, 
                        int nBottom)
{
    int x1;
    int x2;
    int nStart;
    int nEnd;
    int nHeight;

    nHeight = nBottom - nTop;
    nStart = hero->x - (nScreenWidth / 2);
    nEnd = hero->x + (nScreenWidth / 2);

    x1 = ScreenX (lastPt->x);
    x2 = ScreenX (pt->x);

    if ((x2 < 0) || 
        (x1 > nScreenWidth)) {

        /* ---  Do nothing --- */

    } else {

        /* --- Draw point --- */
        gdk_draw_line (pixmap, penWhite,
                   ScreenX (lastPt->x),
                   (int) (nBottom - ((lastPt->y * nHeight) / MAX_PEAK)),
                   ScreenX (pt->x),
                   (int) (nBottom - ((pt->y * nHeight) / MAX_PEAK)));
    }
}


void DrawMountains (GdkPixmap *pixmap, 
                    GtkWidget *drawing_area,
                    int nTop, int nBottom)
{
    typPoint *lastPt;
    typPoint *pt;
    GList *node;

    lastPt = NULL;

    /* --- where is the viewpoint from? --- */
    for (node = terrainList; node; node = node->next) {

        /* --- Get the point. --- */
        pt = (typPoint *) node->data;

        if (lastPt) {
   
            DrawMountainSlope (pixmap, lastPt, pt, nTop, nBottom);
        }
        lastPt = pt;
    }
}


void CalculateAdjustments (GtkWidget *drawing_area)
{
    nShipAdjustment = (drawing_area->allocation.width / 2);
    nRelativeAdjustment = hero->x - nShipAdjustment;
}


/*
 * DrawAllUnits
 *
 * Draw all the units
 */
void DrawAllUnits (GdkPixmap *pixmap, 
               GtkWidget *drawing_area)
{
    /*
     * Move and display the hero
     */
    ApplyFriction ();
    Move (hero);

    /* --- Keep him in-bounds. --- */
    AdjustSpriteHeight (hero);

    DisplaySprite (drawing_area, sprite_ship, 
                   nShipAdjustment - (sprite_ship->width / 2), 
                   (int) hero->y - (sprite_ship->height / 2));
    /*
     * Move and display everyone else
     */
    DisplayOtherUnits (pixmap, drawing_area);
}


/*
 * DrawRadar
 *
 * Draw all the units on the radar screen.  Of course, the 
 * radar screen has to be drawn and then the units have to
 * have their points converted to "radar" coordinates and 
 * little dots are drawn on the screen in different colors
 * to refect the type of each of the units in the game.
 */
void DrawRadar (GdkPixmap *pixmap, GtkWidget *drawing_area)
{
    int   nLeft;
    GList *node;
    typUnit *unit;
    int x, y;
    int nMin;
    int newx, newy;

    /* --- Get radar coordinates --- */
    nLeft = (drawing_area->allocation.width - RADAR_WIDTH) / 2;
    nMin = hero->x - (X_RANGE / 2);

    /* --- Clear out the rectangle --- */
    gdk_draw_rectangle (pixmap, drawing_area->style->white_gc,
                        FALSE,
                        nLeft, 0, RADAR_WIDTH, RADAR_HEIGHT);

    /*
     * --- Display all the units
     */
    for (node = unitList; node; node = node->next) {
    
        unit = (typUnit *) node->data;
        x = unit->x;
        y = unit->y;

        if (x > hero->x + (X_RANGE / 2)) {
             
            x -= X_RANGE;

        } else if (x < hero->x - (X_RANGE / 2)) {

            x += X_RANGE;
        }

        /* --- Convert -x- to radar coordinates --- */
        newx = (x - nMin) * RADAR_WIDTH / X_RANGE;

        /* --- Convert -y- to radar coordinates --- */
        newy = ((y - RADAR_HEIGHT) * (RADAR_HEIGHT - 6) / GAME_HEIGHT) + 2;

        switch (unit->type) {

            case PERSON:
                gdk_draw_rectangle (pixmap, penPurple,
                        TRUE, nLeft + newx-1, newy-1, 2, 2);
                break;

            case LANDER:
                gdk_draw_rectangle (pixmap, penGreen,
                        TRUE, nLeft + newx-1, newy-1, 2, 2);
                break;

            case MUTANT:
                gdk_draw_rectangle (pixmap, penRed,
                        TRUE, nLeft + newx-1, newy-1, 2, 2);
                break;

            case MISSILE:
            case EXPLOSION:
                gdk_draw_rectangle (pixmap, penWhite,
                        TRUE, nLeft + newx, newy, 1, 1);
                break;
        }
    }

    /*
     * --- Put the hero on the screen too.
     */

    /* --- Convert -x- to radar coordinates --- */
    newx = (hero->x - nMin) * RADAR_WIDTH / X_RANGE;

    /* --- Convert -y- to radar coordinates --- */
    newy = ((hero->y - RADAR_HEIGHT) * (RADAR_HEIGHT - 6) / GAME_HEIGHT) + 2;

    gdk_draw_rectangle (pixmap, penWhite, TRUE,
                        nLeft + newx-1, newy-1, 2, 2);
}


/*
 * DrawScreen
 *
 * This does a lot of work.  It draws the background and 
 * all the units, as well as the radar.  This routine is 
 * essentially the main loop that gets called by the 
 * timer every update frequency.
 */
void DrawScreen (GdkPixmap *pixmap, GtkWidget *drawing_area)
{
    /* --- Move player based on keys pressed --- */
    HandleKeysPressed ();

    /* --- Get the screen width --- */
    nScreenWidth = drawing_area->allocation.width;

    /* --- Figure out the offset of the player --- */
    CalculateAdjustments (drawing_area);

    /* --- clear pixmap (background image) --- */
    gdk_draw_rectangle (pixmap,
              drawing_area->style->black_gc,
              TRUE,
              0, 0,
              drawing_area->allocation.width,
              drawing_area->allocation.height);

    /* --- Draw top border and radar screen --- */
    gdk_draw_line (pixmap, drawing_area->style->white_gc, 
                   0, RADAR_HEIGHT, 
                   drawing_area->allocation.width, 
                   RADAR_HEIGHT);

    /* --- Oh those high peaks --- */
    DrawMountains (pixmap, drawing_area, 
                   drawing_area->allocation.height - 65,
                   drawing_area->allocation.height - BOTTOM_HEIGHT);
 
    /* --- Draw the characters --- */
    DrawAllUnits (pixmap, drawing_area); 

    /* --- Draw the units on the radar --- */
    DrawRadar (pixmap, drawing_area);

    /* --- Look for collisions --- */
    CollisionCheck ();

    /* --- Clean up those that got destroyed --- */
    FreeDestroyedUnits ();
}


/*
 * CollisionCheck
 *
 * Hero suffer a collision?  Check for a collision
 * but don't really do anything.  This isn't really
 * a game, but rather a demo on how is could be done.
 */
void CollisionCheck ()
{
    GList *node;
    typSprite *sprite;
    int hero_x;
    int unit_x;
    typUnit *unit;

    /* --- See if anything collides with the hero. --- */
    for (node = unitList; node; node = node->next) {

        /* --- Get the unit --- */
        unit = (typUnit *) node->data;

        /* --- Get the unit's sprite --- */
        sprite = GetSprite (unit);

        if (sprite == NULL) continue;
       
        /* --- Eliminate non-collisions --- */
        if (hero->y + (sprite_ship->height / 2) < 
            unit->y - (sprite->height / 2)) continue;

        if (hero->y - (sprite_ship->height / 2) > 
            unit->y + (sprite->height / 2)) continue;

        hero_x = UnitScreenX (hero);
        unit_x = UnitScreenX (unit);
        
        if (hero_x + (sprite_ship->width / 2) < 
            unit_x - (sprite->width / 2)) continue;

        if (hero_x - (sprite_ship->width / 2) > 
            unit_x + (sprite->width / 2)) continue;

        /* --- Hero collides here since everything failed. --- */
    }
}

 
/*
 * GetGameHeight
 *
 * Get the height of the game.
 */
int GetGameHeight ()
{

    return (RADAR_HEIGHT + GAME_HEIGHT + BOTTOM_HEIGHT);
}


/*
 * FreeDestroyedUnits
 *
 * The units are just marked as being destroyed.  This
 * routine goes through the list and removes all the units
 * that are marked as destroyed and frees up any memory
 * they're using.
 */
void FreeDestroyedUnits ()
{
    GList *node;
    GList *next;
    typUnit *unit;

    node = unitList; 
    while (node) {
  
        next = node->next;

        unit = (typUnit *) node->data;
   
        /* --- If it's to be destroyed --- */
        if (unit->bDestroy) {

            /* --- Remove from the list of units --- */
            unitList = g_list_remove (unitList, unit);

            /* --- if unit was locked to another --- */
            if (unit->lockunit) {

                /* --- Remove lock to this unit --- */
                unit->lockunit->lockunit = NULL;
            }
            g_free (unit);
        }

        node = next;
    }
}
