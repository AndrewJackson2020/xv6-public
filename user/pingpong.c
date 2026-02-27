
#include <stddef.h>
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pipe_child_to_parent[2];
  int pipe_parent_to_child[2];

  int max_iter = -1;
  int current_iter = 0;

  if (argc == 2){
    max_iter = atoi(argv[1]);
  }

  if (pipe(pipe_child_to_parent) > 0){
    printf("pingpong: pipe failed\n");
    exit(1);
  }

  if (pipe(pipe_parent_to_child) > 0){
    printf("pingpong: pipe failed\n");
    exit(1);
  }

  int pid = fork();
  if(pid == 0){
    // child codepath
    for (;;){
      char ping_from_parent[4];

      gets2(ping_from_parent, sizeof(ping_from_parent)+1, pipe_parent_to_child[0]);
      printf("%s", ping_from_parent);
      if(write(pipe_child_to_parent[1], "pong", sizeof("pong")) != sizeof("pong")){
        printf("pingpong: pipe write to parent failed\n");
        exit(1);
      }
    }
  } else if(pid > 0){
    // parent codepath
    for (;;){
      char pong_from_child[4];
      if(write(pipe_parent_to_child[1], "ping", sizeof("ping")) != sizeof("ping")){
        printf("pingpong: pipe write to child failed\n");
        exit(1);
      }
      gets2(pong_from_child, sizeof(pong_from_child) + 1, pipe_child_to_parent[0]);
      printf("%s\n", pong_from_child);
      current_iter++;
      if (max_iter > -1){
          if (current_iter == max_iter){
            exit(0);
          }
      }
    }
  }
}
