// IMS_bee_simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;

#define MAX_ROW 10
#define MAX_COL 10
#define NUM_OF_MONTHS 12

struct Map
{
    int hive;
    int bees;
    float honey;
    int flowers;
};

struct Map map[MAX_ROW][MAX_COL];
int eggs[NUM_OF_MONTHS];

struct Map prevMap[MAX_ROW][MAX_COL];


int getInitData();
int createStartRowPosition();
int createStartColPosition();
void test();
int getSurroundValue(int row, int col);
void initMap();
int applyRules(int row, int col);
void copyPreviousData();
void printData();
void applyRulesThroughWholeMap();



int main()
{
    getInitData();
    initMap();
    test();
    for (int i = 0; i < 5; i++) {
        copyPreviousData();
        applyRulesThroughWholeMap();
        #ifdef _WIN32
        system("CLS");
        #endif 
        #ifdef linux
        system("clear");
        #endif
        cout << "month: " << i << "\n";
        printData();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}



void applyRulesThroughWholeMap() {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            applyRules(i, j);
        }
    }
}


void printData() {
    cout << "\n";
    cout << "\n";
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            if (map[i][j].hive == 0) {
                cout << " ";
            }
            else {
                cout << map[i][j].hive;
            }
            cout << " ";
        }
        cout << "\n";
    }
}

void copyPreviousData() {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            prevMap[i][j] = map[i][j];
        }
    }
}

int applyRules(int row, int col) {
    if (prevMap[row][col].hive == 0) {
        return 0;
    }
    int surrValue = getSurroundValue(row, col);
    //chnage value of hive
    if (surrValue < 5) {
        map[row][col].hive = prevMap[row][col].hive+1;
    }
    else if (surrValue < 10) {
        map[row][col] = prevMap[row][col];
    }
    else{
        map[row][col].hive = prevMap[row][col].hive-1;
    }
    
    //nicenie
    if (map[row][col].hive == 0) {
        ;
    }

    //oddelovanie
    if (map[row][col].hive == 4) {
        map[row][col].hive = 2;
        

        

        int rowPlus = row + 1;
        int rowMinus = row - 1;
        int colPlus = col + 1;
        int colMinus = col - 1;
        int done = 0;

        if (rowPlus == MAX_ROW) {
            rowPlus = 0;
        }
        if (colPlus == MAX_COL) {
            colPlus = 0;
        }
        if (rowMinus == -1) {
            rowMinus = MAX_ROW - 1;
        }
        if (colMinus == -1) {
            colMinus = MAX_COL - 1;
        }

        //add surrounding values
        if (prevMap[rowPlus][col].hive == 0 && done == 0) {
            map[rowPlus][col].hive = 2;
            done = 1;
        }
        if (prevMap[rowPlus][colPlus].hive == 0 && done == 0) {
            map[rowPlus][colPlus].hive = 2;
            done = 1;
        }
        if (prevMap[row][colPlus].hive == 0 && done == 0) {
            map[row][colPlus].hive = 2;
            done = 1;
        }
        if (prevMap[rowMinus][colPlus].hive == 0 && done == 0) {
            map[rowMinus][colPlus].hive = 2;
            done = 1;
        }
        if (prevMap[rowMinus][col].hive == 0 && done == 0) {
            map[rowMinus][col].hive = 2;
            done = 1;
        }
        if (prevMap[rowMinus][colMinus].hive == 0 && done == 0) {
            map[rowMinus][colMinus].hive = 2;
            done = 1;
        }
        if (prevMap[row][colMinus].hive == 0 && done == 0) {
            map[row][colMinus].hive = 2;
            done = 1;
        }
        if (prevMap[rowPlus][colMinus].hive == 0 && done == 0) {
            map[rowPlus][colMinus].hive = 2;
            done = 1;
        }
    }

    return 0;

}

void initMap() {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            map[i][j].hive = 0;
            map[i][j].flowers = 0;
            map[i][j].honey = 0.0;
            map[i][j].bees = 0;
        }
    }
    int x = createStartRowPosition();
    int y = createStartColPosition();
    //TODO instead of 2 some value
    map[x][y].hive = 2;
    map[x][y].bees = 15000;
}

void test() {

}

int getSurroundValue(int row, int col) {
    int value = 0;
    int rowPlus = row + 1;
    int rowMinus = row - 1;
    int colPlus = col + 1;
    int colMinus = col - 1;

    if (rowPlus == MAX_ROW) {
        rowPlus = 0;
    }
    if (colPlus == MAX_COL) {
        colPlus = 0;
    }
    if (rowMinus == -1) {
        rowMinus = MAX_ROW-1;
    }
    if (colMinus == -1) {
        colMinus = MAX_COL-1;
    }

    //add surrounding values
    value += prevMap[rowPlus][col].hive;
    value += prevMap[rowPlus][colPlus].hive;
    value += prevMap[row][colPlus].hive;
    value += prevMap[rowMinus][colPlus].hive;
    value += prevMap[rowMinus][col].hive;
    value += prevMap[rowMinus][colMinus].hive;
    value += prevMap[row][colMinus].hive;
    value += prevMap[rowPlus][colMinus].hive;

    return value;
}



int createStartRowPosition() {
    srand(time(NULL));
    return rand() % MAX_ROW;
}

int createStartColPosition() {
    srand(time(NULL));
    return rand() % MAX_COL;
}

int getInitData() {
    string line;
    ifstream myfile("queen_eggs.txt");
    if (myfile.is_open())
    {
        int i = 0;
        while (getline(myfile, line))
        {
            eggs[i] = stoi(line);
            i++;
        }
        myfile.close();
    }
    else cout << "Unable to open file";

    return 0;
}
