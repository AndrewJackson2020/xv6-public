

int
helloread(int user_dst, uint64 dst, int n)
{
  char cbuf;

  // copy the input byte to the user-space buffer.
  cbuf = '4';
  if(either_copyout(user_dst, dst, &cbuf, 1) == -1)
    break;

  return 0
}

int
hellowrite(int user_src, uint64 src, int n)
{
  return 0;
}

void
procfsinit(void)
{
  devsw[HELLO].read = helloread;
  devsw[HELLO].write = hellowrite;
}
