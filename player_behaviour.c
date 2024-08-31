#include <stdio.h>
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

extern struct player players[];  
extern short specialLocations[];

extern short roundCounter;
extern short mysteryCell;

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
            players[BLUE].p[pieceId].distance + diceVal < players[BLUE].p[pieceId].homeStraightDis){

                short newLocation = players[BLUE].p[pieceId].location; 
                
                updateLocation(&newLocation, BLUE, pieceId, diceVal);

                if(newLocation == mysteryCell){
                    
                    movePlayerDirectly(BLUE, pieceId, diceVal);
                    capturePieceByPlayerId(BLUE, pieceId);
                    return;
                }
        }
    }

    movePlayer(diceVal, BLUE);
    
    return;
}

void greenPlayer(short diceVal){
    
    if(diceVal == 6 && 
        (players[GREEN].boardPiecesCount + players[GREEN].winPiecesCount) <= 4 &&
        players[GREEN].winPiecesCount <= 4){

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
            players[GREEN].p[pieceId].distance + diceVal < players[GREEN].p[pieceId].homeStraightDis){

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

        movePlayerDirectly(RED, capturingPieces[indexOfPieceCloseToHome], diceVal);

        capturePiece(RED, capturingPieces[indexOfPieceCloseToHome], 
            beingCapture[indexOfPieceCloseToHome][0],
            beingCapture[indexOfPieceCloseToHome][1]);

        return;

    }else if(players[RED].boardPiecesCount > 0){

        movePlayer(diceVal, RED);
        // checkForMysteryCell();
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

        printf("####################################################################################################################################################");

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
    printf("%s piece %s teleported to Bhawana\n\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);
}

void toKotuwa(short playerId, short pieceId){
    printf("%s piece %s teleported to Kotuwa\n\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);
}

void toPitaKotuwa(short playerId, short pieceId){
    printf("%s piece %s teleported to Pita-Kotuwa\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);

    if(players[playerId].p[pieceId].isClockwise){
        players[playerId].p[pieceId].isClockwise=!players[playerId].p[pieceId].isClockwise;
        printf("The %s piece %s, which was moving clockwise, has changed to moving counter-clockwise\n");
    }else{
        toKotuwa(playerId, pieceId);
        printf("The %s piece %s is moving in a counter-clockwise direction. Teleporting to Kotuwa from Pita-Kotuwa\n");
    }
}

void toBase(short playerId, short pieceId){
    printf("%s piece %s teleported to Base\n\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);

    players[playerId].p[pieceId].location=-1;
    players[playerId].p[pieceId].distance=-1;
    players[playerId].p[pieceId].capCount=0;
    players[playerId].p[pieceId].homeStraightDis=51;
    players[playerId].boardPiecesCount--;
}

void toX(short playerId, short pieceId){
    printf("%s piece %s teleported to X\n\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);

    players[playerId].p[pieceId].location=STARTPOINT(playerId);
}

void toApproach(short playerId, short pieceId){
    printf("%s piece %s teleported to Approach\n\n", players[playerId].playerName, players[playerId].p[pieceId].pieceName);

    players[playerId].p[pieceId].location=APROACH(playerId);
}

// void createBlock(short playerId, short pieceId){
    
//     short counter=1;

//     for(short i=0; i<4; i++){
//         if(i != pieceId){

//             if(players[playerId].p[pieceId].location == players[playerId].p[i].location){
                
//                 if(players[playerId].blocks[0][0] == -1){
                    
//                     players[playerId].blocks[0][0] = pieceId;
//                     players[playerId].blocks[0][1] = i;
        
//                     break;
//                 }else{

//                     while(players[playerId].blocks[0][counter] != -1){

//                         if(players[playerId].blocks[0][counter+1] == -1){

//                             if(players[playerId].blocks[0][0] != i && players[playerId].blocks[0][1] != i && 
//                                 players[playerId].blocks[0][2] != i && players[playerId].blocks[0][3] != i){
                            
//                                 players[playerId].blocks[0][counter+1] = i;
//                                 break;
//                             }
//                         }
//                         counter++;
                    
//                     }
//                 }


//                 printf("####################################################################################################################################################################################\n\n"); 
//                 printf("%s player created block with ", players[playerId].playerName);

//                 for(short i=0; i<3; i++){
//                     printf(" %s,", players[playerId].p[players[playerId].blocks[0][i]].pieceName);
//                 }
//                 printf(".\n");
//             }
//         }
//     }
    
// }