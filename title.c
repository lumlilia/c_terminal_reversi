#include <stdio.h>
#include <string.h>

#include "term_mode_edit.h"
#include "check_input.h"
#include "title.h"
#include "title_image.c"


static const char* turn_labels[3] = {
  "PLAYER",
  "CPU",
  "RANDOM"
};

static void DrawLabel(reversi_mode*, int, int, char*);
static void SetConfigLabel(int, int, char*);


int ShowTitle(reversi_mode* mode){
  int cursor, input_key, loop_flag;
  char labels[5][31] = {
    "1 PLAYER",
    "2 PLAYERS",
    "CPU ONLY",
    "",
    "CONFIG"
  };

  printf("\e[2J\e[H");
  printf("\e(0%s\e(B\n\n", TITLE_IMAGE);

  cursor = 0;
  loop_flag = 1;

  while(loop_flag){
    DrawLabel(mode, cursor, 5, (char *)&labels[0]);
    input_key = GetInput();

    switch(input_key){
      case 13:
      case ' ':
        if(cursor == 3){
          ShowConfig(mode);
          break;
        }
        mode->player = cursor;
      case 27:
        loop_flag = 0;
        break;

      case 'w':
      case INPUT_ARROW_UP:
        if(cursor > 0){
          cursor--;
        }
        else{
          cursor = 3;
        }
        break;

      case 's':
      case INPUT_ARROW_DOWN:
        if(cursor < 3){
          cursor++;
        }
        else{
          cursor = 0;
        }
        break;
    }
  }

  return input_key != 27;
}


void DrawLabel(reversi_mode* mode, int cursor, int l_count, char* label){
  printf("\e[%d;1H\e[J", TITLE_IMAGE_SIZES[1] + 1);
  for(int i = 0; i < l_count; i++){
    if(!strlen(label) && i <= cursor){
      cursor++;
    }

    printf("\e[%dC%s%s\e[27m\n",
      15 - (int)(strlen(label) / 2),
      ((i == cursor) ? "\e[7m" : ""),
      label
    );

    label += 31;
  }
}


int ShowConfig(reversi_mode* mode){
  int n, cursor, input_key, loop_flag;
  reversi_mode temp = *mode;

  char labels[7][31] = {
    "CPU LEVEL",
    "CPU1  ",
    "CPU2  ",
    "",
    "FIRST        ",
    "",
    "OK"
  };

  for(int i = 0; i < 2; i++){
    SetConfigLabel(i, temp.levels[i], &labels[i + 1][6]);
  }

  SetConfigLabel(2, temp.turn, &labels[4][7]);

  loop_flag = 1;
  cursor = 1;

  while(loop_flag){
    DrawLabel(mode, cursor, 7, (char *)&labels[0]);
    input_key = GetInput();

    switch(input_key){
      case 13:
      case ' ':
        if(cursor <= 2){
          n = cursor - 1;
          temp.levels[n] = !temp.levels[n];
          SetConfigLabel(n, temp.levels[n], &labels[n + 1][6]);

          break;
        }

        else if(cursor == 3){
          temp.turn += ((temp.turn < 2) ? 1 : -2);
          SetConfigLabel(2, temp.turn, &labels[4][7]);
          break;
        }

        *mode = temp;

      case 27:
        loop_flag = 0;
        break;

      case 'w':
      case INPUT_ARROW_UP:
        if(cursor > 1){
          cursor--;
        }
        else{
          cursor = 4;
        }
        break;

      case 's':
      case INPUT_ARROW_DOWN:
        if(cursor < 4){
          cursor++;
        }
        else{
          cursor = 1;
        }
        break;
    }
  }
}


void SetConfigLabel(int item_num, int data_num, char* label){
  switch(item_num){
    case 0:
    case 1:
      sprintf(label, "%d", data_num);
      break;

    case 2:
      sprintf(label, "%6s", turn_labels[data_num]);
      break;
  }
}