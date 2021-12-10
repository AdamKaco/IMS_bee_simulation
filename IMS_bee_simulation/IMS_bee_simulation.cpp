#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <algorithm>
#include <chrono>
#include <thread>
#include "IMS_bee_simulation.h"

using namespace std;

int main()
{
    srand(time(NULL));
    getInitData();
    initMap();

    monthCounter = 0;
    for (int i = 0; i < YEARS_OF_SIM; i++) {
        for (int j = 0; j < 5; j++) {
            copyPreviousData();
            applyRulesThroughWholeMap();

            #ifdef _WIN32
                system("CLS");
            #endif
            
            #ifdef linux
                system("clear");
            #endif 

            cout << "year: " << i + 1 << "\n";
            cout << "month: " << j + 1 << "\n";
            cout << "hive map:" << "\n";
            printData();
            //printDataHoney();
            //printDataFlowersGot();
            //printDataFlowers();
            std::this_thread::sleep_for(std::chrono::milliseconds(SPEED_OF_SIMULATION));
            monthCounter = (monthCounter + 1) % NUM_OF_MONTHS;
        }
#ifdef _WIN32
        system("CLS");
#endif

#ifdef linux
        system("clear");
#endif 
        copyPreviousData();
        applyWinterThroughWholeMap();
        cout << endl << "winter " << "\n";
        printData();
        //printDataHoney();
        std::this_thread::sleep_for(std::chrono::milliseconds(SPEED_OF_SIMULATION*3));
    }

}

void applyWinterThroughWholeMap() {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            map[i][j].newHive = 1;
       
            if (map[i][j].hive != EMPTY) {
                map[i][j].bees = randomizer(WINTER_BEES);
                if (map[i][j].honey < WINTER_HONEY_THRESHOLD) {
                    destroyHive(i, j);
                }
                else {
                    map[i][j].honey -= WINTER_HONEY_THRESHOLD;
                    if (map[i][j].honey > WINTER_HONEY_THRESHOLD) {
                        map[i][j].honey = WINTER_HONEY_THRESHOLD;
                    }
                }
            }
        }
    }
}

void applyRulesThroughWholeMap() {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            applyRules(i, j);
        }
    }
}

void copyPreviousData() {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            prevMap[i][j] = map[i][j];
        }
    }
}

void setSurroundings(int* row, int* col, float* value, int* rowPlus, int* rowMinus, int* colPlus, int* colMinus) {
    *value = 0.0;
    *rowPlus = *row + 1;
    *rowMinus = *row - 1;
    *colPlus = *col + 1;
    *colMinus = *col - 1;
    if (*rowPlus == MAX_ROW) {
        *rowPlus = 0;
    }
    if (*colPlus == MAX_COL) {
        *colPlus = 0;
    }
    if (*rowMinus == -1) {
        *rowMinus = MAX_ROW - 1;
    }
    if (*colMinus == -1) {
        *colMinus = MAX_COL - 1;
    }
}

void calcFlowerValue(int row, int col, float* value) {
    float calc = 0.0;
    if (prevMap[row][col].flowers != 0) {
        setBeesThatWantFlower(row, col);
        if (prevMap[row][col].surrBees > 0)
            calc = (prevMap[row][col].flowers / prevMap[row][col].surrBees);
        *value += (calc);
    }
}

void getAvailableFlowers(int row, int col, int bees) {

    float value = 0;
    int rowPlus, rowMinus, colPlus, colMinus;
    setSurroundings(&row, &col, &value, &rowPlus, &rowMinus, &colPlus, &colMinus);

    //int bees = prevMap[row][col].bees;        
    calcFlowerValue(rowPlus, col, &value);
    calcFlowerValue(rowPlus, colPlus, &value);
    calcFlowerValue(row, colPlus, &value);
    calcFlowerValue(rowMinus, colPlus, &value);
    calcFlowerValue(rowMinus, col, &value);
    calcFlowerValue(rowMinus, colMinus, &value);
    calcFlowerValue(row, colMinus, &value);
    calcFlowerValue(rowPlus, colMinus, &value);

    map[row][col].hiveFlowers = bees * value; //hiveFlowers se pøepoèítá na med
    prevMap[row][col].hiveFlowers = bees * value;
}

void goodPlace(int row, int col, int* x, int* y, int* flowerCount, int bees) {
    getAvailableFlowers(row, col, bees);
    int flowersAround = map[row][col].hiveFlowers;
    if (prevMap[row][col].hive == EMPTY && flowersAround > *flowerCount) {
        *x = row;
        *y = col;
        *flowerCount = flowersAround;
    }
    else if (prevMap[row][col].hive == EMPTY && *flowerCount == 0) {
        *x = row;
        *y = col;
        *flowerCount = 0;
    }
}

