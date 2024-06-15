/* Last update 2024/06/12 */

#include <stdio.h>
#include <termios.h>
#include "term_mode_edit.h"


static struct termios default_term, term;


int GetDefaultTerms(){
  if(!default_term.c_lflag){
    tcgetattr(0, &default_term);
  }

  term = default_term;
}


void ChangeTermMode(int mode){
  switch(mode){
    case 0:
      tcsetattr(0, TCSANOW, &default_term);
      break;

    case 1:
      term.c_lflag &= ~(ICANON | ECHO);
      term.c_cc[VMIN] = 1;
      term.c_cc[VTIME] = 0;
      tcsetattr(0, TCSANOW, &term);
      break;
  }
}
