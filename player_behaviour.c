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

#define BAWANA 9
#define KOTUWA 27
#define PITAKOTUWA 46

extern struct player players[];  
extern short specialLocations[];

extern short roundCounter;
extern short mysteryCell;

void yellowPlayer(short diceVal){
    


    return;
}

void bluePlayer(short diceVal){

    return;
}

void redPlayer(short diceVal){

    short capturingPieces[4] = {-1, -1, -1, -1};
    short beingCapture[4][2] = {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}};
    short lastVal=-1; 

    if(diceVal == 6 && 
        (players[RED].boardPiecesCount + players[RED].winPiecesCount) < 2){

        baseToStart(RED);
        return;
    }

    // Determine are there any pieces to capture and store their indexes to capturePieces array 
    for(short pieceId=0; pieceId < 4; pieceId++){

        
        if(players[RED].p[pieceId].location != -1 &&
            players[RED].p[pieceId].distance + diceVal < players[RED].p[pieceId].homeStraightDis &&
            !isSpecialLocation(players[RED].p[pieceId].location+diceVal, specialLocations, 8)){
            
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
                (players[beingCapture[i-1][0]].p[beingCapture[i-1][1]].homeStraightDis - players[beingCapture[i-1][0]].p[beingCapture[i-1][1]].distance)){

                indexOfPieceCloseToHome = i;

            }
        }


        movePlayer1(RED, capturingPieces[indexOfPieceCloseToHome], diceVal);

        capturePiece(RED, capturingPieces[indexOfPieceCloseToHome], 
            beingCapture[indexOfPieceCloseToHome][0],
            beingCapture[indexOfPieceCloseToHome][1]);

        return;

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

void greenPlayer(short diceVal){

    return;
}

void baseToStart(short playerIndex){

    for(short i=0; i<4; i++){
        
        if(players[playerIndex].p[i].location==-1){

            players[playerIndex].boardPiecesCount++;
            players[playerIndex].p[i].distance=0;
            players[playerIndex].p[i].location = 2 + (13*(playerIndex));   

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

void checkForMysteryCell(){
    
}

void createMysteryCell(){
        
    short tmpCellNum;
    
    do{
        
        tmpCellNum = rand() % 52;

    }while( tmpCellNum == mysteryCell );

    mysteryCell = tmpCellNum;

    printf("\033[35m");  
    printf("+-------------------------------------------------------------+\n");
    printf("| A Mystery Cell has appeared at cell number %2d on the board! |\n", mysteryCell);
    printf("+-------------------------------------------------------------+\033[0m\n");
}



