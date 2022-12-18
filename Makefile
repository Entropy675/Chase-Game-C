OBJ = main.o graphics.o runner.o behaviour.o chase.o
CC = gcc -Wall 
LIB = -lpthread -lncurses

a5:	$(OBJ)
	$(CC) -o a5 $(OBJ) $(LIB)

main.o:	main.c chase.c defs.h
	$(CC) -c main.c

graphics.o:	graphics.c defs.h
	$(CC) -c graphics.c

runner.o: runner.c behaviour.c defs.h 
	$(CC) -c runner.c

behaviour.o: behaviour.c runner.c main.c defs.h 
	$(CC) -c behaviour.c

chase.o: chase.c runner.c main.c graphics.c defs.h 
	$(CC) -c chase.c
	

clean:
	rm -f chasegame $(OBJ) 
