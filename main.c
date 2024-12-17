#include <stdio.h>
#include <string.h>
#include <termios.h>

#include "term_mode_edit.h"
#include "check_input.h"
#include "title.h"
#include "reversi_base.h"


int main(){
  int loop_flag;
  reversi_mode r_mode;

  FormatReversiMode(&r_mode);
  GetDefaultFcntlFlag();

  GetDefaultTerms();
  ChangeTermMode(SET_TERM_NONCANON);
  printf("\e[?25l");

  while(loop_flag){
    loop_flag = ShowTitle(&r_mode);

    if(loop_flag){
      loop_flag = ReversiMain(&r_mode);
    }
  }

  ChangeTermMode(SET_TERM_DEFAULT);
  printf("\e[?25h");

  return 0;
}
