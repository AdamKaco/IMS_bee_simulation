#define SPEED_OF_SIMULATION 100

#define MAX_ROW 31
#define MAX_COL 30
#define NUM_OF_MONTHS 5

#define EMPTY 0
#define HIVE 1
#define BEE 2
#define FLOWER 3

#define MAX_SPWAN_FLOWERS 60 // in hundres of thousunds
#define YEARS_OF_SIM 20

#define PER_OF_FEMALE_BEES 0.85
#define PER_OF_BEES_COLLECTING 0.3
#define BEST_FLOWERS_PER_DAY_COLLECTED 1000 
#define FLOWES_FOR_KG_HONEY 12000000 
#define BEE_SWARMING_THRESHOLD 55000
#define WINTER_HONEY_THRESHOLD 5
#define WINTER_BEES 15000
//#define LOW_FLOWERS

struct Map
{
    int hive;
    int bees;
    float honey;
    int flowers;
    int hiveFlowers;
    int surrBees;
    int newHive;
};

struct Map map[MAX_ROW][MAX_COL];
int eggs[NUM_OF_MONTHS];

struct Map prevMap[MAX_ROW][MAX_COL];
int monthCounter;

//function gets data from .txt input
int getInitData();
// returns random row
int createStartRowPosition();
//returns random collum
int createStartColPosition();
// returns surrounding data
int getSurroundValue(int row, int col);
// iniatialize field
void initMap();
// function applies rules
int applyRules(int row, int col);
// save previous state
void copyPreviousData();
// print hive positions
void printData();
// go through whole map and apply all rules
void applyRulesThroughWholeMap();
// adds flower a numeric value of how many bees are near by
void setBeesThatWantFlower(int row, int col);
// returns available flowers for hive
void getAvailableFlowers(int row, int col);
// based on rule change population values
void rulePopulationGain(int row, int col);
// based on rule change honey values
void ruleHoneyGain(int row, int col);
// print generated map of flowers
void printDataFlowers();
// prints how much honey do hives have
void printDataHoney();
// destroy hive
void destroyHive(int row, int col);
// applying winter rules on hives
void applyWinterThroughWholeMap();
// randomize values to mimic real world 
int randomizer(int value);
//prints how many flowers hive collected
void printDataFlowersGot();
//prints data after simulation was finished
void printEndValues();