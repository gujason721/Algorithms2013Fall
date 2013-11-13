#ifndef _HEAP_SORT_H_
#define _HEAP_SORT_H_
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

static void swap(int* a, int* b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}


int heap_adjust(int* Array, int index, int length)
{
  if (index * 2 > length) {
    return 0;
  }
  int child = index * 2;
  if (child + 1 > length) {
    if (Array[index] > Array[child]) {
      swap (Array + index, Array + child);
    }
    return 0;
  }

  if (Array[child] > Array[child + 1]) {
    child = child + 1;
  }
  if (Array[child] < Array[index]) {
    swap(Array + index, Array + child);
    heap_adjust(Array, child, length);
  }

  return 0;
}


int heap_sort(int* Array, int length)
{
  Array = Array - 1;
  int i;
  for (i = length; i >= 1; i--) {
    heap_adjust(Array, i, length);
  }

  int* tempArray = (int *) malloc (sizeof(int) * (length + 1));
  for (i = 1; i <= length; i++) {
    tempArray[i] = Array[i];
  }

  int j = 1;
  Array[j++] = tempArray[1];
  for (i = length; i >= 1; i--) {
    tempArray[1] = tempArray[i];
    heap_adjust(tempArray, 1, i);
    Array[j++] = tempArray[1];
  }
}

#endif
