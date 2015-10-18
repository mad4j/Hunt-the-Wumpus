/*
 * Hunt the Wumpus 2015
 * by Daniele Olmisani <daniele.olmisani@gmail.com>
 *
 * compile:
 * gcc -Wall -std=c99 wumpus.c -o wumpus
 *
 * usage:
 * ./wumpus [-h] [-s seed] [-d]
 *
 * see also:
 * wumpus.c by Eric S. Raymond <esr@snark.thyrsus.com>
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>

static int arrows;
static int debug = 0;	

#define YOU	    0
#define WUMPUS	1
#define PIT1	2
#define PIT2	3
#define BATS1	4
#define BATS2	5
#define LOCS	6

static int loc[LOCS];

#define NOT	     0
#define WIN	     1
#define LOSE	-1

static int finished;

static int cave[20][3] =
{
    {1,4,7},
    {0,2,9},
    {1,3,11},
    {2,4,13},
    {0,3,5},
    {4,6,14},
    {5,7,16},
    {0,6,8},
    {7,9,17},
    {1,8,10},
    {9,11,18},
    {2,10,12},
    {11,13,19},
    {3,12,14},
    {5,13,15},
    {14,16,19},
    {6,15,17},
    {8,16,18},
    {10,17,19},
    {12,15,18},
};

int getnum(char* prompt) {

    int n;
    printf("%s: ", prompt);
    scanf("%d", &n);

    return n;
}

int getlet(char* prompt) {

    char c = '\n';

    printf("%s: ", prompt);
    while (c == '\n') {
        scanf("%c", &c);
    }

    return toupper(c);
}

void print_instructions() {

    printf(
        "WELCOME TO 'HUNT THE WUMPUS'\n"
        "THE WUMPUS LIVES IN A CAVE OF 20 ROOMS. EACH ROOM\n"
        "HAS 3 TUNNELS LEADING TO OTHER ROOMS. (LOOK AT A\n"
        "DODECAHEDRON TO SEE HOW THIS WORKS-IF YOU DON'T KNOW\n"
        "WHAT A DODECAHEDRON IS, ASK SOMEONE)\n"
        "\n"
        " HAZARDS:\n"
        " BOTTOMLESS PITS: TWO ROOMS HAVE BOTTOMLESS PITS IN THEM\n"
        " IF YOU GO THERE, YOU FALL INTO THE PIT (& LOSE!)\n"
        " SUPER BATS     : TWO OTHER ROOMS HAVE SUPER BATS. IF YOU\n"
        " GO THERE, A BAT GRABS YOU AND TAKES YOU TO SOME OTHER\n"
        " ROOM AT RANDOM. (WHICH MAY BE TROUBLESOME)\n"
        " WUMPUS:\n"
        " THE WUMPUS IS NOT BOTHERED BY HAZARDS (HE HAS SUCKER\n"
        " FEET AND IS TOO BIG FOR A BAT TO LIFT).  USUALLY\n"
        " HE IS ASLEEP.  TWO THINGS WAKE HIM UP: YOU SHOOTING AN\n"
        " ARROW OR YOU ENTERING HIS ROOM.\n"
        " IF THE WUMPUS WAKES HE MOVES (P=.75) ONE ROOM\n"
        " OR STAYS STILL (P=.25).  AFTER THAT, IF HE IS WHERE YOU\n"
        " ARE, HE EATS YOU UP AND YOU LOSE!\n"
        "\n"
        " YOU:\n"
        " EACH TURN YOU MAY MOVE OR SHOOT A CROOKED ARROW\n"
        " MOVING:  YOU CAN MOVE ONE ROOM (THRU ONE TUNNEL)\n"
        " ARROWS:  YOU HAVE 5 ARROWS.  YOU LOSE WHEN YOU RUN OUT\n"
        " EACH ARROW CAN GO FROM 1 TO 5 ROOMS. YOU AIM BY TELLING\n"
        "   THE COMPUTER THE ROOM#S YOU WANT THE ARROW TO GO TO.\n"
        "   IF THE ARROW CAN'T GO THAT WAY (IF NO TUNNEL) IT MOVES\n"
        "   AT RANDOM TO THE NEXT ROOM.\n"
        "     IF THE ARROW HITS THE WUMPUS, YOU WIN.\n"
        "     IF THE ARROW HITS YOU, YOU LOSE.\n"
        " WARNINGS:\n"
        " WHEN YOU ARE ONE ROOM AWAY FROM A WUMPUS OR HAZARD,\n"
        " THE COMPUTER SAYS:\n"
        " WUMPUS:  'I SMELL A WUMPUS'\n"
        " BAT   :  'BATS NEARBY'\n"
        " PIT   :  'I FEEL A DRAFT'\n"
        "\n"
    );
}

void show_room() {

    printf("\n");

    for (int k = 0; k < 3; k++) {

	   int room = cave[loc[YOU]][k];

	   if (room == loc[WUMPUS]) {
	       printf("I SMELL A WUMPUS!\n");
	   } else if (room == loc[PIT1] || room == loc[PIT2]) {
	       printf("I FEEL A DRAFT\n");
	   } else if (room == loc[BATS1] || room == loc[BATS2]) {
	       printf("BATS NEARBY!\n");
       }
    }

    printf("YOU ARE IN ROOM %d\n", loc[YOU]+1);

    printf("TUNNELS LEAD TO %d %d %d\n\n",
        cave[loc[YOU]][0]+1, 
        cave[loc[YOU]][1]+1, 
        cave[loc[YOU]][2]+1);
}

int move_or_shoot() {

    int c = -1;

    while ((c != 'S') && (c != 'M')) {
        c = getlet("SHOOT OR MOVE (S-M)");
    }

    return (c == 'S') ? 1 : 0;
}


void move_wumpus() {

    int k = rand() % 4;

    if (k < 3) {
       loc[WUMPUS] = cave[loc[WUMPUS]][k];
    }

    if (loc[WUMPUS] == loc[YOU]) {
        printf("TSK TSK TSK - WUMPUS GOT YOU!\n");
        finished = LOSE;
    }
}


void shoot() {

    int path[5];
    int scratchloc = -1;

    finished = NOT;

    int len = -1;
    while (len < 1 || len > 5) {
        len = getnum("NO. OF ROOMS (1-5)");
    }

    int k = 0;
    while (k < len) {
        path[k] = getnum("ROOM #") - 1;

        if ((k>1) && (path[k] == path[k-2])) {
            printf("ARROWS AREN'T THAT CROOKED - TRY ANOTHER ROOM\n");
            continue; 
       } 

       k++;
    }
 
    scratchloc = loc[YOU];

    for (int k = 0; k < len; k++) {

        if ((cave[scratchloc][0] == path[k]) ||
            (cave[scratchloc][1] == path[k]) ||
            (cave[scratchloc][2] == path[k])) {

            scratchloc = path[k];
        } else {
            scratchloc = cave[scratchloc][rand()%3];
        }

        if (scratchloc == loc[WUMPUS]) {

            printf("AHA! YOU GOT THE WUMPUS!\n");
            finished = WIN;

        } else if (scratchloc == loc[YOU]) {

            printf("OUCH! ARROW GOT YOU!\n");
            finished = LOSE;
        }

        if (finished != NOT) {
            return;
        }
    }

	printf("MISSED\n");

	move_wumpus();

	if (--arrows <= 0) {
	    finished = LOSE;
    }
}


void move() {

    int scratchloc = -1;
    while (scratchloc == -1) {

        scratchloc = getnum("WHERE TO")-1;

        if (scratchloc < 0 || scratchloc > 19) {
            scratchloc = -1;
            continue;
        }

        if ((cave[loc[YOU]][0] != scratchloc) &
            (cave[loc[YOU]][1] != scratchloc) &
            (cave[loc[YOU]][2] != scratchloc) &
            (loc[YOU] != scratchloc)) {

            printf("NOT POSSIBLE\n");

            scratchloc = -1;
            continue;
        }
    }
   
    loc[YOU] = scratchloc;

    while ((scratchloc == loc[BATS1]) || (scratchloc == loc[BATS2])) {
        printf("ZAP--SUPER BAT SNATCH! ELSEWHEREVILLE FOR YOU!\n");
        scratchloc = loc[YOU] = rand()%20;
    }

    if (scratchloc == loc[WUMPUS]) {
	   printf("... OOPS! BUMPED A WUMPUS!\n");
	   move_wumpus();
    } 

    if (scratchloc == loc[PIT1] || scratchloc == loc[PIT2]) {
	   printf("YYYYIIIIEEEE . . . FELL IN PIT\n");
	   finished = LOSE;
    }
}


void game_setup() {

    for (int j = 0; j < LOCS; j++) {

        loc[j] = -1;
        while (loc[j] < 0) {

            loc[j] = rand()%20;

            for (int k=0; k<j-1; k++) {
                if (loc[j] == loc[k]) {
                    loc[j] = -1;
                }
           }
       }
    }
}


void game_play() {

    arrows = 5;

    printf("HUNT THE WUMPUS\n");

    if (debug) {
        printf("Wumpus is at %d, pits at %d & %d, bats at %d & %d\n",
            loc[WUMPUS]+1,
            loc[PIT1]+1, loc[PIT2]+1,
            loc[BATS1]+1, loc[BATS2]+1);
    }

    finished = NOT;
    while (finished == NOT) {

        show_room();
        if (move_or_shoot()) {
            shoot();
        } else {
            move();
        }
    }

    if (finished == WIN) {
        printf("HEE HEE HEE - THE WUMPUS'LL GET YOU NEXT TIME!!\n");
    }

    if (finished == LOSE) {
        printf("HA HA HA - YOU LOSE!\n");
    }

    int c = getlet("NEW GAME (Y-N)");

    if (c == 'N') {
        exit(0);
    }
  
}


void handle_params(int argc, char* argv[]) {

    int c;

    while ((c = getopt(argc, argv, "s:dh")) != -1) {
        switch (c) {
        case 's':
            srand(atoi(optarg));
            break; 
        case 'd':
            debug = 1;
            break;
        case 'h':
        default:
            printf("usage: ./%s [-h] [-d] [-s seed]\n", argv[0]);
            exit(1);
        }
    }
}


int main(int argc, char* argv[]) {

    srand(time(0));
    handle_params(argc, argv);
    
    int c = getlet("INSTRUCTIONS (Y-N)");

    if (c == 'Y') {
	   print_instructions();
    }

    do { 

        game_setup();
        game_play();

    } while (getlet("NEW GAME (Y-N)") != 'N');

    return 0;
}
