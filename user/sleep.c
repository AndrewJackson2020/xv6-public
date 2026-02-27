#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int sleep_time;

  if(argc <= 1){
    fprintf(2, "usage: sleep [time_seconds]\n");
    exit(1);
  }
  sleep_time = atoi(argv[1]);

  pause(sleep_time * 10);

  printf("\n");
  exit(0);
}
