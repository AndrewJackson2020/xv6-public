
#include <stdarg.h>

#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"


struct {
#define HELLO_PROC_FILE_SIZE 12
  char buf[HELLO_PROC_FILE_SIZE];
  uint r;  // Read index
} helloproc;


int
helloread(int user_dst, uint64 dst, int n)
{
  uint target;
  char cbuf;
  int c;

  memmove(helloproc.buf, "hello world\n", HELLO_PROC_FILE_SIZE);

  target = n;
  while(n > 0){
    if (helloproc.r == HELLO_PROC_FILE_SIZE){
      helloproc.r = 0;
      break;
    };
    c = helloproc.buf[helloproc.r];
    helloproc.r = helloproc.r + 1;
    cbuf = c;
    if(either_copyout(user_dst, dst, &cbuf, 1) == -1)
      break;

    target++;
    dst++;
    --n;

    if(c == '\n'){
      // a whole line has arrived, return to
      // the user-level read().
      break;
    }
  }

  return target - n;
}

int
hellowrite(int user_src, uint64 src, int n)
{
  // char buf[32]; // move batches from user space to uart.
  // int i = 0;

  // while(i < n){
  //   int nn = sizeof(buf);
  //   if(nn > n - i)
  //     nn = n - i;
  //   if(either_copyin(buf, user_src, src+i, nn) == -1)
  //     break;
  //   uartwrite(buf, nn);
  //   i += nn;
  // }
  return 0;
}

void
procfsinit(void)
{
  devsw[HELLO].read = helloread;
  devsw[HELLO].write = hellowrite;
}
