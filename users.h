//
// Created by nathnael on 5/27/2017.
//


#ifndef MINIMAX_USERS_H
#define MINIMAX_USERS_H

#include <iostream>
#include <vector>
using namespace std;

struct Board {
    int size;
    int level;
};


struct User {
    string username;
    string password;
    int gamesPlayed;
    int gamesLost;
    int gamesWon;
    float successRate;
    vector<int> boardsolved;
};


// Getch function
#include <termios.h>
#include <stdio.h>

static struct termios old, newt;

/* Initialize new terminal i/o settings */
void initTermios(int echo) {
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    newt = old; /* make new settings same as old settings */
    newt.c_lflag &= ~ICANON; /* disable buffered i/o */
    newt.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
    tcsetattr(0, TCSANOW, &newt); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) {
    tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) {
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}

/* Read 1 character without echo */
char getch(void) {
    return getch_(0);
}



#endif //MINIMAX_USERS_H
