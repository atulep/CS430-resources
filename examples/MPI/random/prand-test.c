#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <prand.h>

#define NUM 1000
#define SEED 3456
#define  TEST64BSTRIDE

static void testrandom(long* A, long n, long seed, int statesize)
{
  int i;
  char rstate[256];
  long v;

  if ( (A == 0) || (statesize < 0) || (statesize > 256) || (n < 2) ) {
    fprintf(stderr, "Incorrect use of testrandom() routine\n");
    exit(1);
  }

  printf("checking %ld numbers with state %3d ", n, statesize);  fflush(stdout);
  if (statesize == 0)  srandom(seed);
  else                 initstate(seed, rstate, statesize);
  for (i = 0; i < n; i++) {
     A[i] = random();
  }
  printf("...");  fflush(stdout);
  for (i = 0; i < (n-1); i++) {
    if (statesize == 0)  srandom(seed);
    else                 initstate(seed, rstate, statesize);
    unrankRand(i);
    if (random() != A[i])  break;
    if (random() != A[i+1])  break;
  }
  printf(" %s ", (i == (n-1)) ? "success" : "FAILURE");  fflush(stdout);

#ifdef TEST64BSTRIDE
  // Test with >32-bit strides
  if (statesize == 0)  srandom(seed);
  else                 initstate(seed, rstate, statesize);
  unrankRand(12000000000LL);
  v = random();
  if (statesize == 0)  srandom(seed);
  else                 initstate(seed, rstate, statesize);
  for (i = 0; i < 12; i++)
    unrankRand(1000000000);
  if (v != random())
    printf("FAIL on >32-bit stride"); fflush(stdout);
#endif

  printf("\n");
}

int main(int argc, char **argv) {
  long int A[NUM];

  printf("bits for char/short/int/long/longlong/* = %d/%d/%d/%d/%d/%d\n",
    8*(int)sizeof(char), 8*(int)sizeof(short), 8*(int)sizeof(int),
    8*(int)sizeof(long), 8*(int)sizeof(long long), 8*(int)sizeof(long*));
  printf("\n");

  testrandom(A, NUM, SEED,   0);
  testrandom(A, NUM, SEED,   8);
  testrandom(A, NUM, SEED,  32);
  testrandom(A, NUM, SEED,  64);
  testrandom(A, NUM, SEED, 128);
  testrandom(A, NUM, SEED, 256);

  return 0;
}
