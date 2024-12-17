/* Last update 2024/06/18 */

#include <stdio.h>
#include <fcntl.h>


static int default_flag;

void GetDefaultFcntlFlag(){
  default_flag = fcntl(0, F_GETFL);
}


void EmptyInputHistory(){
  fcntl(0, F_SETFL, O_NONBLOCK);
  while(getchar() > -1){};
  fcntl(0, F_SETFL, default_flag);
}


int GetInput(){
  int input_key = 0;
  char* p = (char *)&input_key;

  *p = getchar();
  fcntl(0, F_SETFL, O_NONBLOCK);

  for(int i = 0; i < 4; i++){
    p++;
    *p = getchar();
    if(*p == -1){
      *p = 0;
      break;
    }
  }

  fcntl(0, F_SETFL, default_flag);

  return input_key;
}
