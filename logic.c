#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "types.h"

#define YELLOW 0
#define BLUE 1
#define RED 2
#define GREEN 3

#define BASE -1
#define HOMEPATH 5
#define HOME(playerId, pieceId) (players[playerId].p[pieceId].homeStraightDis + HOMEPATH)
#define STARTPOINT(playerId) (2 + (13 * (playerId)))
#define APROACH(playerId) (13 * (playerId))

#define BAWANA 9
#define KOTUWA 27
#define PITAKOTUWA 46

struct player players[] = {
    // Yellow Player
    {
        "Yellow", 0, 0,
        {   // Pieces
            {-1, -1, 51, 0, true, "Y1", {-1, -1}}, 
            {-1, -1, 51, 0, true, "Y2", {-1, -1}}, 
            {-1, -1, 51, 0, true, "Y3", {-1, -1}}, 
            {-1, -1, 51, 0, true, "Y4", {-1, -1}}
        }
    },
    // Blue Player
    {
        "Blue", 0, 0,
        {   // Pieces
            {-1, -1, 51, 0, true, "B1", {-1, -1}}, 
            {-1, -1, 51, 0, true, "B2", {-1, -1}}, 
            {-1, -1, 51, 0, true, "B3", {-1, -1}}, 
            {-1, -1, 51, 0, true, "B4", {-1, -1}}
        }
    },
    // Red Player
    {
        "Red", 0, 0,
        {   // Pieces
            {-1, -1, 51, 0, true, "R1", {-1, -1}}, 
            {-1, -1, 51, 0, true, "R2", {-1, -1}}, 
            {-1, -1, 51, 0, true, "R3", {-1, -1}}, 
            {-1, -1, 51, 0, true, "R4", {-1, -1}}
        }
    },
    // Green Player
    {
        "Green", 0, 0,
        {   // Pieces
            {-1, -1, 51, 0, true, "G1", {-1, -1}}, 
            {-1, -1, 51, 0, true, "G2", {-1, -1}}, 
            {-1, -1, 51, 0, true, "G3", {-1, -1}}, 
            {-1, -1, 51, 0, true, "G4", {-1, -1}}
        }
    }
};
    
short winner=-1;   

short specialLocations[] = {0, 2, 13, 15, 26, 28, 39, 41};

short roundCounter = 0;

short mysteryCell = -1;

short rollDice(char *name) {
    
    short r=(rand() % 6) + 1;
    printf("%s player rolled %d\n", name, r);
    return r;

}

bool tossCoin(short playerId, short pieceId){

    short r=(rand() % 2);
    players[playerId].p[pieceId].isClockwise = r;
    
    if(!r){
        players[playerId].p[pieceId].homeStraightDis = 55;
    }
    
    printf("The %s tossed a %s\n", players[playerId].p[pieceId].pieceName, r? "Head" : "Tail");
    printf("%s will go %s\n", players[playerId].p[pieceId].pieceName, r? "Clockwise" : "Counter-Clockwise");
    return r;

}

short chooseFirstPlayer(){
    short arr[4];
    short max, maxIndex;
    short isMoreMax=0;

    for(short i=0; i<4; i++){
        arr[i]=rollDice(players[i].playerName);
    }

    printf("\n");

    max=arr[0];
    maxIndex=0;

    for(short i=1; i<4; i++){
        if(max<arr[i]){
            max=arr[i];
            maxIndex=i;
        }
    }

    for(short i=0; i<4; i++){
        if(arr[i]==max && maxIndex!=i){
            isMoreMax=0;
            printf("More than one player got the same highest dice value. Let's roll the dice again.\n\n");
            chooseFirstPlayer();
            isMoreMax=1;
            break;
        }
        isMoreMax=0;
    }

    if(!isMoreMax){
        printf("%s player has the highest number and will begin the game.\n\n", players[maxIndex].playerName);

        short j=maxIndex;

        printf("The order of a single round is");
        for(short i=0; i<4; i++){

            if(i<3){
                printf(" %s,", players[j].playerName);
            }else{
                printf(" and %s.\n\n", players[j].playerName);
                return maxIndex;
                break;
            }

            if(j<3){
                j++;
            }else{
                j=0;
            }  
        }
    }
}
        
