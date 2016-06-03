#include "types.h"
#include "stat.h"
#include "user.h"

int main() {
  int a, *p;
  a = 1;
  p = &a;
  printf(1, "%d\n", a);
  p = 0;
  a = *p;
  printf(1,"should not print");
  return 0;
}
