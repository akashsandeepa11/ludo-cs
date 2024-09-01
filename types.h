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

};

struct player {
    char playerName[7];
    short boardPiecesCount;
    short winPiecesCount;
    struct piece p[4];
};

// ===============================Function Prototypes===================================

// Random Value Functions 
short rollDice(char *name);
bool tossCoin(short playerId, short pieceId);

// Genaral Functions
short chooseFirstPlayer();
bool isSpecialLocation(short location, short *locArr, short len);
void updateLocation(short *locVariable, short playerID, short pieceId, short diceVal);
void updateLocationAndDistance(short index, short i, short diceVal);

// Piece Moving Functions
void baseToStart(short playerIndex);
void movePlayerDirectly(short playerId, short pieceId, short diceVal);
void movePlayer(short diceVal, short index);
void approchToHome(short diceVal, short index, short pieceId);
void winPlayer(short index, short i);

// Capture Functions
void capturePiece(short playerId, short pieceId, short opPlayerId, short opPieceId);
bool captureIfAvailable(short playerId, short pieceId, short diceVal, bool ischeck);
void capturePieceByPlayerId(short index, short pieceId);

// Player Functions
void yellowPlayer(short diceVal);
void bluePlayer(short diceVal);
void greenPlayer(short diceVal);
void redPlayer(short diceVal);

// Mytery Cell Functions
void checkForMysteryCell(short playerId, short pieceId);
void createMysteryCell();
void baseToStart(short playerIndex);
void createMysteryCell();
void checkForMysteryCell(short playerId, short pieceId);

void toBawana(short playerId, short pieceId);
void toKotuwa(short playerId, short pieceId);
void toPitaKotuwa(short playerId, short pieceId);
void toBase(short playerId, short pieceId);
void toX(short playerId, short pieceId);
void toApproach(short playerId, short pieceId);

// Game Flow Function
void movePlayerDirectly(short playerId, short pieceId, short diceVal);
void movePlayer(short diceVal, short index);
void playerAction(short diceVal, short index);
void iterateTheGame();

// Print Functions
void printWinnerMessage(int index);
void printPieceStates();

// Initial Call From Main Function
void game();

#endif