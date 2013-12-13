#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Merge_Sort.h"
#include "Insertion_Sort.h"
#include "Heap_Sort.h"
#include "Quick_Sort.h"

#define Algorithm_Numbers 4

struct sort_algorithm {
  char name[10];
  int (*func) (int* A, int length);
};


struct sort_algorithm SA[Algorithm_Numbers] = {
  {"insertion", insertion_sort},
  {"heap", heap_sort},
  {"merge", merge_sort},
  {"quick", quick_sort}
};



int main(int argc, char** argv)
{
  FILE* f;
  int k = 0;

  f = fopen("./data", "rb+");
  int length = 1000000;  
  int tempArray[length];
  int i = 0;
  clock_t start, end;
  while (!feof(f) && i < length) {
    fscanf(f, "%d\n", &tempArray[i++]);
  }
  fclose(f);
  length = i;

  int Array[length];
  printf("%6d\t", length);
  while (k < Algorithm_Numbers) {
    for (i = 0; i < length; i++) {
      Array[i] = tempArray[i];
    }

    start = clock();
    SA[k].func(Array, length);
    end = clock();

    //    printf("(%d, %f)\t", length, (double)(end - start)/CLOCKS_PER_SEC * 1000);
    printf("%8ld\t", (end - start));

    #ifdef _FILE_OUT_
    f = fopen(SA[k].name, "wb+");
    for (i = 0; i < length; i++) {
      fprintf(f, "%d\n", Array[i]);
    }
    fclose(f);
    #endif

    k++;
  }
  printf("\n");
  return 0;
}



