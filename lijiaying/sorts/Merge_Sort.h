#ifndef _MARGE_SORT_H_
#define _MARGE_SORT_H_
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int combine_sort(int* Array, int half, int length, int* tempArray)
{
  int i = 0;
  for (i = 0; i < length; i++) {
    tempArray[i] = Array[i];
  }

  int j, k = 0;
  int* p = tempArray; 
  int* q = tempArray + half;
  for (;; k++) {
    if (p >= tempArray + half) {
      while (k < length){
        Array[k++] = *q;
        q++;
      }
      return 0;
    }
    if (q >= tempArray + length) {
      while (k < length){
        Array[k++] = *p;
        p++;
      }
      return 0;
    }

    if (*p < *q) {
      Array[k] = *p;
      p++;
    } else {
      Array[k] = *q;
      q++;
    }
  }
  return 0;
}



int merge_sort_(int* Array, int length, int* tempArray)
{
  if (length > 1) {
    int half = length / 2;
    merge_sort_(Array, half, tempArray);
    merge_sort_(Array + half, length - half, tempArray);
    combine_sort(Array, half, length, tempArray);
  }
  return 0;
}

int merge_sort(int* Array, int length)
{
  int* tempArray =  (int *)malloc(sizeof(int) * length);
  merge_sort_(Array, length, tempArray);
  free(tempArray);
  return 0;
}
#endif
