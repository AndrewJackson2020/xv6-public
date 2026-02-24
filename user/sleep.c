#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int sleep_time;
  int start_uptime;
  int current_uptime;

  if(argc <= 1){
    fprintf(2, "usage: sleep [time_seconds]\n");
    exit(1);
  }
  sleep_time = atoi(argv[1]);

  start_uptime = uptime();
  for (;;){
    current_uptime = uptime();
    if ((current_uptime - start_uptime) / 10 > sleep_time){
      break;
    }
  }

  printf("\n");
  exit(0);
}
