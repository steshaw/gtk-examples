/*
 * --- Flag for marking the bombs.
 */ 
static char *xpm_flag[] = {
"12 12 4 1",
"  c None",
"X c #000000",
"R c #FF0000",
"r c #AA0000",
"            ",
"  RRRRRRR   ",
"  RRRRRrr   ",
"  RRRrrrr   ",
"  Rrrrrrr   ",
"        X   ",
"        X   ",
"        X   ",
"        X   ",
"        X   ",
"       XXX  ",
"            ",
};


/*
 * --- A bomb.  Ooops, you're not as smart as you thought.
 */
static char *xpm_bomb[] = {
"12 12 4 1",
"  c None",
"X c #000000",
"R c #FF0000",
"r c #AA0000",
"            ",
"     X      ",
"  X  X  X   ",
"   XXXXX    ",
"   XXXXX    ",
" XXXXXXXXX  ",
"   XXXXX    ",
"   XXXXX    ",
"  X  X  X   ",
"     X      ",
"            ",
"            ",
};


/*
 * --- Wrong move!
 */
static char *xpm_bigx[] = {
"12 12 4 1",
"  c None",
"X c #000000",
"R c #FF0000",
"r c #AA0000",
"RRR      RRR",
" RRR    RRR ",
"  RRR  RRR  ",
"   RRRRRR   ",
"    RRRR    ",
"    RRRR    ",
"    RRRR    ",
"   RRRRRR   ",
"  RRR  RRR  ",
" RRR    RRR ",
"RRR      RRR",
"            ",
};


/*
 * --- Bitmap of a smile
 */ 
static char *xpm_smile[] = {
"16 16 4 1",
"  c None",
". c #000000",
"X c #FFFF00",
"r c #AA0000",
"     ......     ",
"   ..XXXXXX..   ",
" ..XXXXXXXXXX.  ",
" .XXXXXXXXXXXX. ",
" .XX..XXXX..XX. ",
".XXX..XXXX..XXX.",
".XXXXXXXXXXXXXX.",
".XXXXXXXXXXXXXX.",
".XXXXXXXXXXXXXX.",
".XXXXXXXXXXXXXX.",
" .XX.XXXXXX.XX. ",
" .XXX......XXX. ",
"  .XXXXXXXXXX.  ",
"   ..XXXXXX..   ",
"     ......     ",
"                ",
};


/*
 * --- frown.  You lost.
 */
static char *xpm_frown[] = {
"16 16 4 1",
"  c None",
". c #000000",
"X c #FFFF00",
"r c #AA0000",
"     ......     ",
"   ..XXXXXX..   ",
" ..XXXXXXXXXX.  ",
" .XXXXXXXXXXXX. ",
" .XX.X.XX.X.XX. ",
".XXXX.XXXX.XXXX.",
".XXX.X.XX.X.XXX.",
".XXXXXXXXXXXXXX.",
".XXXXXXXXXXXXXX.",
".XXXXXXXXXXXXXX.",
" .XXX......XXX. ",
" .XX.XXXXXX.XX. ",
"  .XXXXXXXXXX.  ",
"   ..XXXXXX..   ",
"     ......     ",
"                ",
};


/*
 * --- We have a winner
 */ 
static char *xpm_winner[] = {
"16 16 4 1",
"  c None",
". c #000000",
"X c #FFFF00",
"r c #AA0000",
"     ......     ",
"   ..XXXXXX..   ",
" ..XXXXXXXXXX.  ",
" .XXXXXXXXXXXX. ",
" .XX...XX...XX. ",
".XX..........XX.",
".X.X...XX...X.X.",
"..XXXXXXXXXXXX..",
".XXXXXXXXXXXXXX.",
".XXXXXXXXXXXXXX.",
" .XX.XXXXXX.XX. ",
" .XXX......XXX. ",
"  .XXXXXXXXXX.  ",
"   ..XXXXXX..   ",
"     ......     ",
"                ",
};
