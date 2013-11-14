//
//  Sort.cpp
//  algorithm_sort
//
//  Created by Tian Feng on 16/9/13.
//  Copyright (c) 2013 Tian Feng. All rights reserved.
//

#include "Sort.h"
#include "SortUtility.h"

#include <fstream>
#include <vector>
#include <iostream>
using namespace std;

Sort::Sort()
{
    
}

Sort::Sort(string filePath)
{
    fstream fs(filePath);
    _src = new int[MAX_SIZE];
    _data = NULL;
    
    int temp = 0;
    int pointer = 0;
    
    try {
        while (fs>>temp) {
            _src[pointer] = temp;
            pointer++;
        }
    } catch (exception e) {
        cout<<"-------------\nFailed reading the input file!\n";
        _bIsDataValid = false;
        return;
    }
    
    try {
        fs.close();
    } catch (exception e) {
        cout<<"-------------\nFailed closing the input file!\n";
        _bIsDataValid = false;
        return;
    }
    
    if (0 == pointer) {
        cout<<"-------------\nThere is no data in the input file!\n";
        _bIsDataValid = false;
        return;
    }
    else
    {
        _length = pointer;
        _data = new int[_length];
        
        for (int i = 0; i < _length; i++) {
            _data[i] = _src[i];
        }

    }
    
    delete [] _src;
    _src = NULL;
    
    _bIsDataValid = true;
}

Sort::~Sort()
{    
    if (NULL != _data) {
        delete [] _data;
    }
    
    _data = NULL;
    _length = 0;
}

Sort* Sort::getSortInstance(string filePath, SortType sortType)
{
    Sort* pSort = NULL;
    switch (sortType) {
        case Insertion:
            pSort = new InsertionSort(filePath);
            break;
        case Merge:
            pSort = new MergeSort(filePath);
            break;
        case Quick:
            pSort = new QuickSort(filePath);
            break;
        case Heap:
            pSort = new HeapSort(filePath);
            break;
        default:
            break;
    }
    
    return pSort;
}

bool Sort::executeSort()
{
    return true;
}

void Sort::showData()
{
    for (int i = 0; i < _length; i++)
    {
        cout<<_data[i]<<" ";
    }
    
    cout<<"\n";

    return;
}

void Sort::swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
    
    return;
}

bool Sort::exportData(string filePath)
{
    bool bRes = false;
    
    try {
        fstream fs(filePath, ofstream::out);
        fs<<"Time Usage: "<<_timeUsage<<" ms\n";
        
        for (int i = 1; i < _length; i++) {
            fs<<_data[i]<<"\n";
        }
        
        fs.close();
        
        bRes = true;
    } catch (exception e) {
        bRes = false;
    }
    
    return bRes;
}
