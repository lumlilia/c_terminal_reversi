#pragma once

typedef struct{
  char places[256];
  long long reverse_counts[256];
  int counts[2];
  int sizes[3];
  int colors[2];
} boarddata;

#define REVERSI_DISCS ((char*[]){ \
  "　", \
  "⚪️", \
  "⚫️", \
\
  "　", \
  "ｏ", \
  "ｘ" \
})

void ResetBoard(boarddata*, int);
int CheckMove(boarddata*, int, int);
void CheckRev(boarddata*, int, int, int, int*);
long long Search(boarddata*, int, int);
int SetReverseCounts(boarddata*, int);
void SetDisc(boarddata*, int, int);
void DrawBoard(boarddata*, int, int, int);
int ChangeTurn(boarddata*, int);
void ReversiMain(int, int);
