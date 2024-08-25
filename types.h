#ifndef TYPES_H
#define TYPES_H
#include <stdbool.h>

struct piece{
    short index;
    short location;
    short distance;
    short capCount;
    bool isClockwise;
    char pieceName[3];
};

struct player {
    short index;
    char playerName[7];
    short boardPiecesCount;
    short winPiecesCount;
    struct piece p[4];
};

short rollDice(char *name);
short chooseFirstPlayer(struct player *players);
void capturePiece(struct player *players, short index, short pieceId);
void movePlayer(struct player *players, short diceVal, short index, short *winners);
void baseToStart(struct player *player);
void winPlayer(struct player *players, short *winners, short index, short i);
void playerAction(struct player *players, short diceVal, short index, short *winners);
void iterateTheGame(struct player *players, short *winners);
void printPieceStates(struct player *players); 
void printWinners(struct player *players, short *winners);
void approchToHome(struct player *players, short diceVal, short index, short *winners, short pieceId);
void game();

#endif