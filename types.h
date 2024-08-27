#ifndef TYPES_H
#define TYPES_H
#include <stdbool.h>

struct piece{
    short index;
    short location;
    short distance;
    short homeStraightDis;
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
bool tossCoin(short playerId, short pieceId);
short chooseFirstPlayer();
bool isSpecialLocation(short location, short *locArr, short len);
void capturePiece(short index, short pieceId);
void playerAction(short diceVal, short index);
void movePlayer(short diceVal, short index);
void approchToHome(short diceVal, short index, short pieceId);
void baseToStart(short playerIndex);
void winPlayer(short index, short i);
void playerAction(short diceVal, short index);
void iterateTheGame();
void printPieceStates();
void printWinners();
void game();

#endif