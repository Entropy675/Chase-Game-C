#include "defs.h"

// needs to be linked with behaviour

/* 
 * initRunner(RunnerType *r, char av, MoralType m, StatusType s, PositionType p)
 * Initializes the given runner object based on the given parameters. The avatar is used to determine the runners behavior. 
 * out: RunnerType* r - pointer to the runner to be initialized
 * in: char av - the avatar of the runner
 * in: MoralType m - the moral type of the runner
 * in: StatusType s - the status of the runner
 * in: PositionType p - the position of the runner
 */
void initRunner(RunnerType *r, char av, MoralType m, StatusType s, PositionType p)
{
	// 'T' 'H' 'd' 'N' - the characters (in both meanings)
	
	r->avatar = av;
	r->moral = m;
	r->st = s;
	r->pos = p;

	if(av == 'T') // based on the av value, we will determine the runners behaviours
		initTimmyBehav(r);
	else if(av == 'H')
		initHarBehav(r);
	else if(av == 'd')
		initDorcBehav(r);
	else if(av == 'N')
		initNiBehav(r);
	else
		r->numBehav = 0;
}

/* 
 * cleanupRunner(RunnerType* r)
 * Frees the memory allocated for the given runner object, as well as the behavior associated with it.
 * in: RunnerType* r - pointer to the runner to be cleaned up
 */
void cleanupRunner(RunnerType* r)
{
	for(int i = 0; i < r->numBehav; i++)
		cleanupBehav(r->behavArr[i]);
	
	free(r);
}

/* 
 * addRunner(ChaseType* game, RunnerType* r) 
 * Adds the given runner object to the game object. This function should only be called when the mutex is locked (or irrelevent).
 * in/out: ChaseType* game - pointer to the game object
 * in: RunnerType* r - pointer to the runner to be added
 */
void addRunner(ChaseType* game, RunnerType* r)
{
	if (game->numRunners < MAX_ARR)
		game->runners[game->numRunners++] = r; // only add runners up till the max, after which ignore adds
}

/* 
 * killRunner(RunnerType* r)
 * Kills the given runner object by setting the avatar to '+', status to DEAD, and cleaning up any behaviors associated with it.
 * in/out: RunnerType* r - pointer to the runner to be killed
 */
void killRunner(RunnerType* r)
{
	r->avatar = '+';
	r->st = DEAD;
	
	for(int i = 0; i < r->numBehav; i++)
		cleanupBehav(r->behavArr[i]);
	
	r->numBehav = 0; // need an if statement for this when running behavior later
}

/* 
 * runnerBoundPosition(RunnerType* r)
 * If the position of the runner is out of bounds, then the position is set to the boundary. Returns whether or not the player won (game status).
 * in/out: RunnerType* r - pointer to the runner 
 * return: int - C_TRUE if the runner is the hero and has reached the boundary, else returns C_FALSE. 
 */
int runnerBoundPosition(RunnerType* r)
{
	if(r->pos.row >= MAX_ROW)
		r->pos.row = MAX_ROW-1;
		
	if(r->pos.col >= MAX_COL)
	{
		r->pos.col = MAX_COL-1;
		if(r->moral == GOOD)
			return C_TRUE; // the hero won the race
	}
	
	if(r->pos.row < 0)
		r->pos.row = 0;
	if(r->pos.col < 0)
		r->pos.col = 0;
		
	return C_FALSE;
}

/* 
 * findDist2Rnr(PositionType p, RunnerType* r)
 * Calculates the manhattan distance (2d grid distance) between the given point and runner.
 * in: PositionType p - point to calculate distance from
 * in: RunnerType* r - pointer to the runner
 * return: int - the distance between the point and runner 
 */
int findDist2Rnr(PositionType p, RunnerType* r)
{
	int a = abs(p.row - r->pos.row);
	int b = abs(p.col - r->pos.col);
	
	return (a < b) ? a : b; // btwn point to runner.pos
}

/* 
 * closestPlrToEnmy(ChaseType* game, RunnerType* t)
 * Finds the closest player to the given enemy by calculating the distance between them using findDist2Rnr.
 * in: ChaseType* game - pointer to the game object
 * in: RunnerType* t - pointer to the enemy 
 * return: RunnerType* - pointer to the closest player 
 */ 
void closestPlrToEnmy(ChaseType* game, RunnerType** r, RunnerType* t) // takes in game and enemy
{
	if(game->timmy->st == DEAD)
		*r = game->harold;
	else if(game->harold->st == DEAD)
		*r = game->timmy;
	else if(findDist2Rnr(t->pos, game->timmy) < findDist2Rnr(t->pos, game->harold))
		*r = game->timmy;
	else
		*r = game->harold;
}





