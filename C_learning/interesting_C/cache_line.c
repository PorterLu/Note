#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

unsigned long long get_cpu_cycle() {
  unsigned long long reth1;
  unsigned long long retl0;
  __asm__ __volatile__(
    "rdtsc" :
    "=d" (reth1),
    "=a" (retl0)
  );
  return ((reth1 << 32) | (retl0));
}

int main() {
  int a[2049] = {0};
  int b[2049] = {0};
  unsigned long long t1 = get_cpu_cycle();
  for (int i = 0; i < 2048; i++) a[i] = b[i];
  unsigned long long t2 = get_cpu_cycle();
  a[2048] = b[2048];
  unsigned long long t3 = get_cpu_cycle();
  printf("t2-t1=%ld\n", t2-t1);
  printf("t3-t2=%ld\n", t3-t2);
}