bool isSpecialLocation(short location, short *locArr, short len){

    for (short i = 0; i < len; i++) {
        if (location == locArr[i]) {
            return true;
        }
    }
    return false;
}

void capturePiece(short playerId, short pieceId, short opPlayerId, short opPieceId){
    
    if(!isSpecialLocation(players[playerId].p[pieceId].location, specialLocations, 8) &&
        players[playerId].p[pieceId].location == players[opPlayerId].p[opPieceId].location &&
        players[opPlayerId].p[pieceId].distance < players[opPlayerId].p[pieceId].homeStraightDis){   

        printf("%s piece %s lands on square L%d, captures %s piece %s, and returns it to the base.\n", 
            players[playerId].playerName,
            players[playerId].p[pieceId].pieceName,
            players[playerId].p[pieceId].location,
            players[opPlayerId].playerName,
            players[opPlayerId].p[opPieceId].pieceName);
                        
        players[playerId].p[pieceId].capCount++;
                    
        players[opPlayerId].p[opPieceId].location=-1;
        players[opPlayerId].p[opPieceId].distance=-1;
        players[opPlayerId].p[opPieceId].capCount=0;
        players[opPlayerId].p[opPieceId].homeStraightDis=51;
        players[opPlayerId].boardPiecesCount--;
        players[opPlayerId].p[opPieceId].mysteryData.counter=-1;
        players[opPlayerId].p[opPieceId].mysteryData.isEnergised=-1;

        printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n\n", 
            players[opPlayerId].playerName,
            players[opPlayerId].boardPiecesCount,
            4-players[opPlayerId].boardPiecesCount);

        playerAction(rollDice(players[playerId].playerName), playerId);
    }
}

