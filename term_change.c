#include <stdio.h>
#include <termios.h>


int GetDefaultTerms(struct termios*);
void TermModeChange(struct termios*, int);


int GetDefaultTerms(struct termios* terms){
  if(terms[0].c_lflag){
    return -1;
  }

  tcgetattr(0, &terms[0]);
  terms[1] = terms[0];
}


void TermModeChange(struct termios* terms, int mode){
  if(mode){
    setvbuf(stdout, NULL, _IONBF, 0);

    terms[1].c_lflag &= ~(ICANON | ECHO);
    terms[1].c_cc[VMIN] = 1;
    terms[1].c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &terms[1]);
  }

  else{
    tcsetattr(0, TCSANOW, &terms[0]);
  }
}
