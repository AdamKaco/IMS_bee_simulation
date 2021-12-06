// IMS_bee_simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

#define MAX_ROW 40
#define MAX_COL 40
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


int getInitData();
int createStartRowPosition();
int createStartColPosition();
void test();
void newHive(int newRow, int newCol);
int getSurroundValue(int row, int col);
void initMap();

int main()
{
    getInitData();
    initMap();
    test();
}

void initMap() {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            map[i][j] = 0;
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
    value += map[rowPlus][col];
    value += map[rowPlus][colPlus];
    value += map[row][colPlus];
    value += map[rowMinus][colPlus];
    value += map[rowMinus][col];
    value += map[rowMinus][colMinus];
    value += map[row][colMinus];
    value += map[rowPlus][colMinus];

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
