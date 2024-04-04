#include <stdio.h>
#include <string.h>
#include <termios.h>


typedef struct{
  char places[256];
  long long reverse_counts[256];
  int counts[2];
  int sizes[3];
  int colors[2];
} boarddata;


void ResetBoard(boarddata*, int);
int CheckMove(boarddata*, int, int);
void CheckRev(boarddata*, int, int, int, int*);
long long Search(boarddata*, int, int);
int SetReverseCounts(boarddata*, int);
void SetStone(boarddata*, int, int);
void DrawBoard(boarddata*, char*, int, int);
int ChangeTurn(boarddata*, int);


int main(void){
  setvbuf(stdout, NULL, _IONBF, 0);
  struct termios term0, term1;

  tcgetattr(0, &term1);
  term0 = term1;
  term1.c_lflag &= ~(ICANON | ECHO);
  term1.c_cc[VMIN] = 1;
  term1.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &term1);


  boarddata board;
  board.sizes[0] = 8;
  board.sizes[1] = 8;
  board.sizes[2] = board.sizes[0] * board.sizes[1];
  board.colors[0] = 42;
  board.colors[1] = 102;

  ResetBoard(&board, 1);

  char* stone[3] = {
    "　",
    "⚪️",
    "⚫️"
  };

  char ch;
  int loop_flag = 1;
  int pos = 0;
  int move;

  int turn = 0;

  SetReverseCounts(&board, 0);

  printf("\e[?25l\e[2J");
  DrawBoard(&board, stone[0], 0, 0);

  while(loop_flag){
    ch = getchar();
    move = 0;

    switch(ch){
      case 27:
        loop_flag = 0;
        break;

      case 'a':
        move--;
        break;

      case 'd':
        move++;
        break;

      case 'w':
        move -= board.sizes[0];
        break;

      case 's':
        move += board.sizes[0];
        break;

      case 32:
        if(board.reverse_counts[pos]){
          SetStone(&board, pos, turn);

          turn = ChangeTurn(&board, turn);

          if(turn < 0){
            loop_flag = 0;
          }

          DrawBoard(&board, stone[0], pos, turn);
        }

        break;
    }

    if(move){
      int mv = pos + move;
      if(mv >= 0 && mv < board.sizes[2]){
        pos = mv;

        DrawBoard(&board, stone[0], pos, turn);
      }
    }
  }

  printf("\e[0m\e[%d;1H\e[?25h", board.sizes[1] + 6);

  if(board.counts[0] == board.counts[1]){
    puts("引き分け…");
  }

  else{
    printf("%sの勝ち！！\n", stone[1 + (board.counts[0] < board.counts[1])]);
  }

  tcsetattr(0, TCSANOW, &term0);
  return 0;
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


void SetStone(boarddata* board, int pos, int turn){
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


void DrawBoard(boarddata* board, char* stone, int pos, int turn){
  int si[3];
  si[0] = 0;
  si[1] = strlen(&stone[0]) + 1;
  si[2] = si[1] + strlen(&stone[si[1]]) + 1;
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

    printf("\e[%dm%s\e[49m",
      col_n,
      &stone[si[board->places[i]]]
    );

    if(i % board->sizes[1] == board->sizes[1] - 1){
      printf("\e[%d;1H", (int)(i / board->sizes[0]) + 2);
    }
  }

  printf("\e[1B\e[1G\e[1m\e[0m");

  if(turn < 0){
    printf("勝負あり！！");
  }
  else{
    printf("%sのターン", &stone[si[turn + 1]]);
  }

  printf("\n\n%s: %d  /  %s: %d\n",
    &stone[si[1]],
    board->counts[0],
    &stone[si[2]],
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
