#include <iostream>
#include <windows.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <limits>

using namespace std;

ifstream fin("date.txt");

struct Coord{
    int x;
    int y;
};

struct Rotation{
    Coord blocks[100];
    int crash[100] = {-1};
};

struct Piesa{
    Coord blocks[100];
    int symbol;
    int weight;
    int length;
    Rotation rotation[5];
    int rotationLength;
};

Piesa piesa[10];

const int height = 15;
const int weight = 20;

int fps = 50;

int piecesAmount;

int score = 0;

int grid[height + 5][weight + 5];

int currentKey = 0;

int movePerFrame = 20;

bool falling = false;
bool gameWorks = true;

void loadPieces()
{

    int nrPiese, coordsCount;
    fin>>nrPiese;

    for(int i = 1; i <= nrPiese; i++)
    {
        fin>>coordsCount;
        fin>>piesa[i].rotationLength;

        piesa[i].symbol = 65+i;
        piesa[i].length = coordsCount;

        for(int rot = 1; rot <= piesa[i].rotationLength; rot++){

            for(int c = 1; c <= coordsCount; c++){

                fin>>piesa[i].rotation[rot].blocks[c].x;
                fin>>piesa[i].rotation[rot].blocks[c].y;

                int x = piesa[i].rotation[rot].blocks[c].x;
                int y = piesa[i].rotation[rot].blocks[c].y;

                if(y > piesa[i].weight)
                    piesa[i].weight = y;
            }
        }
        piesa[i].weight ++;
    }

    piecesAmount = nrPiese;
}

void findCrash()
{
    for(int i = 1; i <= piecesAmount; i++){
        for(int rot = 1; rot <= piesa[i].rotationLength; rot++){
            for(int c = 1; c <= piesa[i].length; c++){

                int y = piesa[i].rotation[rot].blocks[c].y;
                int x = piesa[i].rotation[rot].blocks[c].x;

                if(y >= piesa[i].rotation[rot].crash[x])
                    piesa[i].rotation[rot].crash[x] = y;
            }
        }
    }
}

void drawGrid()
{
    if(gameWorks){
        cout<<"Scor: "<<score<<endl;
        cout<<endl;

        for(int i = 0; i <= height + 1; i++){
            for(int j = 0; j <= weight + 1; j++){
                if(grid[i][j] == 0) cout<<" ";
                else cout<<(char)grid[i][j];
            }
            cout<<endl;
        }
    } else {
        cout<<"Ai pierdut! Scor total: "<<score<<endl;
        cout<<"Apasa SPACE pentru a inchide jocul.";
    }
}

void bordare()
{
    for(int i = 0; i <= height + 1; i++)
        for(int j = 0; j <= weight + 1; j++)
            grid[i][j] = 0;

    for(int i = 0; i <= height + 1; i++){
        grid[i][0] = 478;
        grid[i][weight + 1] = 477;
    }

    for(int i = 0; i <= weight + 1; i++){
        grid[0][i] = 476;
        grid[height + 1][i] = 479;
    }
}

void print(){
    for(int i = 0; i <= height + 1; i++){
        for(int j = 0; j <= weight + 1; j++)
            cout<<grid[i][j]<<" ";
        cout<<endl;
    }

}

void checkFilledRows(){
    for(int i = 2; i <= height; i++){
        int ok = true;
        for(int j = 1; j <= weight; j++)
            if(grid[i][j] == 0)
                ok = false;

        if(!ok) continue;

        for(int p = i ; p >= 2; p--)
            for(int j = 1; j <= weight; j++){
                grid[p][j] = grid[p - 1][j];

            }
        score++;
    }

}

bool checkFirstRow(){
    for(int j = 1; j <= weight; j++)
        if(grid[1][j] != 0)
            return true;
    return false;
}



void refresh(){
    drawGrid();
    Sleep(1000/(fps * movePerFrame));
    system("cls");
}

bool isHeld(char key){
    return GetKeyState(key) & 0x8000;
}

bool isPressed(char key){
    return GetAsyncKeyState(key) & 0x8000;
}




