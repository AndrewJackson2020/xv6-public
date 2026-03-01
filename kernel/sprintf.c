#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#include <stdarg.h>

static char digits[] = "0123456789ABCDEF";

char *
sprintint(char *p, long long xx, int base, int sgn)
{
  char buf[20];
  int i, neg;
  unsigned long long x;

  neg = 0;
  if(sgn && xx < 0){
    neg = 1;
    x = -xx;
  } else {
    x = xx;
  }

  i = 0;
  do{
    buf[i++] = digits[x % base];
  }while((x /= base) != 0);
  if(neg)
    buf[i++] = '-';

  while(--i >= 0)
    *p++ = buf[i];

  return p;
}

char *
sprintptr(char *p, uint64 x) {
  int i;
  *p++ = '0';
  *p++ = 'x';
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
    *p++ = digits[x >> (sizeof(uint64) * 8 - 4)];

  return p;
}

// Only understands %d, %x, %p, %c, %s.
void
sprintf(char *buf, const char *fmt, ...)
{
  char *s;
  int c0, c1, c2, i, state;
  va_list ap;
  char *p;
  p = buf;
  va_start(ap, fmt);

  state = 0;
  for(i = 0; fmt[i]; i++){
    c0 = fmt[i] & 0xff;
    if(state == 0){
      if(c0 == '%'){
        state = '%';
      } else {
        *p++ = c0;
      }
    } else if(state == '%'){
      c1 = c2 = 0;
      if(c0) c1 = fmt[i+1] & 0xff;
      if(c1) c2 = fmt[i+2] & 0xff;
      if(c0 == 'd'){
        p = sprintint(p, va_arg(ap, int), 10, 1);
      } else if(c0 == 'l' && c1 == 'd'){
        p = sprintint(p, va_arg(ap, uint64), 10, 1);
        i += 1;
      } else if(c0 == 'l' && c1 == 'l' && c2 == 'd'){
        p = sprintint(p, va_arg(ap, uint64), 10, 1);
        i += 2;
      } else if(c0 == 'u'){
        p = sprintint(p, va_arg(ap, uint32), 10, 0);
      } else if(c0 == 'l' && c1 == 'u'){
        p = sprintint(p, va_arg(ap, uint64), 10, 0);
        i += 1;
      } else if(c0 == 'l' && c1 == 'l' && c2 == 'u'){
        p = sprintint(p, va_arg(ap, uint64), 10, 0);
        i += 2;
      } else if(c0 == 'x'){
        p = sprintint(p, va_arg(ap, uint32), 16, 0);
      } else if(c0 == 'l' && c1 == 'x'){
        p = sprintint(p, va_arg(ap, uint64), 16, 0);
        i += 1;
      } else if(c0 == 'l' && c1 == 'l' && c2 == 'x'){
        p = sprintint(p, va_arg(ap, uint64), 16, 0);
        i += 2;
      } else if(c0 == 'p'){
        p = sprintptr(p, va_arg(ap, uint64));
      } else if(c0 == 'c'){
        *p++ = va_arg(ap, uint32);
      } else if(c0 == 's'){
        if((s = va_arg(ap, char*)) == 0)
          s = "(null)";
        for(; *s; s++)
          *p++ = *s;
      } else if(c0 == '%'){
        *p++ = '%';
      } else {
        // Unknown % sequence.  Print it to draw attention.
        *p++ = '%';
        *p++ = c0;
      }

      state = 0;
    }
  }
}

