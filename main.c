#include <stdio.h>
#include <string.h>
#include <termios.h>

#include "term_change.c"
#include "reversi_base.h"


int main(int argc, char* argv[]){
  int vs_mode = ((argc == 1) ? 0 : 1);
  struct termios terms[2];

  GetDefaultTerms(&terms[0]);
  TermModeChange(&terms[0], 1);

  ReversiMain(0, vs_mode);

  TermModeChange(&terms[0], 0);

  return 0;
}
