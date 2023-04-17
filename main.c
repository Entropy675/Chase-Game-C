#include "defs.h"

int main()
{
	ChaseType *game = malloc(sizeof(ChaseType)); // game object
	initChase(game);

	int c = CONTINUE; // win variable, C_TRUE is win, C_FALSE is loss.
	while(c == CONTINUE)
	{
		c = gameRun(game);
		usleep(VALE_OUTPUT_RATE); // at the speed of specification
	}
	
	pthread_cancel(game->outThr);
	
	
	// final vale print
	ValeType vale;
	initVale(&vale, ' '); // lets change the background to * for the ending, just for flair! (please dont doc marks ;-;)
	for(int i = game->numRunners - 1; i >= 0; i--) // draws enemies first then players if they are dead, correct order
		vale.grid[game->runners[i]->pos.row][game->runners[i]->pos.col] = game->runners[i]->avatar;

	drawVale2Screen(&vale);
	
	
	if(c == C_TRUE)
	{
		char s[MAX_ARR];
		
		if(game->harold->pos.col == MAX_COL-1)
			sprintf(s, "Harold escaped with the Emerald!");
		else
			sprintf(s, "Timmy escaped with the Emerald!");
		
		if(game->harold->st == DEAD)
			sprintf(s, " Unfortunately, Harold got caught... ");
		else if(game->timmy->st == DEAD)
			sprintf(s, " Unfortunately, Timmy got caught... ");
			
		scrPrt(s, MAX_ROW + 1, 0);
	}
	else if(c == C_FALSE)
		scrPrt("GAME OVER, both heros died!", MAX_ROW + 1, 0);
	
	cleanupNcurses(MAX_ROW + 2);
	cleanupChase(game); // free all mem alloc'd to game object
	
	printf("Thank you for playing!");
	return(0);
}

/*
 * int randm(int max);
 * Generates a pseudorandom number in the range [0, max).
 * in: int max - upper range of generated number
 * return: int - the randomly generated number
 */
int randm(int max)
{
	double r = ( (double)rand() / ((double)(RAND_MAX)+(double)(1)) ); 
	return (int)(r * max);
}

