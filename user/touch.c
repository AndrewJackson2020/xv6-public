
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    fprintf(2, "Usage: touch files...\n");
    exit(1);
  }

  for(i = 1; i < argc; i++){
    if(open(argv[i], O_CREATE|O_RDWR) < 0){
      fprintf(2, "touch: %s failed to create\n", argv[i]);
      break;
    }
  }

  exit(0);
}
