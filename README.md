# Chase Game C

Author: Sibte Kazmi
OS: Linux

Included files: main.c, behaviour.c, chase.c, graphics.c, runner.c, defs.h, Makefile, readme.txt


Compilation Command: make 

Run Command: ./cg


Description: 

This code implements a game of chase. It creates runners, such as a Timmy, Harold, Dorcs, and Ninjas, which move around on a the Vale.
Each character has their own movement behavoir.
Uses multithreading for display vs logic, uses a semaphore (as a mutex) to protect game data.
The code runs until one of the heros (Timmy/Harold) reaches the other side of the Vale, or if both of them die. 
The heros die if an enemy occupies the same square as them.
The game then tells you the result of the simulation.

- Walk: [Enemy: takes a diagonal only step towards the nearist player] [Player: take a single step towards the ending line]
- Jump: [Enemy: takes multiple left or up (up to 7) steps towards player] [Player: take multiple steps (up to 7) towards the ending line]
- Slide: [Anyone: Move back (left) up to 4 steps.]
- Sleep: [Anyone: Do nothing.]
- Teleport: [Anyone: Teleports to a random location in the game]


#### Behaviour Probabilities
  - ##### Timmy 
    + Walk 						            70%
    + Jump 						            25% 
    + Slide 						           5%
  - ##### Harold 
    + Walk 						            30%
    + Jump 					    	        30%
    + Slide 				  		        10%
    + Sleep 				          		30%
  - ##### Dorcs 
    + Walk 				            		95%
    + Jump 						             5%
  - ##### Ninjas 
    + Jump 						            60%
    + Teleport 					          40%