bool captureIfAvailable(short playerId, short pieceId, short diceVal, bool ischeck){

    short newLoc = players[playerId].p[pieceId].location;
    updateLocation(&newLoc, playerId, pieceId, diceVal);

    if(players[playerId].p[pieceId].location != -1 &&
        players[playerId].p[pieceId].distance + diceVal < players[playerId].p[pieceId].homeStraightDis &&
        !isSpecialLocation(newLoc, specialLocations, 8) &&
        !(players[playerId].p[pieceId].mysteryData.counter != -1 && players[playerId].p[pieceId].mysteryData.isEnergised == -1)){
        
        for(short opPlayerId=0; opPlayerId<4; opPlayerId++){
            
            if(playerId != opPlayerId){
                for(short opPieceId=0; opPieceId<4; opPieceId++){

                    if(newLoc == players[opPlayerId].p[opPieceId].location){
                        
                        if(!ischeck){
                            movePlayerDirectly(playerId, pieceId, diceVal);
                            capturePiece(playerId, pieceId, opPlayerId, opPieceId);
                        }
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void capturePieceByPlayerId(short index, short pieceId){
        
    for(short i=0; i<4; i++){
        if(index!=i){
            for(short j=0; j<4; j++){

                if(players[index].p[pieceId].location == players[i].p[j].location && 
                    players[i].p[j].distance < players[i].p[j].homeStraightDis &&
                    !isSpecialLocation(players[index].p[pieceId].location, specialLocations, 8)){

                    capturePiece(index, pieceId, i, j);
                                    
                }

                break;
            }
        } 
    }
}   
    
void updateLocation(short *locVariable, short playerID, short pieceId, short diceVal){

    if (players[playerID].p[pieceId].isClockwise) {
        // Move clockwise
        *locVariable = (*locVariable + diceVal) % 52;
    } else {
        // Move counterclockwise
        *locVariable -= diceVal;

        if (*locVariable < 0) {
            *locVariable = 52 + *locVariable;
        }
    }
}

void updateLocationAndDistance(short index, short i, short diceVal) {

    updateLocation(&players[index].p[i].location, index, i, diceVal);

    if(players[index].p[i].capCount ==  0 &&
        players[index].p[i].distance >= players[index].p[i].homeStraightDis){

        players[index].p[i].homeStraightDis = players[index].p[i].homeStraightDis + 52;
    }          
}

void approchToHome(short diceVal, short index, short pieceId){

    if(HOME(index, pieceId) - players[index].p[pieceId].distance == diceVal){
        players[index].p[pieceId].location+=diceVal;
        players[index].p[pieceId].distance+=diceVal;

        winPlayer(index, pieceId);

    }              
}

void yellowPlayer(short diceVal){
    
    if(diceVal == 6 && 
        (players[YELLOW].boardPiecesCount + players[YELLOW].winPiecesCount) < 4 &&
        players[YELLOW].winPiecesCount < 4){

        baseToStart(YELLOW);
        return;
    } 

    if((players[YELLOW].boardPiecesCount + players[YELLOW].winPiecesCount) >= 4
        || (players[YELLOW].boardPiecesCount + players[YELLOW].winPiecesCount) < 4 && diceVal != 6){
        
        // check are there any piece that havent capture any piece
        for(short pieceId=0; pieceId<4; pieceId++){

            if(players[YELLOW].p[pieceId].capCount == 0 && 
                players[YELLOW].p[pieceId].location != -1 && 
                players[YELLOW].p[pieceId].distance < players[YELLOW].p[pieceId].homeStraightDis){

                if(captureIfAvailable(YELLOW, pieceId, diceVal, true)){
                    captureIfAvailable(YELLOW, pieceId, diceVal, false);
                    return;
                }                
            }
        }            
    }
    
    movePlayer(diceVal, YELLOW);
    return;
}

void bluePlayer(short diceVal){

    // check mystery cell
    for(short pieceId = 0; pieceId < 4; pieceId++){
        
        if(players[BLUE].p[pieceId].location != -1 &&
            players[BLUE].p[pieceId].distance + diceVal < players[BLUE].p[pieceId].homeStraightDis &&
            !(players[BLUE].p[pieceId].mysteryData.counter != -1 && players[BLUE].p[pieceId].mysteryData.isEnergised == -1)){

            short newLocation = players[BLUE].p[pieceId].location; 
            
            updateLocation(&newLocation, BLUE, pieceId, diceVal);

            if(newLocation == mysteryCell){
                
                if(!captureIfAvailable(BLUE, pieceId, diceVal, false)){

                    movePlayerDirectly(BLUE, pieceId, diceVal);
                    capturePieceByPlayerId(BLUE, pieceId);
                }
                return;
            }
        }
    }

    movePlayer(diceVal, BLUE);
    
    return;
}

void greenPlayer(short diceVal){
    
    if(diceVal == 6 && 
        (players[GREEN].boardPiecesCount + players[GREEN].winPiecesCount) < 4 &&
        players[GREEN].winPiecesCount < 4){

        baseToStart(GREEN);
        return;
    } 

    // check are there any piece that havent capture any piece
    for(short pieceId=0; pieceId<4; pieceId++){

        if(players[GREEN].p[pieceId].capCount == 0 && 
            players[GREEN].p[pieceId].location != -1 && 
            players[GREEN].p[pieceId].distance < players[GREEN].p[pieceId].homeStraightDis){

            if(captureIfAvailable(GREEN, pieceId, diceVal, true)){
                captureIfAvailable(GREEN, pieceId, diceVal, false);
                return;        
            }
        }
    }

    // check and move if there are any piece to create block
    for(short pieceId = 0; pieceId < 4; pieceId++){
        
        if(players[GREEN].p[pieceId].location != -1 &&
            players[GREEN].p[pieceId].distance + diceVal < players[GREEN].p[pieceId].homeStraightDis &&
            !(players[GREEN].p[pieceId].mysteryData.counter != -1 && players[GREEN].p[pieceId].mysteryData.isEnergised == -1)){

            for(short j=0; j<4; j++){

                if(pieceId != j){

                    short newLocation = players[GREEN].p[pieceId].location; 
                    
                    updateLocation(&newLocation, GREEN, pieceId, diceVal);

                    if(newLocation == players[GREEN].p[j].location){
                        
                        movePlayerDirectly(GREEN, pieceId, diceVal);
                        capturePieceByPlayerId(GREEN, pieceId);
                        return;
                    }
                }
            }
        }
    }

    movePlayer(diceVal, GREEN);

    return;
}

void redPlayer(short diceVal){

    short capturingPieces[4] = {-1, -1, -1, -1};
    short beingCapture[4][2] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}};
    short lastVal=-1; 

    // Determine are there any pieces to capture and store their indexes to capturePieces array 
    for(short pieceId=0; pieceId < 4; pieceId++){
        
        if(players[RED].p[pieceId].location != -1 &&
            players[RED].p[pieceId].distance + diceVal < players[RED].p[pieceId].homeStraightDis &&
            !isSpecialLocation(players[RED].p[pieceId].location + diceVal, specialLocations, 8)){
            
            short newLocation = players[RED].p[pieceId].location; 
            
            updateLocation(&newLocation, RED, pieceId, diceVal);

            for(short opPlayer=0; opPlayer<4; opPlayer++){
                
                if(opPlayer != RED){
                    
                    for(short opPiece=0; opPiece<4; opPiece++){

                        if(players[opPlayer].p[opPiece].distance < players[opPlayer].p[opPiece].homeStraightDis){

                            if(newLocation == players[opPlayer].p[opPiece].location){
                                
                                capturingPieces[++lastVal]=pieceId;
                                beingCapture[lastVal][0]=opPlayer;
                                beingCapture[lastVal][1]=opPiece;

                            } 
                        }
                    }
                }
            }
        }
    }

    short indexOfPieceCloseToHome = 0;

    if(lastVal != -1){
         
        // Identify what is the closest piece to Its's home
        for(short i=1; i < lastVal+1; i++){

            if((players[beingCapture[i][0]].p[beingCapture[i][1]].homeStraightDis - players[beingCapture[i][0]].p[beingCapture[i][1]].distance) < 
                (players[beingCapture[indexOfPieceCloseToHome][0]].p[beingCapture[indexOfPieceCloseToHome][1]].homeStraightDis - players[beingCapture[indexOfPieceCloseToHome][0]].p[beingCapture[indexOfPieceCloseToHome][1]].distance)){

                indexOfPieceCloseToHome = i;

            }
        }
    
    if(!(players[RED].p[capturingPieces[indexOfPieceCloseToHome]].mysteryData.counter != -1 && players[RED].p[capturingPieces[indexOfPieceCloseToHome]].mysteryData.isEnergised == -1)){
        
        movePlayerDirectly(RED, capturingPieces[indexOfPieceCloseToHome], diceVal);

        capturePiece(RED, capturingPieces[indexOfPieceCloseToHome], 
            beingCapture[indexOfPieceCloseToHome][0],
            beingCapture[indexOfPieceCloseToHome][1]);

        return;
    } 
    }else if(players[RED].boardPiecesCount > 0){

        movePlayer(diceVal, RED);
        return;
    }    

    if(diceVal == 6 && 
        (players[RED].boardPiecesCount + players[RED].winPiecesCount) < 4 &&
        players[RED].winPiecesCount < 4){

        baseToStart(RED);
        return;

    }       
}

void baseToStart(short playerIndex){

    for(short i=0; i<4; i++){
        
        if(players[playerIndex].p[i].location==-1){

            players[playerIndex].boardPiecesCount++;
            players[playerIndex].p[i].distance=0;
            players[playerIndex].p[i].location = STARTPOINT(playerIndex);   

            // printf("Location: L%d", players[playerIndex].p[i].location);
            printf("%s player moves piece %s to the starting point L%d.\n", players[playerIndex].playerName, 
                players[playerIndex].p[i].pieceName, 
                players[playerIndex].p[i].location);

            tossCoin(playerIndex, i);

            printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n", 
                players[playerIndex].playerName,
                players[playerIndex].boardPiecesCount + players[playerIndex].winPiecesCount,
                4 - (players[playerIndex].boardPiecesCount + players[playerIndex].winPiecesCount));

            break;
        }
    }
}

void checkForMysteryCell(short playerId, short pieceId){
    
    if(mysteryCell == players[playerId].p[pieceId].location){

        short rnd=(rand() % 6);

        switch (rnd){
        case 0:
            toApproach(playerId, pieceId);
            break;
        case 1:
            toBase(playerId, pieceId);
            break;
        case 2:
            toBawana(playerId, pieceId);
            break;
        case 3:
            toKotuwa(playerId, pieceId);
            break;
        case 4:
            toPitaKotuwa(playerId, pieceId);
            break;
        case 5:
            toX(playerId, pieceId);
            break;
        }
    };
}

void createMysteryCell(){
        
    short tmpCellNum;
    bool isOnPiece=false;

    do{
        isOnPiece = false;

        tmpCellNum = rand() % 52;

        for(short i = 0; i<3; i++){
            for(short j = 0; j < 3; j++){
                if(players[i].p[j].location == tmpCellNum){
                    isOnPiece=true;
                    break;
                    break;
                }
            }
        }

    }while( tmpCellNum == mysteryCell && isOnPiece);

    mysteryCell = tmpCellNum;

    printf("\033[35mA mystery cell has spawned in Location L%d and will be at this location for next four rounds.\033[0m\n\n", mysteryCell);
}

void toBawana(short playerId, short pieceId){
    printf("%s piece %s teleported to Bhawana\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);
    
    if(rand() % 2){
        
        printf("%s piece %s feels energized, and movement speed doubles.\n\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);
        players[playerId].p[pieceId].mysteryData.counter=3;
        players[playerId].p[pieceId].mysteryData.isEnergised=1;
    }else{
        
        printf("%s piece %s feels sick, and movement speed halves.\n\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);
        players[playerId].p[pieceId].mysteryData.counter=3;
        players[playerId].p[pieceId].mysteryData.isEnergised=0;
    }
}

void toKotuwa(short playerId, short pieceId){
    
    printf("%s piece %s teleported to Kotuwa\n\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);

    players[playerId].p[pieceId].mysteryData.counter=3;
    players[playerId].p[pieceId].mysteryData.isEnergised=-1;
}

void toPitaKotuwa(short playerId, short pieceId){
    printf("%s piece %s teleported to Pita-Kotuwa\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);

    if(players[playerId].p[pieceId].isClockwise){
        players[playerId].p[pieceId].isClockwise=!players[playerId].p[pieceId].isClockwise;
        printf("The %s piece %s, which was moving clockwise, has changed to moving counter-clockwise\n\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);
    }else{
        printf("The %s piece %s is moving in a counter-clockwise direction. Teleporting to Kotuwa from Pita-Kotuwa\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);
        toKotuwa(playerId, pieceId);
    }
}

void toBase(short playerId, short pieceId){
    printf("%s piece %s teleported to Base\n\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);

    players[playerId].p[pieceId].location=-1;
    players[playerId].p[pieceId].distance=-1;
    players[playerId].p[pieceId].capCount=0;
    players[playerId].p[pieceId].homeStraightDis=51;
    players[playerId].boardPiecesCount--;
    players[playerId].p[pieceId].mysteryData.counter=-1;
    players[playerId].p[pieceId].mysteryData.isEnergised=-1;
}

void toX(short playerId, short pieceId){
    printf("%s piece %s teleported to X\n\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);

    players[playerId].p[pieceId].location=STARTPOINT(playerId);
}

void toApproach(short playerId, short pieceId){
    printf("%s piece %s teleported to Approach\n\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);

    players[playerId].p[pieceId].location=APROACH(playerId);
}

void printWinnerMessage(int index) {

    switch(index) {
        case 0:  
            printf("\033[1;33m%s player wins!!!\033[0m\n", players[index].playerName);
            break;
        case 1:  
            printf("\033[1;34m%s player wins!!!\033[0m\n", players[index].playerName);
            break;
        case 2:  
            printf("\033[1;31m%s player wins!!!\033[0m\n", players[index].playerName);
            break;
        case 3:  
            printf("\033[1;32m%s player wins!!!\033[0m\n", players[index].playerName);
            break;
    }
}

void winPlayer(short index, short i){
    if(players[index].p[i].distance >= HOME(index, i)){
        players[index].winPiecesCount++;
        players[index].boardPiecesCount--;
        printf(">> %s piece reached to the Home . <<\n", players[index].p[i].pieceName);

        if(players[index].winPiecesCount>=4){

            printWinnerMessage(index);
            winner=index;

        }
    }
}

void movePlayerDirectly(short playerId, short pieceId, short diceVal){

    if(players[playerId].p[pieceId].mysteryData.counter != -1 && players[playerId].p[pieceId].mysteryData.isEnergised == -1){
        return;
    }

    short tmpDiceVal=diceVal;

    if(players[playerId].p[pieceId].mysteryData.counter != -1 && players[playerId].p[pieceId].mysteryData.isEnergised == 1){
        tmpDiceVal=diceVal*2;
    }else if(players[playerId].p[pieceId].mysteryData.counter != -1 && players[playerId].p[pieceId].mysteryData.isEnergised == 0){
        tmpDiceVal=diceVal/2;
    }

    short tmpLoc=players[playerId].p[pieceId].location;

    players[playerId].p[pieceId].distance+=tmpDiceVal;

    updateLocationAndDistance(playerId, pieceId, tmpDiceVal);
    
    if(players[playerId].p[pieceId].distance < players[playerId].p[pieceId].homeStraightDis){

            printf("%s moves piece %s from location L%d to L%d by %d units in %s direction.\n", 
                players[playerId].playerName, 
                players[playerId].p[pieceId].pieceName,
                tmpLoc,    
                players[playerId].p[pieceId].location,
                tmpDiceVal,
                players[playerId].p[pieceId].isClockwise? "Clockwise" : "Counter-Clockwise"
                );

        checkForMysteryCell(playerId, pieceId);

        return;

    }else if(players[playerId].p[pieceId].distance >= players[playerId].p[pieceId].homeStraightDis){

        if(players[playerId].p[pieceId].distance == HOME(playerId, pieceId) &&
            players[playerId].p[pieceId].capCount > 0){
                                        
            winPlayer(playerId, pieceId);                            

            return;
        }
                            
        printf("%s moves piece %s from L%d to %s homepath %d by %d units %s direction.\n", 
            players[playerId].playerName, 
            players[playerId].p[pieceId].pieceName, 
            tmpLoc,
            players[playerId].playerName,
            players[playerId].p[pieceId].distance - players[playerId].p[pieceId].homeStraightDis,
            tmpDiceVal,
            players[playerId].p[pieceId].isClockwise? "Clockwise" : "Counter-Clockwise"
            );

        return;
    }    
}

void movePlayer(short diceVal, short index){

    if(diceVal == 6 && 
        (players[index].boardPiecesCount + players[index].winPiecesCount) < 4 &&
        players[index].winPiecesCount < 4){

        baseToStart(index);
        return;

    }

    bool isOnBoard=false;

    for(short k=0; k<4; k++){
        if(players[index].p[k].distance < players[index].p[k].homeStraightDis && 
            players[index].p[k].distance > -1){
            
            isOnBoard=true;
            break;
        }
    }

    for(short i=0; i<4; i++){
        
        if(players[index].p[i].distance < players[index].p[i].homeStraightDis && 
            players[index].p[i].distance != -1 && 
            players[index].p[i].location != -1 && 
            players[index].boardPiecesCount > 0 &&
            !(players[index].p[i].mysteryData.counter != -1 && players[index].p[i].mysteryData.isEnergised == -1)){
                
            movePlayerDirectly(index, i, diceVal);
            capturePieceByPlayerId(index, i);
            return;
            
        }else if(players[index].p[i].distance < HOME(index, i) && 
            players[index].p[i].distance >= players[index].p[i].homeStraightDis && 
            players[index].p[i].distance != -1 &&
            players[index].p[i].location != -1 &&
            !isOnBoard){

            if(players[index].p[i].capCount > 0){
                approchToHome(diceVal, index, i);
                return;
            }    

            return;
        }        
    }
}

void playerAction(short diceVal, short index){
        
    switch(index){
        case BLUE:
            bluePlayer(diceVal);
            return;
        case RED:
            redPlayer(diceVal);
            return;
        case GREEN:
            greenPlayer(diceVal);
            return;
        case YELLOW:
            yellowPlayer(diceVal);
            return;
    }
}

void iterateTheGame(){
    short firstPlayer=chooseFirstPlayer(players);
    short playerId=firstPlayer;

    while(1){
        short diceVal;
        short isSixCount=3;
    
        // For Kotuwa function
        short isThreeCount=3;
        
        roundCounter++;

        // mystery cell apear function call start
        if(roundCounter == 9 ){
            
            createMysteryCell();

        }else if((roundCounter - 5) % 16 == 0 && roundCounter > 20 ){
            
            createMysteryCell();

        }

        // mystery cell apear function call end
        do{

            if(players[playerId].winPiecesCount<4){    
                
                diceVal = rollDice(players[playerId].playerName);

                playerAction(diceVal, playerId);
                printf("\n");

                isSixCount--;

                if(diceVal==3){
                    isThreeCount--;
                } 
            }

        }while(diceVal==6 && isSixCount>0 && players[playerId].winPiecesCount < 4);

        if(playerId<3){
            playerId++;
        }else{
            playerId=0;
        }

        if (winner != -1) {
            break;  
        }

        if((roundCounter) % 4 == 0){

            for(short plId=0; plId<4; plId++){

                for(short pcId=0; pcId<4; pcId++){

                    if(players[plId].p[pcId].mysteryData.counter != -1){

                        players[plId].p[pcId].mysteryData.counter--;

                        // If player had been teleported to the bawana after 4 rounds, stop thw power
                        if(players[plId].p[pcId].mysteryData.counter == -1){
                            
                            players[plId].p[pcId].mysteryData.isEnergised == -1;
                        }

                        // check if player had teleported to the kotuwa
                        if(players[plId].p[pcId].mysteryData.counter == 0 && players[plId].p[pcId].mysteryData.isEnergised == -1 && isThreeCount==0){
                            
                            printf("The %s piece %s is movement-restricted and has rolled three consecutively.\n");
                            
                            toBase(plId, pcId);
                        }    
                    }
                }    
            }
        }

        if(playerId==firstPlayer){
            printPieceStates();
        }
    }
}

void printPieceStates() {
    
    for (short k = 0; k < 4; k++) {
        // Set color based on the player index (k)
        switch (k) {
            case 0: printf("\033[33m"); break; // Yellow
            case 1: printf("\033[34m"); break; // Blue
            case 2: printf("\033[31m"); break; // Red
            case 3: printf("\033[32m"); break; // Green
            default: printf("\033[0m"); break;  // Reset to default
        }

        printf("====================================\n");
        printf("Location of pieces %s\n", players[k].playerName);
        printf("====================================\n");

        for (short i = 0; i < 4; i++) {
            printf("Piece %s -> ", players[k].p[i].pieceName);

            if (players[k].p[i].distance == BASE) {
                printf("Base");
            } else if (players[k].p[i].distance >= players[k].p[i].homeStraightDis && 
                       players[k].p[i].distance < HOME(k, i) && 
                       players[k].p[i].capCount > 0) {
                printf("%s homepath %d. cap:%d dis:%d home:%d c:%d", 
                       players[k].playerName,
                       players[k].p[i].distance - players[k].p[i].homeStraightDis, 
                       players[k].p[i].capCount, 
                       players[k].p[i].distance,
                       players[k].p[i].homeStraightDis,
                       players[k].p[i].isClockwise);
            } else if (players[k].p[i].distance >= HOME(k, i) && players[k].p[i].capCount > 0) {
                printf("Home cap:%d dis:%d home:%d c:%d", 
                       players[k].p[i].capCount,
                       players[k].p[i].distance,
                       players[k].p[i].homeStraightDis,
                       players[k].p[i].isClockwise);
            } else {
                printf("L%d - cap:%d dis:%d home:%d c:%d", 
                       players[k].p[i].location, 
                       players[k].p[i].capCount, 
                       players[k].p[i].distance,
                       players[k].p[i].homeStraightDis,
                       players[k].p[i].isClockwise);
            }

            printf("\n");
        }

        printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n", 
               players[k].playerName,
               players[k].boardPiecesCount + players[k].winPiecesCount,
               4 - (players[k].boardPiecesCount + players[k].winPiecesCount));

        // Reset color to default after each player's output
        printf("\033[0m");
    }

    if(mysteryCell != -1 && ((roundCounter % 16) / 4) != 1 ){        
        
        printf("The mystery cell is at L%d and will be at that location for the next %d rounds\n", mysteryCell, 
            (((roundCounter % 16) / 4) == 0) ? 1 : (((roundCounter % 16) / 4) == 1) ? 0 : (((roundCounter % 16) / 4) == 2) ? 3 : 2);
            
    }

    printf("\n");
}

void game(){

    srand(time(NULL));

    printf("The red player has four (04) pieces named R1, R2, R3 and R4.\n");
    printf("The green player has four (04) pieces named G1, G2, G3 and G4.\n");
    printf("The yellow player has four (04) pieces named Y1, Y2, Y3 and Y4.\n");
    printf("The blue player has four (04) pieces named B1, B2, B3 and B4.\n\n");

    iterateTheGame();

}