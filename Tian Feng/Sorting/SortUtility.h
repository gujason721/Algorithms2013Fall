//
//  SortUtility.h
//  algorithm_sort
//
//  Created by Tian Feng on 16/9/13.
//  Copyright (c) 2013 Tian Feng. All rights reserved.
//

#ifndef algorithm_sort_SortUtility_h
#define algorithm_sort_SortUtility_h

#include "Sort.h"

class InsertionSort : public Sort
{
public:
    InsertionSort(string filePath);
    ~InsertionSort();
    
    bool executeSort();
};

class MergeSort : public Sort
{
private:
    void mergeSort(int *src, int *temp, int lPos, int rPos);

public:
    MergeSort(string filePath);
    ~MergeSort();
    
    bool executeSort();
};

class QuickSort : public Sort
{
private:
    int getPivot(int *data, int left, int right);
    void quickSort(int *data, int left, int right);
    
public:
    QuickSort(string filePath);;
    ~QuickSort();
    
    bool executeSort();
};

class HeapSort : public Sort
{
public:
    HeapSort(string filePath);
    ~HeapSort();
    
    void percDown(int *data, int i, int n);
    bool executeSort();
};

#endif
