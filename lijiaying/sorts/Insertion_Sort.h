#ifndef _INSERTION_SORT_H_
#define _INSERTION_SORT_H_
#include<stdio.h>
#include<math.h>

int insertion_sort(int* Array, int length)
{
  if (length <= 1) {
    return 0;
  }

  int j, k = 1;
  int temp = 0;
  while (k < length){
    j = k - 1;
    temp = Array[k];
    while (j >= 0) {
      if (temp < Array[j]) {
        Array[j+1] = Array[j];
        j--;
      }
      else { 
        break;
      }
    }
    Array[j+1] = temp;
    k++;
  }
  return 0;
}

#endif
