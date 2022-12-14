#include "defs.h"

// link in graphics, runner, main

/* 
 * void initChase(ChaseType* game)
 * Initializes the game by creating the game object and setting up the ncurses library.
 * out: ChaseType* game - pointer to the initialized game object
 */
void initChase(ChaseType* game)
{
	srand( (unsigned)time(NULL)); // sets random seed to the current system time 
	initNcurses(); // initialize the ncurses library, our 'GUI'

	if(sem_init(&game->mutex, 0, 1) < 0) // init mutex
		exit(1);
	
	PositionType p;
	p.row = randm(MAX_ROW);
	p.col = 0;
	
	game->numRunners = 0;
	
	game->timmy = (RunnerType*) malloc(sizeof(RunnerType));
	initRunner(game->timmy, 'T', GOOD, ALIVE, p); 
	
	addRunner(game, game->timmy);
	
	p.row = randm(MAX_ROW);
	
	game->harold = (RunnerType*) malloc(sizeof(RunnerType));
	initRunner(game->harold, 'H', GOOD, ALIVE, p); 
	
	addRunner(game, game->harold);
	
	pthread_create(&game->outThr, NULL, drawT, game);
	/**/
}

/* 
 * cleanupChase(ChaseType* game)
 * Frees all memory allocated to the game object.
 * in: ChaseType* game - pointer to the game object
 */
void cleanupChase(ChaseType* game)
{
	for(int i = 0; i < game->numRunners; i++)
		cleanupRunner(game->runners[i]);
	
	free(game);
}

/* 
 * drawVale2Screen(ValeType* vale)
 * Draws the vale to the screen by printing each character of the vale's grid to the corresponding location.
 * in: ValeType* vale - pointer to the vale object
 */
void drawVale2Screen(ValeType* vale)
{
	for(int c = 0; c < MAX_ROW; c++)
	{
		for(int r = 0; r < MAX_COL; r++)
		{
			char a = vale->grid[c][r];
			scrPrt(&a, c, r); // should draw at the right location? 
			//scrPrt("X", c, r);
			scrPrt("  ", c, MAX_COL); // get rid of flicker
		}
		scrPrt("| ", c, MAX_COL); // this took way too long to figure out... added this so that the end looks nicer (no random chars)
	}
	
	for(int r = 0; r < MAX_COL; r++) // optionally added an extra bar at the bottom
		scrPrt("_", MAX_ROW, r);
}

/* 
 * initVale(ValeType* vale, char c)
 * Initializes all positions in the vale to the given character.
 * out: ValeType* vale - pointer to the vale object
 * in: char c - the character to initialize the vale to 
 */
void initVale(ValeType* vale, char c)
{
	for(int x = 0; x < MAX_ROW; x++)
		for(int y = 0; y < MAX_COL; y++)
			vale->grid[x][y] = c; // initialize vale to char
}

/* 
 * void* drawT(void* g);
 * a thread function used to draw players and enemies onto the vale (the screen). Uses
 * semaphore mutex in game function to protect critical code.
 * in: void* g - pointer to the game object, needs to be passed in with void ptr because this is thread func
 */
void* drawT(void *g)
{
	ChaseType* game = (ChaseType*) g;
	
	while(1)
	{
		ValeType vale;
		initVale(&vale, ' ');

		for(int i = 0; i < game->numRunners; i++) // draws players first then enemies, correct order
		{
			if(0 <= game->runners[i]->pos.col && game->runners[i]->pos.col < MAX_COL && 0 <= game->runners[i]->pos.row && game->runners[i]->pos.row < MAX_ROW)
			{
				if (sem_wait(&game->mutex) < 0)
					exit(1);
				
				vale.grid[game->runners[i]->pos.row][game->runners[i]->pos.col] = game->runners[i]->avatar;
				
				if (sem_post(&game->mutex) < 0)
					exit(1);
			}
		}
		
		drawVale2Screen(&vale);
	}
}

