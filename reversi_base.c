#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "reversi_base.h"
#include "check_input.h"


void FormatReversiMode(reversi_mode* mode){
  mode->player = 1;
  mode->levels[0] = 1;
  mode->levels[1] = 1;
  mode->turn = 0;
  mode->disc_type = 0;
  memset(&mode->name1[0], 0, sizeof(mode->name1));
  memset(&mode->name2[0], 0, sizeof(mode->name2));
  strcpy(&mode->name1[0], "nanasi 1");
  strcpy(&mode->name2[0], "nanasi 2");
}


void ResetBoard(boarddata* board, int flag){
  memset(board->places, 0, 256);
  memset(board->reverse_counts, 0, 2048);

  if(flag){
    int b_center[2] = {
      board->sizes[0] / 2 - 1,
      board->sizes[1] / 2 - 1
    };
    char* p = board->places + b_center[0] + board->sizes[0] * b_center[1];

    for(int i = 0; i < 2; i++){
      *p = 1 + !i;
      p++;
      *p = 1 + i;
      p += board->sizes[0] - 1;
    }

    board->counts[0] = 2;
    board->counts[1] = 2;
  }
}


int CheckMove(boarddata* board, int pos, int v){
  int flag = 0;
  int x = pos % board->sizes[0];
  int y = pos / board->sizes[0];

  if((v >= 1 && v <= 3) && x < (board->sizes[0] - 1)){
    x++;
  }
  else if((v >= 5 && v <= 7) && x > 0){
    x--;
  }
  else{
    flag++;
  }

  if((v >= 3 && v <= 5) && y < (board->sizes[1] - 1)){
    y++;
  }
  else if((v == 0 || v == 1 || v == 7) && y > 0){
    y--;
  }
  else{
    flag++;
  }

  if((v % 2 && flag) || (!(v % 2) && flag == 2)){
    return -1;
  }

  pos = x + y * board->sizes[0];
  return pos;
}


void CheckRev(boarddata* board, int pos, int turn, int v, int* count){
  pos = CheckMove(board, pos, v);

  if(pos > -1){
    if(board->places[pos] == (1 + !turn)){
      *count += 1;
      CheckRev(board, pos, turn, v, count);
    }
    else if(board->places[pos] == 0){
      *count = 0;
    }
  }

  else{
    *count = 0;
  }
}


long long Search(boarddata* board, int pos, int turn){
  int count;
  int flag = 0;
  long long ret = 0;
  char* p = (char *)&ret;

  for(int i = 0; i < 8; i++){
    count = 0;

    CheckRev(board, pos, turn, i, &count);

    *p = count;
    p++;

    if(!count){
      continue;
    }
  }

  return ret;
}


int SetReverseCounts(boarddata* board, int turn){
  memset(board->reverse_counts, 0, board->sizes[2] * 8);
  int ret = 0;

  for(int i = 0; i < board->sizes[2]; i++){
    if(!board->places[i]){
      board->reverse_counts[i] = Search(board, i, turn);
    }

    if(board->reverse_counts[i]){
      ret++;
    }
  }

  return ret;
}


void SetDisc(boarddata* board, int pos, int turn){
  board->places[pos] = turn + 1;
  board->counts[turn]++;

  char* p = (char *)&board->reverse_counts[pos];
  int m;

  for(int i = 0; i < 8; i++){
    if(*p){
      m = pos;

      for(int j = 0; j < *p; j++){
        m = CheckMove(board, m, i);
        if(m > -1){
          board->places[m] = 1 + turn;
          board->counts[!turn]--;
          board->counts[turn]++;
        }
        else{
          break;
        }
      }
    }

    p++;
  }
}


