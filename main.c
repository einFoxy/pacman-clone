#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "console.h"

#define XYMAX 19
#define X 0
#define Y 1
#define true 1
#define false 0
#define GOSTCOUNT 4
#define MAXDOTS 164             //dots to collect to win
#define MAXHEALTH 3

#define MOVE_PER_MS 100
#define HITCOOLDONW 5000        //ms
#define GOSTRESPAWN 5000        //ms

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

#define gostyellow 0
#define gostgreen 1
#define gostpink 2
#define gostblue 3



void printmap();

void move(char input);

void pacmanset();

void gostset(bool powerup);

void checkdots(int *dots);

void tpcheck();

void wincheck(bool *run, int dots);

void endgame();

void hitcheck(bool *run, short *health);

void healthoutput(short health);

void checkpowerup(short *gostrunning);

void gostmove(short gost, long long *gostrunning, int *gostdirecion);

short map[XYMAX][XYMAX] = {
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,},
        {2,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,2,},
        {2,1,2,2,1,2,2,2,1,2,1,2,2,2,1,2,2,1,2,},
        {2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,},
        {2,1,2,2,1,2,1,2,2,2,2,2,1,2,1,2,2,1,2,},
        {2,1,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,1,2,},
        {2,2,2,2,1,2,2,2,1,2,1,2,2,2,1,2,2,2,2,},
        {2,2,2,2,1,2,1,1,1,1,1,1,1,2,1,2,2,2,2,},
        {2,2,2,2,1,2,1,2,2,2,2,2,1,2,1,2,2,2,2,},
        {3,1,1,1,1,1,1,2,4,4,4,2,1,1,1,1,1,1,3,},
        {2,2,2,2,1,2,1,2,2,2,2,2,1,2,1,2,2,2,2,},
        {2,2,2,2,1,2,1,1,1,1,1,1,1,2,1,2,2,2,2,},
        {2,2,2,2,1,2,2,2,1,2,1,2,2,2,1,2,2,2,2,},
        {2,1,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,1,2,},
        {2,1,2,2,1,2,1,2,2,2,2,2,1,2,1,2,2,1,2,},
        {2,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,2,},
        {2,1,2,2,1,2,2,2,1,2,1,2,2,2,1,2,2,1,2,},
        {2,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,2,},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,},
        };

//Koordinaten
short pacpos[2] = {9, 15};
short gostpos[GOSTCOUNT][2] = {{9,  7},
                               {8,  9},
                               {9,  9},
                               {10, 9}};




int main() {


    initConsole();
    setCursorType(0);

        printmap();
        srand(time(0));
        //Input var
        char input;

        //text befor geme
        textcolor(WHITE);
        while (!kbhit()) {
            gotoxy(3, XYMAX + 3);
            printf("Willkommen bei Pacaman!\n");
            gotoxy(3, XYMAX + 4);
            printf("Verwende WASD um dich zu bewegen!");
            delay(500);
            gotoxy(0, XYMAX + 3);
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < XMAX; ++j) {
                    printf(" ");
                }
                printf("\n");
            }
            gotoxy(0, 0);

            delay(50);

        }

        short health = MAXHEALTH;
        bool powerup = false;
        healthoutput(health);
        //Gost vars
        long long gostrunnig[GOSTCOUNT] = {clock(), GOSTRESPAWN + clock(), GOSTRESPAWN * 2 + clock(),
                                           GOSTRESPAWN * 3 + clock()};
        int gostdirection[GOSTCOUNT] = {0};
        gostset(powerup);
        //player var




        //gamestart
        long long lastmove = clock();
        long long lasthit = clock();
        int dots = 1;
        bool running = 1;

        while (running) { //Game

            //Keybord inputregistry
            if (kbhit())
                input = getch();
            //moving / renderning player
            if (lastmove + MOVE_PER_MS < clock()) {
                move(input);

                if (lasthit + HITCOOLDONW < clock()) {
                    hitcheck(&running, &health);
                    if (health == 0) {
                        endgame();
                        running = 0;
                    }
                }

                for (int i = 0; i < GOSTCOUNT; i++) {
                    gostmove(i, &gostrunnig, &gostdirection);
                }
                gostset(powerup);

                checkdots(&dots);
                lastmove = clock();
                if (lasthit + HITCOOLDONW < clock()) {
                    hitcheck(&running, &health);
                    if (health == 0) {
                        endgame();
                        running = 0;
                    }
                }
                wincheck(&running, dots);
            }


        }
        getch();
    while(1);
    return 0;
}