void swarming(int row, int col, int count) {
    //cout << "swarming" << endl;
    float value;
    int rowPlus, rowMinus, colPlus, colMinus;
    setSurroundings(&row, &col, &value, &rowPlus, &rowMinus, &colPlus, &colMinus);

    //Finding the new place for a hive
    //With best ratio of flowers and bees
    int x = -1, y = -1, flowerCount = 0;
    int bees = prevMap[row][col].bees;
    goodPlace(rowPlus, col, &x, &y, &flowerCount, bees);
    goodPlace(rowPlus, colMinus, &x, &y, &flowerCount, bees);
    goodPlace(row, colPlus, &x, &y, &flowerCount, bees);
    goodPlace(rowMinus, colPlus, &x, &y, &flowerCount, bees);
    goodPlace(rowMinus, col, &x, &y, &flowerCount, bees);
    goodPlace(rowMinus, colMinus, &x, &y, &flowerCount, bees);
    goodPlace(row, colMinus, &x, &y, &flowerCount, bees);
    goodPlace(rowPlus, colMinus, &x, &y, &flowerCount, bees);
    if (x > 0) {
        map[x][y].bees = count;
        map[x][y].hive = 2;
        prevMap[x][y].newHive = 2;
        //cout << bees << endl;

        prevMap[x][y].bees = count;
        prevMap[x][y].hive = 2;
        prevMap[x][y].newHive = 2;
    }

    //else bees die, because no flowers around
}

int applyRules(int row, int col) {
    if (prevMap[row][col].flowers != 0) {
        //setBeesThatWantFlower(row, col);
    }

    if (prevMap[row][col].hive != EMPTY) {

        //if population dropped bellow 8000
        //hive need to find better place to live          
        if (prevMap[row][col].bees < 10000) {
            int count = prevMap[row][col].bees;
            swarming(row, col, count);
            map[row][col].bees = 0;
            map[row][col].hive = 0;
        }
        getAvailableFlowers(row, col, prevMap[row][col].bees);
        ruleHoneyGain(row, col);
        rulePopulationGain(row, col);
    }


    return 0;

}

void setBeesThatWantFlower(int row, int col) {
    float value;
    int rowPlus, rowMinus, colPlus, colMinus;
    setSurroundings(&row, &col, &value, &rowPlus, &rowMinus, &colPlus, &colMinus);

    value += prevMap[row][col].bees;
    value += prevMap[rowPlus][col].bees;
    value += prevMap[rowPlus][colPlus].bees;
    value += prevMap[row][colPlus].bees;
    value += prevMap[rowMinus][colPlus].bees;
    value += prevMap[rowMinus][col].bees;
    value += prevMap[rowMinus][colMinus].bees;
    value += prevMap[row][colMinus].bees;
    value += prevMap[rowPlus][colMinus].bees;
    prevMap[row][col].surrBees = value;
}

void ruleHoneyGain(int row, int col) {
    // theoretical most flowers hive can collect
    //0.8  to average bee population throughout month
    int flowersCollected = map[row][col].bees * PER_OF_FEMALE_BEES * PER_OF_BEES_COLLECTING * BEST_FLOWERS_PER_DAY_COLLECTED;

    // actual number hive can collect
    if (flowersCollected > map[row][col].hiveFlowers) {
        flowersCollected = map[row][col].hiveFlowers;
    }

    flowersCollected = randomizer(flowersCollected);

    float flowersMath = (float)flowersCollected;

    //maybe add weather factor
    // adding honey based on income
    map[row][col].honey = prevMap[row][col].honey + (flowersMath / FLOWES_FOR_KG_HONEY) * 30;
    // hive of 50 000 bees require 500g of honey a day to survive
    float bees = (float)prevMap[row][col].bees;

    map[row][col].honey -=  bees / 50000 * 0.5 * 30;
    prevMap[row][col].honey = map[row][col].honey;
}

