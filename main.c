#include <stdio.h>
#include <string.h>
#include <termios.h>

#include "term_change.c"
#include "reversi_base.h"


int main(void){
  struct termios terms[2];

  GetDefaultTerms(&terms[0]);
  TermModeChange(&terms[0], 1);

  ReversiMain(0, 0);

  TermModeChange(&terms[0], 0);

  return 0;
}
