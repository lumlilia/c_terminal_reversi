#pragma once

typedef struct{
  char places[256];
  long long reverse_counts[256];
  int counts[2];
  int sizes[3];
  int colors[2];
} boarddata;

typedef struct{
  int player;
  int levels[2];
  int turn;
  char name1[25];
  char name2[25];
} reversi_mode;


#define REVERSI_DISCS ((char*[]){ \
  "　", \
  "⚪️", \
  "⚫️", \
\
  "　", \
  "Ｏ", \
  "Ｘ" \
})

int CpuSelectPos(boarddata*, int, int);

void FormatReversiMode(reversi_mode*);

void ResetBoard(boarddata*, int);
int CheckMove(boarddata*, int, int);
void CheckRev(boarddata*, int, int, int, int*);
long long Search(boarddata*, int, int);
int SetReverseCounts(boarddata*, int);
void SetDisc(boarddata*, int, int);
void DrawBoard(boarddata*, int, int, int);
int ChangeTurn(boarddata*, int);
int ReversiMain(int, reversi_mode*);
