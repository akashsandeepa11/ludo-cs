#ifndef TYPES_H
#define TYPES_H
#include <stdbool.h>

struct piece{
    short location;
    short distance;
    short homeStraightDis;
    short capCount;
    bool isClockwise;
    char pieceName[3];
};

struct player {
    char playerName[7];
    short boardPiecesCount;
    short winPiecesCount;
    struct piece p[4];
};


// in player_behaviour.c Function prototypes
void yellowPlayer(short diceVal);
void bluePlayer(short diceVal);
void redPlayer(short diceVal);
void greenPlayer(short diceVal);
void baseToStart(short playerIndex);

// in logic.c Function prototypes
short rollDice(char *name);
bool tossCoin(short playerId, short pieceId);
short chooseFirstPlayer();
bool isSpecialLocation(short location, short *locArr, short len);
void capturePiece(short playerId, short pieceId, short opPlayerId, short opPieceId);
void capturePieceWrap(short index, short pieceId);
void updateLocation(short *locVariable, short playerID, short pieceId, short diceVal);
void updateLocationAndDistance(short index, short i, short diceVal);
void approchToHome(short diceVal, short index, short pieceId);
void winPlayer(short index, short i);
void movePlayer1(short playerId, short pieceId, short diceVal);
void movePlayer(short diceVal, short index);
void playerAction(short diceVal, short index);
void iterateTheGame();
void printPieceStates();
void printWinners();
void game();


#endif