/* 
 * spawnEnemyAtChance(ChaseType* game)
 * Randomly spawns either a ninja or dorc enemy at the given chance (described in instructions 30% for dorc, 5% for ninja).
 * in: ChaseType* game - pointer to the game object
 */
void spawnEnemyAtChance(ChaseType* game)
{	
	if (sem_wait(&game->mutex) < 0)
		exit(1);
	
	int spawnChance = randm(100) + 1; // return 1 - 100
	
	if (spawnChance <= 5) 
	{
		// spawn a ninja
		RunnerType* ninja = malloc(sizeof(RunnerType));
		
		PositionType p;
		p.col = randm(MAX_COL);
		p.row = randm(MAX_ROW);
		
		initRunner(ninja, 'N', EVIL, WALK_DEAD, p);
		addRunner(game, ninja);
	} 
	else if (spawnChance <= 35) 
	{
		// spawn a dorc
		RunnerType* dorc = malloc(sizeof(RunnerType));
		
		PositionType p;
		p.col = 0;
		p.row = randm(MAX_ROW);
		
		initRunner(dorc, 'd', EVIL, WALK_DEAD, p);
		addRunner(game, dorc);
	}
	
	if (sem_post(&game->mutex) < 0)
		exit(1);
}

/* 
 * int executeAllBehavior(ChaseType* game)
 * Executes the behavior of each runner according to their probability.
 * in/out: ChaseType* game - pointer to the game object
 * return: int - C_TRUE if player won, else CONTINUE
 */
int executeAllBehavior(ChaseType* game)
{
	int ret = CONTINUE;
	
	for(int i = 0; i < game->numRunners; i++)
	{
		int prob = randm(100) + 1; // this should randomly select and run one of the behaviours, range is [1-100] check is for 0 or above
		int c = 0;
		
		while(prob > 0 && game->runners[i]->numBehav != 0)
		{
			prob -= game->runners[i]->behavArr[c]->prob;
			
			if(prob > 0)
				c++;
		}

		if(game->runners[i]->numBehav != 0)
		{
			game->runners[i]->behavArr[c]->bFunc(game, game->runners[i]);
			
			if(runnerBoundPosition(game->runners[i])) // make sure that the runner stays in bounds, 
				ret = C_TRUE; // but if they are a player and they got past the right bound then they win
		}
		
	}
	
	return ret;
}

/* 
 * detectCollision(ChaseType* game)
 * Detects collisions between the characters and updates the game accordingly.
 * in/out: ChaseType* game - pointer to the game object
 */
void detectCollision(ChaseType* game)
{
	for(int i = 0; i < game->numRunners; i++) // collision detection
	{
		if(game->runners[i]->moral == EVIL)
		{
			if (sem_wait(&game->mutex) < 0)
				exit(1); // semaphore crit error
		
			if(game->runners[i]->pos.row == game->timmy->pos.row && game->runners[i]->pos.col == game->timmy->pos.col && !(game->timmy->st == DEAD))
				killRunner(game->timmy);
			else if(game->runners[i]->pos.row == game->harold->pos.row && game->runners[i]->pos.col == game->harold->pos.col && !(game->harold->st == DEAD))
				killRunner(game->harold);
			
			if (sem_post(&game->mutex) < 0)
				exit(1);
		}
	}
}

/* 
 * int gameRun(ChaseType* game)
 * Runs the game loop.
 * in: ChaseType* game - pointer to the game object
 * return: int - win condition, C_TRUE for winning, C_FALSE for loss, else returns CONTINUE.
 */
int gameRun(ChaseType* game)
{
	spawnEnemyAtChance(game);
	
	int c = executeAllBehavior(game); // returns either true (behaviour occured that resulted in win) or continue
	
	detectCollision(game);
	
	if (sem_wait(&game->mutex) < 0)
		exit(1);
	
	if(game->timmy->st == DEAD && game->harold->st == DEAD)
		return C_FALSE; // loss state
	
	if (sem_post(&game->mutex) < 0)
		exit(1);
	
	return c; // executeAllBehavior, when no win occures, returns CONTINUE
}