void DrawBoard(boarddata* board, int disc_type, int pos, int turn){
  int si[3];
  si[0] = 0;
  printf("\e[1;1H\e[2J\e[1;38;2;0;0;0m");

  int c = 0;

  for(int i = 0; i < board->sizes[2]; i++){
    if(i % board->sizes[0]){
      c = !c;
    }

    int col_n;
    if(i == pos){
      col_n = 103;
    }
    else if(board->reverse_counts[i]){
      col_n = 43;
    }
    else{
      col_n = board->colors[c];
    }

    printf("\e[%dm%s",
      col_n,
      REVERSI_DISCS[disc_type + board->places[i]]
    );

    if(i % board->sizes[1] == board->sizes[1] - 1){
      printf("\e[1B\e[1G");
    }
  }

  printf("\e[1B\e[1G\e[0m\e[1m");

  if(turn < 0){
    printf("勝負あり！！");
  }
  else{
    printf("%s\e[0mのターン", REVERSI_DISCS[disc_type + turn + 1]);
  }

  printf("\n\n\e[1m%s\e[0m: %d  /  \e[1m%s\e[0m: %d\n",
    REVERSI_DISCS[disc_type + 1],
    board->counts[0],
    REVERSI_DISCS[disc_type + 2],
    board->counts[1]
  );

  int x = 1 + pos % board->sizes[0];
  int y = 1 + pos / board->sizes[1];
  printf("\e[0m\e[%d;%dH", x, y);
}


int ChangeTurn(boarddata* board, int turn){
  int c = 0;

  for(int i = 0; i < 2; i++){
    turn = !turn;
    c = SetReverseCounts(board, turn);
    if(c){
      break;
    }
  }

  if(c){
    return turn;
  }

  return -1;
}


int ReversiMain(reversi_mode* mode){
  int disc_type = mode->disc_type * 3;

  boarddata board;
  board.sizes[0] = 8;
  board.sizes[1] = 8;
  board.sizes[2] = board.sizes[0] * board.sizes[1];
  board.colors[0] = 42;
  board.colors[1] = 102;

  ResetBoard(&board, 1);

  int input_key, move, player_turn;
  int loop_flag = 1;
  int pos = 0;

  int turn = 0;

  SetReverseCounts(&board, 0);

  if(!mode->player){
    if(mode->turn == 2){
      srand((unsigned)time(NULL));
      player_turn = rand() % 2;
    }
    else{
      player_turn = mode->turn;
    }

    printf("\e[2J\e[8;Hあなたは \e[1m%s\e[22m です\n", REVERSI_DISCS[disc_type + (player_turn + 1)]);

    getchar();
  }
  
  printf("\e[2J");

  DrawBoard(&board, disc_type, 0, 0);

  while(loop_flag){
    if(
      (!mode->player && turn == !player_turn) ||
      mode->player == 2
    ){
      pos = CpuSelectPos(&board, turn, mode->levels[(!mode->player ? 0 : turn)]);
      input_key = 32;
    }
    else{
      input_key = GetInput();
    }
    move = 0;

    switch(input_key){
      case 27:
      case 'Q':
        loop_flag = 0;
        break;

      case 'a':
      case INPUT_ARROW_LEFT:
        move--;
        break;

      case 'd':
      case INPUT_ARROW_RIGHT:
        move++;
        break;

      case 'w':
      case INPUT_ARROW_UP:
        move -= board.sizes[0];
        break;

      case 's':
      case INPUT_ARROW_DOWN:
        move += board.sizes[0];
        break;

      case 10:
      case 13:
      case ' ':
        if(board.reverse_counts[pos]){
          SetDisc(&board, pos, turn);

          turn = ChangeTurn(&board, turn);

          if(turn < 0){
            loop_flag = 0;
          }

          DrawBoard(&board, disc_type, pos, turn);
        }

        break;
    }

    if(move){
      int mv = pos + move;
      if(mv >= 0 && mv < board.sizes[2]){
        pos = mv;

        DrawBoard(&board, disc_type, pos, turn);
      }
    }

    EmptyInputHistory();
  }

  printf("\e[0m\e[%d;1H", board.sizes[1] + 6);

  if(board.counts[0] == board.counts[1]){
    puts("引き分け…");
  }

  else{
    printf("\e[1m%sの勝ち！！\n\e[0m", REVERSI_DISCS[disc_type + 1 + (board.counts[0] < board.counts[1])]);
  }

  if(input_key != 27){
    getchar();
    return 1;
  }

  return 0;
}
