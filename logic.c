    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    #include <stdbool.h>
    #include "types.h"

    #define BASE -1
    #define STARTPOINT 0
    #define HOME 56
    #define HOMESTRAIGHT 51


    short rollDice(char *name) {
        short r=(rand() % 6) + 1;
        printf("%s player rolled %d\n", name, r);
        return r;
    }

    short chooseFirstPlayer(struct player *players){
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
                chooseFirstPlayer(players);
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
        
    bool isSpecialLocation(short location, short *locArr, short len) {

        for (short i = 0; i < len; i++) {
            if (location == locArr[i]) {
                return true;
            }
        }
        return false;
    }

    void capturePiece(struct player *players, short index, short pieceId){
        short specialLocations[] = {0, 2, 13, 15, 26, 28, 39, 41};
        
        for(short i=0; i<4; i++){
            if(index!=i){
            for(short j=0; j<4; j++){
                    if(players[index].p[pieceId].location==players[i].p[j].location && 
                        players[i].p[j].distance < 52 &&
                        !isSpecialLocation(players[index].p[pieceId].location, specialLocations, 8)){

                        printf("%s piece %s lands on square L%d, captures %s piece %s, and returns it to the base.\n", 
                            players[index].playerName,
                            players[index].p[pieceId].pieceName,
                            players[index].p[pieceId].location,
                            players[i].playerName,
                            players[i].p[j].pieceName);
                        
                        players[i].p[j].distance=-1;
                        players[i].p[j].location=-1;
                        players[i].boardPiecesCount--;

                        printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n", 
                            players[i].playerName,
                            players[i].boardPiecesCount,
                            4-players[i].boardPiecesCount);
                        
                        break;
                    }
            } 
            }
        }
    }

    void movePlayer(struct player *players, short diceVal, short index, short *winners){
        bool isOnBoard=false;

        for(int k=0; k<4; k++){
            if(players[index].p[k].distance < HOMESTRAIGHT && players[index].p[k].distance > -1){
                isOnBoard=true;
                break;
            }
        }

        for(int i=0; i<4; i++){
            if(players[index].p[i].distance < HOMESTRAIGHT && players[index].p[i].distance != -1 && players[index].boardPiecesCount > 0){
                
                if(players[index].p[i].distance+diceVal < HOMESTRAIGHT){
                    printf("%s moves piece %s from location L%d to L%d by %d units in %s direction.\n", 
                        players[index].playerName, 
                        players[index].p[i].pieceName,
                        players[index].p[i].location,    
                        (players[index].p[i].location+diceVal)%52,
                        diceVal,
                        players[index].p[i].isClockwise? "clockwise" : "counter-clockwise"
                        );
                        
                    players[index].p[i].distance+=diceVal;
                    players[index].p[i].location=(players[index].p[i].location+diceVal)%52;

                    capturePiece(players, index, i);

                    return;

                }else if(players[index].p[i].distance+diceVal >= HOMESTRAIGHT ){

                    players[index].p[i].distance+=diceVal;

                    if(players[index].p[i].distance==HOME){
                        winPlayer(players, winners, index, i);
                        return;
                    }
                    
                        
                    printf("%s moves piece %s from L%d to Home Staight and %d cells far from home.\n", 
                        players[index].playerName, 
                        players[index].p[i].pieceName, 
                        players[index].p[i].location,
                        HOME - players[index].p[i].distance);

                    return;
                }
                
            }else if(players[index].p[i].distance < HOME && 
                players[index].p[i].distance >= HOMESTRAIGHT && 
                players[index].p[i].distance != -1 &&
                !isOnBoard){

                approchToHome(players, diceVal, index, winners, i);

                return;
            }
            
        }
        
    }

    void approchToHome(struct player *players, short diceVal, short index, short *winners, short pieceId){

        if(56-players[index].p[pieceId].distance == diceVal){
            players[index].p[pieceId].location+=diceVal;
            players[index].p[pieceId].distance+=diceVal;
            winPlayer(players, winners, index, pieceId);
        }           
    
    }

    void baseToStart(struct player *player){
        for(int i=0; i<4; i++){
            if(player->p[i].distance==-1){
                
                player->boardPiecesCount++;
                player->p[i].distance=0;
                player->p[i].location=2+(13*(player->index));   

                    // printf("Location: L%d", player->p[i].location);
                printf("%s player moves piece %s to the starting point.\n", player->playerName, player->p[i].pieceName);
                printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n", 
                    player->playerName,
                    player->boardPiecesCount + player->winPiecesCount,
                    4 - (player->boardPiecesCount + player->winPiecesCount));
                break;
            }
        }
    }

    void winPlayer(struct player *players, short *winners, short index, short i){
        if(players[index].p[i].distance>=HOME){
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

    void playerAction(struct player *players, short diceVal, short index, short *winners){
        if(diceVal==6 && 
        (players[index].boardPiecesCount + players[index].winPiecesCount) < 4 ){
            baseToStart(&players[index]);
            return;
        }
        if(players[index].boardPiecesCount > 0 ){
            movePlayer(players, diceVal, index, winners);
        }
    }

    void iterateTheGame(struct player *players, short *winners){
        short firstPlayer=chooseFirstPlayer(players);
        short j=firstPlayer;

        do{
            short diceVal;
            short isSixCount=3;
            do{
                if(players[j].winPiecesCount<4){    
                    diceVal = rollDice(players[j].playerName);
                    playerAction(players, diceVal, j, winners);
                    isSixCount--;
                    printf("\n");
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
                printPieceStates(players);
            }

        }while(1);

    }

    void printPieceStates(struct player *players){
        for(short k=0; k<4; k++){
            printf("%s player now has %d/4 on pieces on the board and %d/4 pieces on the base.\n", 
                players[k].playerName,
                players[k].boardPiecesCount + players[k].winPiecesCount,
                4 - (players[k].boardPiecesCount + players[k].winPiecesCount));
            printf("====================================\n");
            printf("Location of pieces %s\n", players[k].playerName);
            printf("====================================\n");
            for(int i=0; i<4; i++){
                printf("Piece %s - > ", players[k].p[i].pieceName);
                if(players[k].p[i].distance==BASE){
                    printf("Base");
                }else if(players[k].p[i].distance >= HOMESTRAIGHT && players[k].p[i].distance < HOME){
                    printf("%d cells to reach home.", HOME - players[k].p[i].distance);
                }else if(players[k].p[i].distance >= HOME){
                    printf("Home");
                }else{
                    printf("L%d", players[k].p[i].location);
                }
                printf("\n");
            }
        }
        printf("\n");
    }

    void printWinners(struct player *players, short *winners){
        for(short i=0; i<4; i++){
            printf("\n%d place >>> %s player <<<", i+1, players[winners[i]].playerName);
        }
    }

    void game(){
        srand(time(NULL));

        struct player players[] = {
        {0, "Yellow", 0, 0, {{0, -1, -1, 0, true, "Y1"}, {1, -1, -1, 0, true, "Y2"}, {2, -1, -1, 0, true, "Y3"}, {3, -1, -1, 0, true, "Y4"}}},
        {1, "Blue", 0, 0, {{0, -1, -1, 0, true, "B1"}, {1, -1, -1, 0, true, "B2"}, {2, -1, -1, 0, true, "B3"}, {3, -1, -1, 0, true, "B4"}}},
        {2, "Red", 0, 0, {{0, -1, -1, 0, true, "R1"}, {1, -1, -1, 0, true, "R2"}, {2, -1, -1, 0, true, "R3"}, {3, -1, -1, 0, true, "R4"}}},
        {3, "Green", 0, 0, {{0, -1, -1, 0, true, "G1"}, {1, -1, -1, 0, true, "G2"}, {2, -1, -1, 0, true, "G3"}, {3, -1, -1, 0, true, "G4"}}}};
        
        short winners[4]={-1, -1, -1, -1};   

        printf("The red player has four (04) pieces named R1, R2, R3 and R4.\n");
        printf("The green player has four (04) pieces named G1, G2, G3 and G4.\n");
        printf("The yellow player has four (04) pieces named Y1, Y2, Y3 and Y4.\n");
        printf("The blue player has four (04) pieces named B1, B2, B3 and B4.\n\n");

        iterateTheGame(players, winners);
        printWinners(players, winners);

    }