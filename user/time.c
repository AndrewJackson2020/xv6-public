
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int start_uptime;
  int end_uptime;
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
  total_time = (end_uptime - start_uptime) / 10;
  printf("%d total\n", total_time);
  exit(0);
}

