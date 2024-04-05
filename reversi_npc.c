#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "reversi_base.h"


int CpuSelectPos(boarddata* board, int turn, int mode){
  usleep(100000);
  int placeables[256];
  int p_i = 0;

  for(int i = 0; i < board->sizes[2]; i++){
    if(board->reverse_counts[i]){
      placeables[p_i] = i;
      p_i++;
    }
  }

  srand((unsigned)time(NULL));
  return placeables[rand() % p_i];
}
