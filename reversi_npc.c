#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "reversi_base.h"


int CpuSelectPos(boarddata* board, int turn, int level){
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

  if(!level){
    return placeables[rand() % p_i];
  }

  boarddata temp = *board;

  int candidates[32], count, edge_flag;
  int candidates_len = 0;
  int min = 255;

  for(int i = 0; i < p_i; i++){
    SetDisc(&temp, placeables[i], turn);
    count = SetReverseCounts(&temp, !turn);

    edge_flag = (
      placeables[i] == 0 ||
      placeables[i] == temp.sizes[0] - 1 ||
      placeables[i] == temp.sizes[0] * (temp.sizes[1] - 1) ||
      placeables[i] == temp.sizes[2] - 1
    );

    if(edge_flag && min != -1){
      candidates_len = 1;
      candidates[0] = placeables[i];
      min = -1;
    }

    if(count < min){
      candidates_len = 0;
      min = count;
    }

    else if(count > min && !edge_flag){
      continue;
    }

    candidates[candidates_len] = placeables[i];
    candidates_len++;

    temp = *board;
  }

  return candidates[rand() % candidates_len];
}
