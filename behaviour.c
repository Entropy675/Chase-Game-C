#include "defs.h"

// needs to be linked to runner and main
/* 
 * b_walk(ChaseType* game, RunnerType* t)
 *  - behaviour function
 * Moves a villan one row and one column in the direction of the closest player, 
 * Moves the player one column towards the goal. Uses semaphores to protect the critical sections.
 * in/out: ChaseType* game - pointer to the game object
 * in/out: RunnerType* t - pointer to the runner 
 */
void b_walk(ChaseType* game, RunnerType* t)
{
	if(t->moral == EVIL)
	{
		// move one row and one column in direction towards closest player
		PositionType closePlr = (closestPlrToEnmy(game, t))->pos;
		
		if (sem_wait(&game->mutex) < 0)
			exit(1);
		
		if (t->pos.row > closePlr.row) 
			t->pos.row--;
		else 
			t->pos.row++;
		
		if (t->pos.col > closePlr.col) 
			t->pos.col--;
		else 
			t->pos.col++;
		
		if (sem_post(&game->mutex) < 0)
			exit(1);
	}
	else
	{
		if (sem_wait(&game->mutex) < 0)
			exit(1);
		
		t->pos.col++; // go towards goal by one
		    
		if (sem_post(&game->mutex) < 0) 
			exit(1);
    }
}


/*
 * void b_sleep(ChaseType* game, RunnerType* t)
 *  - behaviour function
 * Causes the given runner to sleep for a brief period of time, effectively does nothing.
 * Intended to be used with a function pointer (Essentially a "nothing" value).
 * in: ChaseType* game - pointer to the game object
 * in: RunnerType* t - pointer to the runner 
 */
void b_sleep(ChaseType* game, RunnerType* t){}

/*
 * b_jump(ChaseType* game, RunnerType* t)
 *  - behaviour function
 * Moves a villan one row and one column in the direction of the closest player,
 * or moves a hero one column towards the goal if the runner is good. Uses semaphores to protect critical sections.
 * in/out: ChaseType* game - pointer to the game object
 * in/out: RunnerType* t - pointer to the runner 
 */
void b_jump(ChaseType* game, RunnerType* t)
{
	
	int a = randm(8); // num btwn [0,7] inc
	
	if(t->moral == EVIL)
	{
		int b = randm(8); // num btwn [0,7]
		
		PositionType closePlr = closestPlrToEnmy(game, t)->pos;
		
		if (sem_wait(&game->mutex) < 0)
			exit(1);
		
		if (t->pos.row > closePlr.row)
			t->pos.row -= a;
		else 
			t->pos.row += a;
		
		if (t->pos.col > closePlr.col) 
			t->pos.col -= b;
		else 
			t->pos.col += b;
		
		if (sem_post(&game->mutex) < 0)
			exit(1);
	}
	else
	{
		if (sem_wait(&game->mutex) < 0)
			exit(1);
		
		t->pos.col += a;
		
		if (sem_post(&game->mutex) < 0)
			exit(1);
	}
	
}

/*
 * b_slide(ChaseType* game, RunnerType* t)
 *  - behaviour function
 * Slides the given runner back by a random amount. Uses semaphores to protect the critical sections.
 * in/out: ChaseType* game - pointer to the game object
 * in/out: RunnerType* t - pointer to the runner 
 */
void b_slide(ChaseType* game, RunnerType* t) // slide runner back by random amount
{
	int a = randm(5); // num btwn [0,4]
	
	if (sem_wait(&game->mutex) < 0)
		exit(1);
	
	t->pos.col -= a;
	
	if (sem_post(&game->mutex) < 0)
		exit(1);
}

/*
 * b_teleport(ChaseType* game, RunnerType* t)
 *  - behaviour function
 * Teleports the given runner to a random location in the game. Uses semaphores to protect the critical sections. 
 * in/out: ChaseType* game - pointer to the game object
 * in/out: RunnerType* t - pointer to the runner 
 */
void b_teleport(ChaseType* game, RunnerType* t)
{
	int r, c;
	r = randm(MAX_ROW); // rnd num btween [0, MAX_ROW)
	c = randm(MAX_COL);
	
	if (sem_wait(&game->mutex) < 0)
		exit(1);
	
	t->pos.row = r;
	t->pos.col = c;
	    
	if (sem_post(&game->mutex) < 0)
		exit(1);
}

