/*
 * Directions the player can move
 */
enum {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN
};


/*
 * The cast of characters in the game.
 */
enum {
 HERO,     /* --- player --- */
 PERSON,   /* --- people on the surface --- */
 LANDER,   /* --- alien trying to pluck people --- */
 MUTANT,   /* --- alien that took person to top and mutated --- */
 MISSILE,  /* --- alien shoots at player --- */
 LASER,    /* --- player shoots at alien --- */
 EXPLOSION /* --- Somebody goes kaboom --- */
};


/*
 * Data structure used by all game units.
 */
typedef struct unit {

   int bDestroy;          /* --- Scheduled to be destroyed --- */
   int direction;         /* --- Which direction is unit going? --- */
   int type;              /* --- Unit type --- */
   float pctFiring;       /* --- Chance of firing on player --- */
   float x;               /* --- Position --- */
   float y;               /* --- Position --- */
   float vx;              /* --- Velocity --- */
   float vy;              /* --- Velocity --- */
   int life;              /* --- Life left --- */
   struct unit *lockunit; /* --- Units locked to it --- */

} typUnit;



typedef struct {

    int x;
    int y;

} typPoint;


typedef struct {

    typPoint start;
    typPoint peak;
    typPoint end;

} typMountain;


/*
 * The sprites used to draw the units on the screen
 */
typedef struct {

    char **xpm_data;    /* --- Original xpm data --- */
    GdkPixmap *pixmap;  /* --- Pixmap --- */
    GdkBitmap *mask;    /* --- Pixmap mask --- */
    int height;         /* --- Height of sprite --- */
    int width;          /* --- Width of sprite --- */

} typSprite;
