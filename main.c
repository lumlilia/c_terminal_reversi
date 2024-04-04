#include <stdio.h>
#include <string.h>
#include <termios.h>
#include "reversi_base.h"


int main(void){
  setvbuf(stdout, NULL, _IONBF, 0);
  struct termios term0, term1;

  tcgetattr(0, &term1);
  term0 = term1;
  term1.c_lflag &= ~(ICANON | ECHO);
  term1.c_cc[VMIN] = 1;
  term1.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &term1);


  ReversiMain(0, 0);

  tcsetattr(0, TCSANOW, &term0);
  return 0;
}


