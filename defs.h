#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <curses.h>

#define C_OK       0
#define C_NOK     -1

#define C_TRUE     1
#define C_FALSE    0

#define CONTINUE  -1

#define C_DIR_POS  1
#define C_DIR_NEG -1

#define DORC_SPAWN_RATE       30
#define NINJA_SPAWN_RATE       5
#define VALE_OUTPUT_RATE   50000

#define MAX_ROW   15
#define MAX_COL   80
#define MAX_STR   64
#define MAX_ARR  128

typedef enum {GOOD, NEUTRAL, EVIL} MoralType;
typedef enum {ALIVE, DEAD, WALK_DEAD} StatusType;

typedef struct {
  int row;
  int col;
} PositionType;  // row major order, give me back my x and y ;-;

struct Chase;
struct Runner;

typedef void (*BehavFuncType)(struct Chase*, struct Runner*);
// function pointer to any of the behaviour functions
// (They all have the same input params, this is required for function pointers.)

typedef struct {
  int prob;
  BehavFuncType bFunc;
} BehavType;

typedef struct Runner {
  char avatar; // 'T' 'H' 'd' 'N' - the characters (in both meanings)
  MoralType moral; // enum (literally just integers) alignment 
  StatusType st; // life status, enum: alive, dead, or undead.
  PositionType pos; // position on the grid (row = y, col = x)
  BehavType *behavArr[MAX_ARR];
  int numBehav;
} RunnerType;

typedef struct {
  char grid[MAX_ROW][MAX_COL];
} ValeType;

typedef struct Chase {
  RunnerType *timmy;
  RunnerType *harold;
  RunnerType *runners[MAX_ARR];
  int numRunners;
  pthread_t outThr;
  sem_t mutex;
} ChaseType;


/* Provided functions */
int randm(int);
void initNcurses();
void cleanupNcurses(int);
void scrPrt(char*, int, int);

/* Behaviour functions */
void b_sleep(ChaseType*, RunnerType*); // movement here
void b_walk(ChaseType*, RunnerType*);  // make a BehavFuncType function variable set it to these
void b_jump(ChaseType*, RunnerType*);  // call a BehavFuncType for movement behav
void b_slide(ChaseType*, RunnerType*);
void b_teleport(ChaseType*, RunnerType*);
void initTimmyBehav(RunnerType*); 
void initHarBehav(RunnerType*);
void initDorcBehav(RunnerType*);
void initNiBehav(RunnerType*);
void initBehav(BehavType*, int, BehavFuncType);
void cleanupBehav(BehavType*);

/* Runner Functions */
void initRunner(RunnerType*, char, MoralType, StatusType, PositionType);
void cleanupRunner(RunnerType*);
void addRunner(ChaseType*, RunnerType*); // add runner to chases runners
void killRunner(RunnerType*);
int runnerBoundPosition(RunnerType* r);
int findDist2Rnr(PositionType a, RunnerType* plr);
void closestPlrToEnmy(ChaseType*, RunnerType**, RunnerType*);

/* Chase Functions */
void initChase(ChaseType*);
void cleanupChase(ChaseType*);
void initVale(ValeType*, char);
void* drawT(void*); // the main function for the draw thread, pass in a vale
int gameRun(ChaseType*);
void spawnEnemyAtChance(ChaseType*);
int executeAllBehavior(ChaseType*);
void drawVale2Screen(ValeType*);

