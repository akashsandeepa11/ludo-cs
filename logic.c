#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "player_behaviour.c"
#include "types.h"

#define BASE -1
#define STARTPOINT 0
#define HOMEPATH 5
#define HOME(playerId, pieceId) (players[playerId].p[pieceId].homeStraightDis + HOMEPATH)

#define BAWANA 9
#define KOTUWA 27
#define PITAKOTUWA 46

    struct player players[] = {
        // Yellow Player
        {
            "Yellow", 0, 0,
            {   // Blocks
                {-1, -1, -1, -1}, 
                {-1, -1, -1, -1}
            }, 
            {   // Pieces
                {-1, -1, 51, 0, true, "Y1"}, 
                {-1, -1, 51, 0, true, "Y2"}, 
                {-1, -1, 51, 0, true, "Y3"}, 
                {-1, -1, 51, 0, true, "Y4"}
            }
        },
        // Blue Player
        {
            "Blue", 0, 0,
            {   // Blocks
                {-1, -1, -1, -1}, 
                {-1, -1, -1, -1}
            }, 
            {   // Pieces
                {-1, -1, 51, 0, true, "B1"}, 
                {-1, -1, 51, 0, true, "B2"}, 
                {-1, -1, 51, 0, true, "B3"}, 
                {-1, -1, 51, 0, true, "B4"}
            }
        },
        // Red Player
        {
            "Red", 0, 0,
            {   // Blocks
                {-1, -1, -1, -1}, 
                {-1, -1, -1, -1}
            }, 
            {   // Pieces
                {-1, -1, 51, 0, true, "R1"}, 
                {-1, -1, 51, 0, true, "R2"}, 
                {-1, -1, 51, 0, true, "R3"}, 
                {-1, -1, 51, 0, true, "R4"}
            }
        },
        // Green Player
        {
            "Green", 0, 0,
            {   // Blocks
                {-1, -1, -1, -1}, 
                {-1, -1, -1, -1}
            }, 
            {   // Pieces
                {-1, -1, 51, 0, true, "G1"}, 
                {-1, -1, 51, 0, true, "G2"}, 
                {-1, -1, 51, 0, true, "G3"}, 
                {-1, -1, 51, 0, true, "G4"}
            }
        }
    };
    
    short winners[4]={-1, -1, -1, -1};   

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
        !isSpecialLocation(newLoc, specialLocations, 8)){
        
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

void winPlayer(short index, short i){
    if(players[index].p[i].distance >= HOME(index, i)){
        players[index].winPiecesCount++;
        players[index].boardPiecesCount--;
        printf(">> %s piece reached to the Home . <<\n", players[index].p[i].pieceName);

        if(players[index].winPiecesCount>=4){

            printf(">>>>> %s player's all pieces Reached to the Home!!!. <<<<<\n", players[index].playerName);
            for(short i=0; i<4; i++){
                if(winners[i]==-1){
                    winners[i]=index;
                    break;
                }
            }

        }

    }

    //Stop iterating after winned 3rd player
    if(winners[2] != -1){
        bool isFouthPlayer=false;
        for(short a=0; a<4; a++){
            for(short b=0; b<3; b++){
                if(winners[b]==a){
                    isFouthPlayer=true;
                    break;
                }
            }
            if(!isFouthPlayer){
                winners[3]=a;
                break;
            }
            isFouthPlayer=false;
        }
    }
}

void movePlayerDirectly(short playerId, short pieceId, short diceVal){

    short tmpLoc=players[playerId].p[pieceId].location;

    players[playerId].p[pieceId].distance+=diceVal;

    updateLocationAndDistance(playerId, pieceId, diceVal);
    
    if(players[playerId].p[pieceId].distance < players[playerId].p[pieceId].homeStraightDis){

        // players[playerId].p[pieceId].distance+=diceVal;
        // updateLocation(playerId, i, diceVal);

        printf("%s moves piece %s from location L%d to L%d by %d units in %s direction.\n", 
            players[playerId].playerName, 
            players[playerId].p[pieceId].pieceName,
            tmpLoc,    
            players[playerId].p[pieceId].location,
            diceVal,
            players[playerId].p[pieceId].isClockwise? "Clockwise" : "Counter-Clockwise"
            );

        checkForMysteryCell(playerId, pieceId);
        // createBlock(playerId, pieceId);

        return;

    }else if(players[playerId].p[pieceId].distance >= players[playerId].p[pieceId].homeStraightDis){

        // players[playerId].p[pieceId].distance+=diceVal;
        // updateLocation(playerId, i, diceVal);

        if(players[playerId].p[pieceId].distance == HOME(playerId, pieceId) &&
            players[playerId].p[pieceId].capCount > 0){
                                        
            winPlayer(playerId, pieceId);                            

            //players[playerId].p[pieceId].distance = players[playerId].p[pieceId].distance % HOMESTRAIGHT(players[playerId].p[pieceId].isClockwise);
            
            return;
        }
                               
        printf("%s moves piece %s from L%d to %s homepath %d by %d units %s direction.\n", 
            players[playerId].playerName, 
            players[playerId].p[pieceId].pieceName, 
            tmpLoc,
            players[playerId].playerName,
            players[playerId].p[pieceId].distance - players[playerId].p[pieceId].homeStraightDis,
            diceVal,
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

        //Check any piece on the base, then first priority will be the move piece on the base 
        // instead od moving home straight
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
                players[index].boardPiecesCount > 0){
                    
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
        // case BLUE:
        //     bluePlayer(diceVal);
            // return;
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
        
    movePlayer(diceVal, index);

}

void iterateTheGame(){
    short firstPlayer=chooseFirstPlayer(players);
    short j=firstPlayer;

    while(1){
        short diceVal;
        short isSixCount=3;
    
        roundCounter++;
        
        // mystery cell apear function call start
        if(roundCounter == 9 ){
            
            createMysteryCell();

        }else if((roundCounter - 5) % 16 == 0 && roundCounter > 20 ){
            
            createMysteryCell();

        }
        // mystery cell apear function call end

        do{
            if(players[j].winPiecesCount<4){    
                
                diceVal = rollDice(players[j].playerName);

                playerAction(diceVal, j);
                printf("\n");

                isSixCount--;
            }
        }while(diceVal==6 && isSixCount>0 && players[j].winPiecesCount < 4);

        if(j<3){
            j++;
        }else{
            j=0;
        }

        if (winners[3] != -1) {
            break;  
        }

        if(j==firstPlayer){
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

void printWinners(){
    for(short i=0; i<4; i++){
        printf("\n%d place >>> %s player <<<", i+1, players[winners[i]].playerName);
    }
}

void game(){
    srand(time(NULL));


    printf("The red player has four (04) pieces named R1, R2, R3 and R4.\n");
    printf("The green player has four (04) pieces named G1, G2, G3 and G4.\n");
    printf("The yellow player has four (04) pieces named Y1, Y2, Y3 and Y4.\n");
    printf("The blue player has four (04) pieces named B1, B2, B3 and B4.\n\n");

    iterateTheGame();
    printWinners();

}