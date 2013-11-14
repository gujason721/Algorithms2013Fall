#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int length = 999999;


int main(int argc, char** argv)
{
  int Array[length];
  if (argc > 1) {
    if (atoi(argv[1]) < length) {
      length = atoi(argv[1]);
    }
  }
  int i;
  srandom((int) time(0));
  for (i = 0; i < length; i++) {
    Array[i] = rand() % 9999;
  }
  for (i = 0; i < length; i++) {
    printf ("%d\n", Array[i]);
  }
  return 0;
}



