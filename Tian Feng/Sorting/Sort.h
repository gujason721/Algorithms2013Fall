//
//  Sort.h
//  algorithm_sort
//
//  Created by Tian Feng on 16/9/13.
//  Copyright (c) 2013 Tian Feng. All rights reserved.
//

#ifndef algorithm_sort_sort_h
#define algorithm_sort_sort_h

#include <string>
#define MAX_SIZE 65535
using namespace std;

enum SortType
{
    Insertion = 1,
    Merge = 2,
    Quick = 3,
    Heap = 4
};

class Sort
{
protected:
    int *_src;
    int *_data;
    int _length;

public:
    Sort();
    Sort(string filePath);
    ~Sort();
    
    static Sort* getSortInstance(string filePath, SortType sortType);
    virtual bool executeSort();
    void showData();
    static void swap(int *a, int *b);
    bool exportData(string filePath);
    
    bool _bIsDataValid;
    double _timeUsage;
};

#endif