void printmap(){
    for(int i = 0; i < XYMAX; i++){
        for (int j = 0; j < XYMAX; ++j) {
            switch (map[i][j]) {
                case 4:
                case 0:
                    printf("  ");
                    break;
                case 1:
                    textcolor(WHITE);
                    printf(" .");
                    break;
                case 2:
                    textcolor(BLUE);
                    printf(" %c",220);
                    break;
                case 3:
                    textcolor(BLUE);
                    printf(" T");
                    break;
                default:
                    textcolor(WHITE);
                    printf(" ?"); // for debuging: if n wrong number is in the Array
                    break;
            }
        }
        printf("\n");
    }
    pacmanset();
}

void move(char input){
        switch(input) {
            case 'w':
                if (map[pacpos[Y] - 1][pacpos[X]] != 2 && pacpos[Y] > 0) {
                    gotoxy(pacpos[X] * 2, pacpos[Y]);
                    printf("  ");
                    --pacpos[Y];
                }
                break;

            case 's':
            if (map[pacpos[Y] + 1][pacpos[X]] != 2 && pacpos[Y] < XYMAX - 1) {
                gotoxy(pacpos[X] * 2, pacpos[Y]);
                printf("  ");
                ++pacpos[Y];
            }
            break;
            case 'd':
            if (map[pacpos[Y]][pacpos[X] + 1] != 2 && pacpos[X] < XYMAX - 1) {
                gotoxy(pacpos[X] * 2, pacpos[Y]);
                printf("  ");
                ++pacpos[X];
            }
            break;
            case 'a':
            if (map[pacpos[Y]][pacpos[X] - 1] != 2 && pacpos[X] > 0) {
                gotoxy(pacpos[X] * 2, pacpos[Y]);
                printf("  ");
                --pacpos[X];
            break;
        }

        }
        pacmanset();
        tpcheck();

}

void pacmanset(){
    gotoxy(pacpos[X]*2,pacpos[Y]);
    textcolor(YELLOW);
    printf(" %c",2);
}

void gostset(bool powerup){

        for (int i = 0; i < GOSTCOUNT; ++i) {
            gotoxy(gostpos[i][X] * 2, gostpos[i][Y]);
            if(powerup == true)
                textcolor(BLUE);
            else {
                if (i == 1)
                    textcolor(GREEN);
                else
                    textcolor(i + 3); // collor Definitions are Number, defined in console.h
            }
            printf(" %c", 1);
        }
    }

void checkdots(int *dots){
    if(map[pacpos[Y]][pacpos[X]] == 1){
        map[pacpos[Y]][pacpos[X]] = 0;
        *dots += 1;
    }
}

void tpcheck(){
    if(pacpos[X] == 0 && pacpos[Y] == 9){
        gotoxy(pacpos[X]*2, pacpos[Y]);
        textcolor(BLUE);
        printf(" T");
        pacpos[X] = XYMAX - 2;}
    else if(pacpos[X] == XYMAX - 1  && pacpos[Y] == 9){
        gotoxy(pacpos[X]*2, pacpos[Y]);
        textcolor(BLUE);
        printf(" T");
        pacpos[X] = 1;
    }
    pacmanset();
}

void wincheck(bool *run, int dots){
    if(dots == MAXDOTS){
        textcolor(WHITE);
        int minwamimationtime = clock() + 10000;
        while(!kbhit() || minwamimationtime > clock()){
            gotoxy(3, XYMAX + 3);
            printf("YOU WIN!");
            delay(500);
            gotoxy(3, XYMAX + 3);
            for (int i = 0; i < XMAX; ++i)
                printf(" ");
            delay(50);
        }
        *run = 0;
    }
}

void endgame(){
    textcolor(WHITE);
    while(!kbhit()){
        gotoxy(3, XYMAX + 3);
        printf("YOU LOSE!");
        delay(500);
        gotoxy(3, XYMAX + 3);
        for (int i = 0; i < XMAX; ++i)
            printf(" ");
        delay(50);
    }
}