int main(){

    srand(time(NULL));

    bordare();
    loadPieces();

    findCrash();

    do{
        if(!gameWorks){
            refresh();
            if(GetAsyncKeyState(VK_SPACE) & 0x8000){
                break;
                return 0;
            }
        }
        else{
            if(!falling){
                falling = true;

                int currentY = 1;
                int nrPiesa = rand() % 4 + 1;
                int currentRot = 1;
                //int nrPiesa = 3;

                piesa[nrPiesa].symbol = rand() % 5 + 1;

                int currentX = rand() % weight + 1;

                if(currentX + piesa[nrPiesa].weight >= weight)
                    currentX -= abs(weight - (weight - piesa[nrPiesa].weight));


                bool crash = false;

                bool rotating = false;

                int refreshes = 0;

                while(falling){
                    crash = false;

                    if(isHeld('S')){
                        bool ok = true;

                        for(int index = 1; index <= piesa[nrPiesa].length; index++){
                            int y = currentY + piesa[nrPiesa].rotation[currentRot].blocks[index].y;
                            int x = currentX + piesa[nrPiesa].rotation[currentRot].blocks[index].x;

                            if(grid[y+1][x] != 0)
                                ok = false;
                        }

                        if(ok)
                            currentY ++;
                    }

                    if(isHeld('D')){
                        bool ok = true;

                        for(int index = 1; index <= piesa[nrPiesa].length; index++){
                            int y = currentY + piesa[nrPiesa].rotation[currentRot].blocks[index].y;
                            int x = currentX + piesa[nrPiesa].rotation[currentRot].blocks[index].x;

                            if(grid[y][x+1] != 0)
                                ok = false;
                        }

                        if(ok)
                            currentX++;
                    }
                    if(isHeld('A')){
                        bool ok = true;

                        for(int index = 1; index <= piesa[nrPiesa].length; index++){
                            int y = currentY + piesa[nrPiesa].rotation[currentRot].blocks[index].y;
                            int x = currentX + piesa[nrPiesa].rotation[currentRot].blocks[index].x;

                            if(grid[y][x-1] != 0)
                                ok = false;
                        }

                        if(ok)
                            currentX--;

                    }

                    if(isPressed('R')){
                        if(!rotating){
                            bool ok = true;
                            int tempRot = currentRot;
                            rotating = true;

                            if(currentRot == piesa[nrPiesa].rotationLength)
                                tempRot = 1;
                            else tempRot ++;

                            for(int index = 1; index <= piesa[nrPiesa].length; index++){
                                int y = currentY + piesa[nrPiesa].rotation[tempRot].blocks[index].y;
                                int x = currentX + piesa[nrPiesa].rotation[tempRot].blocks[index].x;

                                if(grid[y][x] != 0)
                                    ok = false;
                            }

                            if(ok) currentRot = tempRot;
                        }

                    }
                    else rotating = false;

                    refreshes++;

                    for(int index = 1; index <= piesa[nrPiesa].length; index++){

                        int y = currentY + piesa[nrPiesa].rotation[currentRot].blocks[index].y;
                        int x = currentX + piesa[nrPiesa].rotation[currentRot].blocks[index].x;

                        grid[y][x] = piesa[nrPiesa].symbol;

                    }

                    for(int index = 0; index <= piesa[nrPiesa].length; index++){

                        int x = currentX + piesa[nrPiesa].rotation[currentRot].blocks[index].x;
                        int defX = piesa[nrPiesa].rotation[currentRot].blocks[index].x;

                        int crashY = piesa[nrPiesa].rotation[currentRot].crash[defX] + 1;


                        if(grid[currentY + crashY][x] != 0)
                            crash = true;
                    }

                    if(crash)  //piece crashes
                    {

                        if(checkFirstRow()){
                            gameWorks = false;
                            falling  = false;
                        }

                        checkFilledRows();

                        falling = false;
                        break;
                    }

                    refresh();

                    for(int index = 1; index <= piesa[nrPiesa].length; index++){
                        grid[currentY + piesa[nrPiesa].rotation[currentRot].blocks[index].y]
                        [currentX + piesa[nrPiesa].rotation[currentRot].blocks[index].x] = 0;
                    }

                    if(refreshes == movePerFrame){
                        currentY++;
                        refreshes = 0;
                    }

                }
            }
        }
    } while(true);



    return 0;
}
