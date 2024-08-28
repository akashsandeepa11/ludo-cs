#include <stdio.h>
#include "types.h"
#define YELLOW 0
#define BLUE 1
#define RED 2
#define GREEN 3


    #define BASE -1
    #define STARTPOINT 0
    #define HOMEPATH 5
    #define HOME(playerId, pieceId) (players[playerId].p[pieceId].homeStraightDis + HOMEPATH)



extern struct player players[];  
extern short specialLocations[];

void yellowPlayer(short diceVal){
    


    return;
}

void bluePlayer(short diceVal){

    return;
}

void redPlayer(short diceVal){

    short capturePieces[4] = {-1, -1, -1, -1};
    short lastVal=0; 

    // Determine are there any pieces to capture and store their indexes to capturePieces arrey 
    for(short pieceId=0; pieceId<4; pieceId++){
        
        if(players[RED].p[pieceId].location != -1 &&
            players[RED].p[pieceId].distance < players[RED].p[pieceId].homeStraightDis &&
            !isSpecialLocation(players[RED].p[pieceId].location+diceVal, specialLocations, 8)){
            
            short newLocation; 
            
            updatedLocation(&newLocation ,RED, pieceId, diceVal);

            for(short opPlayer=0; opPlayer<4; opPlayer++){
                
                if(opPlayer != RED){
                    
                    for(short opPiece=0; opPiece<4; opPiece++){

                        if(players[opPlayer].p[opPiece].distance < players[opPlayer].p[opPiece].homeStraightDis){

                            if(newLocation == players[opPlayer].p[opPiece].location){
                                
                                capturePieces[lastVal++]=pieceId;
                            
                            } 

                        }

                    }
                }
            }
        }
    }

    // Identify what is the closest piece to Its's home
    short pieceCloseHome = capturePieces[0];

    if(lastVal > 1){
         
        for(short i=1; i<lastVal; i++){

            // if((players[])){

            //     // pieceCloseHome = capturePieces[i];

            // }else{

            // }

                printf("#######################capture players\n %s ##########################", players[RED].p[pieceCloseHome].pieceName);
        }
    }

    
    
    return;
}

void greenPlayer(short diceVal){

    return;
}

void baseToStart(short playerIndex){

    for(short i=0; i<4; i++){
        
        if(players[playerIndex].p[i].location==-1){
                
            players[playerIndex].boardPiecesCount++;
            players[playerIndex].p[i].distance=0;
            players[playerIndex].p[i].location = 2 + (13*(players[playerIndex].index));   

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

short updatedLocation(short *locVariable, short playerID, short pieceId, short diceVal){

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

    return *locVariable;
}