/* 
 * cleanupBehav(BehavType* a);
 * Frees memory allocated to a behavior object.
 * in: BehavType* a - pointer to the behavior object
 */
void cleanupBehav(BehavType* a)
{
	free(a);
}

/*
 * initBehav(BehavType* a, int p, BehavFuncType b_mov)
 * Initializes a behavior object.
 * out: BehavType* a - pointer to the behavior object
 * in: int prob - probability of a behavior
 * in: BehavFuncType b_mov - input function pointer to the behavior function
 */
void initBehav(BehavType* a, int p, BehavFuncType b_mov)
{
	//a = malloc(sizeof(BehavType));
	a->prob = p;
	a->bFunc = b_mov;
}

/*
	===============================
	Runner Behaviour Probabilities
	=========== Timmy =============
	Walk 						70%
	Jump 						25%
	Slide 						5%
	=========== Harold ============
	Walk 						30%
	Jump 						30%
	Slide 						10%
	Sleep 						30%
	=========== Dorcs =============
	Walk 						95%
	Jump 						5%
	=========== Ninjas ============
	Jump 						60%
	Teleport 					40%
	===============================
*/

/*
 * initTimmyBehav(RunnerType* r)
 * Initializes the behaviors of the given runner to the predefined behaviors of Timmy.
 * in/out: RunnerType* r - pointer to the runner 
 */
void initTimmyBehav(RunnerType* r)
{
	BehavType* a;
	
	a = (BehavType*) malloc(sizeof(BehavType));
	initBehav(a, 70, b_walk);
	r->behavArr[0] = a;
	
	a = (BehavType*) malloc(sizeof(BehavType));
	initBehav(a, 25, b_jump);
	r->behavArr[1] = a;
	
	a = (BehavType*) malloc(sizeof(BehavType));
	initBehav(a, 5, b_slide);
	r->behavArr[2] = a;
	
	r->numBehav = 3;
	//cleanupBehav(a); // since this isnt happening here for each one, remember to free in cleanup runner
}

/*
 * initHarBehav(RunnerType* r)
 * Initializes the behaviors of the given runner to the predefined behaviors of Harold.
 * in/out: RunnerType* r - pointer to the runner 
 */
void initHarBehav(RunnerType* r)
{
	BehavType* a;
	
	a = (BehavType*) malloc(sizeof(BehavType));
	initBehav(a, 30, b_walk);
	r->behavArr[0] = a;
	
	a = (BehavType*) malloc(sizeof(BehavType));
	initBehav(a, 30, b_jump);
	r->behavArr[1] = a;
	
	a = (BehavType*) malloc(sizeof(BehavType));
	initBehav(a, 10, b_slide);
	r->behavArr[2] = a;
	
	a = (BehavType*) malloc(sizeof(BehavType));
	initBehav(a, 30, b_sleep);
	r->behavArr[3] = a;
	
	r->numBehav = 4;
}

/*
 * initDorcBehav(RunnerType* r)
 * Initializes the behaviors of the given runner to the predefined behaviors of the Dorc.
 * in/out: RunnerType* r - pointer to the runner 
 */
void initDorcBehav(RunnerType* r)
{
	BehavType* a;
	
	a = (BehavType*) malloc(sizeof(BehavType));
	initBehav(a, 95, b_walk);
	r->behavArr[0] = a;
	
	a = (BehavType*) malloc(sizeof(BehavType));
	initBehav(a, 5, b_jump);
	r->behavArr[1] = a;
	
	r->numBehav = 2;
}

/*
 * initNiBehav(RunnerType* r)
 * Initializes the behaviors of the given runner to the predefined behaviors of the Ninja.
 * in/out: RunnerType* r - pointer to the runner 
 */
void initNiBehav(RunnerType* r)
{
	BehavType* a;
	
	a = (BehavType*) malloc(sizeof(BehavType));
	initBehav(a, 60, b_jump);
	r->behavArr[0] = a;
	
	a = (BehavType*) malloc(sizeof(BehavType));
	initBehav(a, 40, b_teleport);
	r->behavArr[1] = a;
	
	r->numBehav = 2;
}





