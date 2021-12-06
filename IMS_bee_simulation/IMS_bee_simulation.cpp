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

int main()
{
    getInitData();
    test();
}


void test() {
    newHive(1, 1);
    newHive(2, 2);
    newHive(3, 3);

    auto hive_front = hives.begin();

    std::advance(hive_front, 0);

    std::cout << hive_front->row << '\n';
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
