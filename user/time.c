
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int start_uptime;
  int end_uptime;
  int ticks;
  int total_time;

  start_uptime = uptime();
  int pid = fork();
  if (pid == 0){
    exec(argv[1], &argv[1]);
    exit(0);
  } else {
    wait(0);
  }

  end_uptime = uptime();
  ticks = end_uptime - start_uptime;
  total_time = ticks * 100 / 10;
  printf("%d.%d seconds total\n", total_time / 100 , total_time % 100);
  exit(0);
}

