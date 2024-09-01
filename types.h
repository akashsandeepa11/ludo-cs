#ifndef TYPES_H
#define TYPES_H
#include <stdbool.h>

struct mysteryCellData {
    short counter;
    short isEnergised;
};

struct piece{
    short location;
    short distance;
    short homeStraightDis;
    short capCount;
    bool isClockwise;
    char pieceName[3];
    struct mysteryCellData mysteryData;
    short blockPieces[4];
};

struct player {
    char playerName[7];
    short boardPiecesCount;
    short winPiecesCount;
    struct piece p[4];
};

short rollDice(char *name);
bool tossCoin(short playerId, short pieceId);
short chooseFirstPlayer();
bool isSpecialLocation(short location, short *locArr, short len);
void capturePiece(short playerId, short pieceId, short opPlayerId, short opPieceId);
bool captureIfAvailable(short playerId, short pieceId, short diceVal, bool ischeck);
void capturePieceByPlayerId(short playerId, short pieceId);
void updateLocation(short *locVariable, short playerID, short pieceId, short diceVal);
void updateLocationAndDistance(short index, short i, short diceVal);
void approchToHome(short diceVal, short index, short pieceId);
void printWinnerMessage(int index);
void winPlayer(short index, short i);
void yellowPlayer(short diceVal);
void bluePlayer(short diceVal);
void greenPlayer(short diceVal);
void redPlayer(short diceVal);
void baseToStart(short playerIndex);
void createMysteryCell();
void checkForMysteryCell(short playerId, short pieceId);
void toBawana(short playerId, short pieceId);
void toKotuwa(short playerId, short pieceId);
void toPitaKotuwa(short playerId, short pieceId);
void toBase(short playerId, short pieceId);
void toX(short playerId, short pieceId);
void toApproach(short playerId, short pieceId);
void movePlayerDirectly(short playerId, short pieceId, short diceVal);
void movePlayer(short diceVal, short index);
void playerAction(short diceVal, short index);
void iterateTheGame();
void printPieceStates();
void game();

#endif