void rulePopulationGain(int row, int col) {
  
    if (monthCounter % NUM_OF_MONTHS > 1) {
        map[row][col].bees = prevMap[row][col].bees + randomizer(eggs[monthCounter]) * 30 - (randomizer(eggs[monthCounter - 1]) * 30 / 2 + randomizer(eggs[monthCounter - 2]) * 30 / 2) / prevMap[row][col].newHive;
    }
    else if (monthCounter % NUM_OF_MONTHS == 1) {
        map[row][col].bees = prevMap[row][col].bees + randomizer(eggs[monthCounter]) * 30 - (randomizer(eggs[monthCounter - 1]) * 30 / 2) / prevMap[row][col].newHive;
    }
    else {
        map[row][col].bees = prevMap[row][col].bees + randomizer(eggs[monthCounter]) * 30;
    }
    
    //nicenie
    if (map[row][col].honey <= 0.0) {
        destroyHive(row,col);
    }

    //oddelovanie
    if (prevMap[row][col].bees > randomizer(BEE_SWARMING_THRESHOLD)) {
        map[row][col].bees /= 2;
        map[row][col].newHive = 2;
        int count = map[row][col].bees;
        //Finding a location to live
        swarming(row, col, count);
    }
}

void destroyHive(int row, int col) {
    map[row][col].bees = 0;
    map[row][col].hive = 0;
    map[row][col].honey = 0.0;
    map[row][col].newHive = 1;

}

int generateFlowerCount() {

#ifndef LOW_FLOWERS
    return (rand() % MAX_SPWAN_FLOWERS) * 100000;
#endif

    //creates more areas of low flower count
#ifdef LOW_FLOWERS
    if ((rand() % 5) > 1) {
        return (rand() % MAX_SPWAN_FLOWERS/3) * 100000;
    }
    else {
        return (rand() % MAX_SPWAN_FLOWERS) * 100000;
    }
#endif


}
void spawnFlowers() {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            map[i][j].flowers = generateFlowerCount();
        }
    }
}

int getSurroundFlower(int row, int col) {
    float value;
    int rowPlus, rowMinus, colPlus, colMinus;
    setSurroundings(&row, &col, &value, &rowPlus, &rowMinus, &colPlus, &colMinus);
    //add surrounding values
    value += prevMap[rowPlus][col].flowers;
    value += prevMap[rowPlus][colPlus].flowers;
    value += prevMap[row][colPlus].flowers;
    value += prevMap[rowMinus][colPlus].flowers;
    value += prevMap[rowMinus][col].flowers;
    value += prevMap[rowMinus][colMinus].flowers;
    value += prevMap[row][colMinus].flowers;
    value += prevMap[rowPlus][colMinus].flowers;

    return value;
}
void initMap() {
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            map[i][j].hive = 0;
            map[i][j].flowers = 0;
            map[i][j].honey = 0.0;
            map[i][j].bees = 0;
            map[i][j].newHive = 1;
        }
    }

    spawnFlowers();
    //TODO instead of 2 some value
    int x, y;
    do
    {
        x = createStartRowPosition();
        y = createStartColPosition();

    } while (getSurroundFlower(x, y) > 100);

    map[x][y].hive = 2;
    map[x][y].bees = 15000;
}


int getSurroundValue(int row, int col) {
    float value;
    int rowPlus, rowMinus, colPlus, colMinus;
    setSurroundings(&row, &col, &value, &rowPlus, &rowMinus, &colPlus, &colMinus);

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
    return rand() % MAX_ROW;
}

int createStartColPosition() {
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

int randomizer(int value) {
    int per = rand() % 20 + 90;
    return value * per / 100;
}

void printEndValues() {
    long tmp_value = 0;
    long tmp2 = 0;
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            tmp_value += map[i][j].flowers;
            if (map[i][j].hive != EMPTY) {
                tmp2++;
            }
        }
    }
    cout << tmp_value << endl;
    cout << tmp2 << endl << endl;
}

void printDataHoney() {
    cout << "\n";
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            // dont show low flower areas
            if (map[i][j].hive != EMPTY) {
                cout << (int)map[i][j].honey;
            }
            else {
                cout << " ";
            }
            cout << " ";
        }
        cout << "\n";
    }
}

void printDataFlowersGot() {
    cout << "\n";
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            // dont show low flower areas
            if (map[i][j].hive != EMPTY) {
                cout << map[i][j].hiveFlowers;
            }
            else {
                cout << " ";
            }
            cout << " ";
        }
        cout << "\n";
    }
}

void printData() {
    cout << "\n";
    cout << "\n";
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            if (map[i][j].hive == EMPTY) {
                cout << " ";
            }
            else {
                //cout << map[i][j].bees / (BEE_SWARMING_THRESHOLD/5);
                cout << "*";
            }
            cout << " ";
        }
        cout << "\n";
    }
}

void printDataFlowers() {
    cout << "\n";
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            // dont show low flower areas
            if (map[i][j].flowers / (MAX_SPWAN_FLOWERS * 10000) > 2) {
                cout << map[i][j].flowers / (MAX_SPWAN_FLOWERS * 10000);
            }
            else {
                cout << " ";
            }
            cout << " ";
        }
        cout << "\n";
    }
}