#ifndef _QUICK_SORT_H_
#define _QUICK_SORT_H_
#include<stdio.h>
#include<math.h>


int quick_sort(int* Array, int length)
{
  if (length <= 1) {
    return 0;
  }

  int i, j;
  int standard = Array[0];
  int index = 0;

  for (i = 1, j = length -1; i <= j; i++, j--) {
    while(Array[j] > standard) {
      j--;
    }
    if (j >= i) {
      Array[index] = Array[j];
      index = j;
    }

    while(Array[i] <= standard) {
      i++;
    }
    if (j >= i) {
      Array[index] = Array[i];
      index = i;
    }
  }
  Array[index] = standard;
  quick_sort(Array, index);
  quick_sort(Array + index + 1, length - i);

  return 0;
}
#endif
