// IMS_bee_simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>

using namespace std;

#define MAX_ROW 10
#define MAX_COL 10
#define NUM_OF_MONTHS 12

class Hive {
public:
    int row;
    int col;
    int bees;
    float honey;
    Hive(int, int);
};

int eggs[NUM_OF_MONTHS];
int map[MAX_ROW][MAX_COL];
list<Hive> hives;

int prevMap[MAX_ROW][MAX_COL];


int getInitData();
int createStartRowPosition();
int createStartColPosition();
void test();
void newHive(int newRow, int newCol);
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

    for (int i = 0; i < 40; i++) {
        cout << "month: " << i << "\n";
        copyPreviousData();
        applyRulesThroughWholeMap();
        printData();
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
            if (map[i][j] == 0) {
                cout << " ";
            }
            else {
                cout << map[i][j];
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
    if (prevMap[row][col] == 0) {
        return 0;
    }
    int surrValue = getSurroundValue(row, col);
    //chnage value of hive
    if (surrValue < 5) {
        map[row][col] = prevMap[row][col]+1;
    }
    else if (surrValue < 10) {
        map[row][col] = prevMap[row][col];
    }
    else{
        map[row][col] = prevMap[row][col]-1;
    }
    
    if (map[row][col] == 0) {
        ;
    }

    if (map[row][col] == 4) {
        map[row][col] = 2;
        

        

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
        if (prevMap[rowPlus][col] == 0 && done == 0) {
            map[rowPlus][col] = 2;
            done = 1;
        }
        if (prevMap[rowPlus][colPlus] == 0 && done == 0) {
            map[rowPlus][colPlus] = 2;
            done = 1;
        }
        if (prevMap[row][colPlus] == 0 && done == 0) {
            map[row][colPlus] = 2;
            done = 1;
        }
        if (prevMap[rowMinus][colPlus] == 0 && done == 0) {
            map[rowMinus][colPlus] = 2;
            done = 1;
        }
        if (prevMap[rowMinus][col] == 0 && done == 0) {
            map[rowMinus][col] = 2;
            done = 1;
        }
        if (prevMap[rowMinus][colMinus] == 0 && done == 0) {
            map[rowMinus][colMinus] = 2;
            done = 1;
        }
        if (prevMap[row][colMinus] == 0 && done == 0) {
            map[row][colMinus] = 2;
            done = 1;
        }
        if (prevMap[rowPlus][colMinus] == 0 && done == 0) {
            map[rowPlus][colMinus] = 2;
            done = 1;
        }
    }

    return 0;

}

void initMap() {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            map[i][j] = 0;
            freeMapSpaces[i * MAX_ROW + j] = i * MAX_ROW + j;
            
        }
    }
    int x = createStartRowPosition();
    int y = createStartColPosition();
    //TODO instead of 2 some value
    map[x][y] = 2;

    newHive(x, y);
}

void test() {
    newHive(1, 1);
    newHive(2, 2);
    newHive(3, 3);

    auto hive_front = hives.begin();

    std::advance(hive_front, 0);

    std::cout << hive_front->row << '\n';
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
    value += prevMap[rowPlus][col];
    value += prevMap[rowPlus][colPlus];
    value += prevMap[row][colPlus];
    value += prevMap[rowMinus][colPlus];
    value += prevMap[rowMinus][col];
    value += prevMap[rowMinus][colMinus];
    value += prevMap[row][colMinus];
    value += prevMap[rowPlus][colMinus];

    return value;
}

Hive::Hive(int rowInit, int colInit) {
    row = rowInit;
    col = colInit;
    bees = 30000;
    honey = 0.0;
}


void newHive(int newRow, int newCol) {
    Hive newHive(newRow, newCol);
    hives.insert(hives.begin(), newHive);
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
