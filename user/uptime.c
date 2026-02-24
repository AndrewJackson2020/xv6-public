
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int up;
  up = uptime();
  printf("up %d \n", up);
  exit(0);
}