void hitcheck(bool *run, short *health){
    for(int i = 0; i < GOSTCOUNT; i++)
    {
        if(pacpos[X] == gostpos[i][X] && pacpos[Y] == gostpos[i][Y]) {
            --(*health);
            healthoutput(*health);
        }
    }
}

void healthoutput(short health){
    gotoxy(XYMAX*2+3, 2);
    textcolor(RED);
    for (int i = 0; i < health; ++i) {
        printf("<3 ");
    }
    for (int j = 0; j < (MAXHEALTH - health); ++j) {
        printf("   ");
    }

}

void gostmove(short gost, long long *gostrunning, int *gostdirecion){
    if(*(gostrunning+gost) < clock()) {

        //debug
        /*
        gotoxy(YMAX-1,YMAX-1);
        printf("time: %d", clock());

        for (int i = 0; i < GOSTCOUNT; ++i) {
            gotoxy(YMAX-1,YMAX-(5-i));
            printf("gost %d: %d\n", i, *(gostdirecion+i));
        }
         */


        //randomices movment
        int zz = rand()%100;
        gotoxy(YMAX-10, YMAX-1);
        printf("%d", zz);
        if(zz < 5) { //UP
            *(gostdirecion+gost) = UP;
        }
        else if(zz < 10){ // RIGHT
            *(gostdirecion+gost) = RIGHT;
        }
        else if(zz < 15){ // DOWN
            *(gostdirecion+gost) = DOWN;
        }
        else if(zz < 20){ // LEFT
            *(gostdirecion+gost)=LEFT;
        }
        //else => direction stays teh same. It is intendet that so that it is more likly to not switch the direction

        //execute movment
        if(map[gostpos[gost][Y]][gostpos[gost][X]] == 4){
            gotoxy(gostpos[gost][X]*2, gostpos[gost][Y]);
            printf("  ");
            gostpos[gost][X] = 9;
            gostpos[gost][Y] = 7;
        }
        switch (*(gostdirecion+gost)) {
            case UP:
                if(map[gostpos[gost][Y]-1][gostpos[gost][X]] == 2 || map[gostpos[gost][Y]-1][gostpos[gost][X]] == 3 )
                    gostmove(gost, gostrunning, gostdirecion);
                else{
                    gotoxy(gostpos[gost][X]*2, gostpos[gost][Y]);
                    if(map[gostpos[gost][Y]][gostpos[gost][X]] == 1)
                        printf(" .");
                    else
                        printf("  ");
                    gostpos[gost][Y]--;
                }
                break;

            case RIGHT:
                if(map[gostpos[gost][Y]][gostpos[gost][X]+1] == 2 || map[gostpos[gost][Y]][gostpos[gost][X]+1] == 3 )
                gostmove(gost, gostrunning, gostdirecion);
                else{
            gotoxy(gostpos[gost][X]*2, gostpos[gost][Y]);
            if(map[gostpos[gost][Y]][gostpos[gost][X]] == 1)
                printf(" .");
            else
                printf("  ");
            gostpos[gost][X]++;
        }
                break;
            case DOWN:
                if(map[gostpos[gost][Y]+1][gostpos[gost][X]] == 2 || map[gostpos[gost][Y]+1][gostpos[gost][X]] == 3 )
                gostmove(gost, gostrunning, gostdirecion);
                else{
            gotoxy(gostpos[gost][X]*2, gostpos[gost][Y]);
            if(map[gostpos[gost][Y]][gostpos[gost][X]] == 1)
                printf(" .");
            else
                printf("  ");
            gostpos[gost][Y]++;
            }
                break;
            case LEFT:
                if(map[gostpos[gost][Y]][gostpos[gost][X]-1] == 2 || map[gostpos[gost][Y]][gostpos[gost][X]-1] == 3 )
                gostmove(gost, gostrunning, gostdirecion);
                else{
            gotoxy(gostpos[gost][X]*2, gostpos[gost][Y]);
            if(map[gostpos[gost][Y]][gostpos[gost][X]] == 1)
                printf(" .");
            else
                printf("  ");
            gostpos[gost][X]--;
            }
                break;
            default:  break;
        }
    }
}

