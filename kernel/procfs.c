
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
#define MEMINFO_PROC_FILE_SIZE 200
  char buf[MEMINFO_PROC_FILE_SIZE];
  uint r;  // Read index
} meminfoproc;


int
meminforead(int user_dst, uint64 dst, int n)
{
  uint target;
  char cbuf;
  int c;
  int freepages;
  freepages = getfreepages();
  sprintf(meminfoproc.buf, "FreePages: %d\nMemFree: %d kB\n", freepages, freepages * PGSIZE / 1024);

  target = n;
  while(n > 0){
    if (meminfoproc.r == (strlen(meminfoproc.buf))){
      meminfoproc.r = 0;
      break;
    };
    c = meminfoproc.buf[meminfoproc.r];
    meminfoproc.r = meminfoproc.r + 1;
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
meminfowrite(int user_src, uint64 src, int n)
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
  devsw[MEMINFO].read = meminforead;
  devsw[MEMINFO].write = meminfowrite;
}
