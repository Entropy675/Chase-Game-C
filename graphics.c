#include "defs.h"

/*
  Function:  initNcurses
   Purpose:  initializes ncurses library
*/
void initNcurses()
{
  initscr();
  curs_set(0);
  clear();
}

/*
  Function:  cleanupNcurses
   Purpose:  cleans up ncurses library
        in:  row where "Press any key..."
             string will be printed out
*/
void cleanupNcurses(int row)
{
  move(row, 0);
  addstr("Press any key to continue...");
  getch();
  endwin();
}

/*
  Function:  scrPrt
   Purpose:  prints out a string to a
             given row and col on the screen
        in:  string to be printed out
        in:  row where the string is printed out
        in:  column where the string is printed out
*/
void scrPrt(char *str, int row, int col)
{
  move(row, col);
  addstr(str);
  refresh();
}

