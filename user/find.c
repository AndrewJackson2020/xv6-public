
#include <stddef.h>
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int find(char *path, char *exec_command, char *exec_args[]){
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return 1;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return 1;
  }

  if (exec_command != NULL){
    int pid = fork();
    if (pid == 0){
      exec(exec_command, exec_args);
      exit(0);
    } else {
      wait(0);
    }
  } else {
    printf("%s\n", path);
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    if (*(p-1) != '/')
      *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if ((strcmp(de.name, ".") == 0) && (strlen(de.name) == 1))
        continue;
      if ((strcmp(de.name, "..") == 0) && (strlen(de.name) == 2))
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      find(buf, exec_command, exec_args);
    }
    break;
  }
  close(fd);
  return 0;
}

int
main(int argc, char *argv[])
{
  char usage_message[] = "usage: find [path] -exec [command] \n";
  if(argc <= 1){
    fprintf(2, usage_message);
    exit(1);
  }

  // Is exec passed?
  if(argc >= 3){
    if ((strcmp(argv[2], "-exec") != 0) || (strlen(argv[2]) != strlen("-exec"))){
      fprintf(2, usage_message);
      exit(1);
    }

    if (find(argv[1], argv[3], &argv[4]) != 0){
      exit(1);
    }

    exit(0);
  }

  if (find(argv[1], NULL, NULL) != 0){
    exit(1);
  }
  exit(0);
}
