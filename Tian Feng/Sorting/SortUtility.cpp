//
//  SortUtility.cpp
//  algorithm_sort
//
//  Created by Tian Feng on 16/9/13.
//  Copyright (c) 2013 Tian Feng. All rights reserved.
//

#include "SortUtility.h"
#include <time.h>
//#include <Windows.h>
#include <iostream>
using namespace std;

InsertionSort::InsertionSort(string filePath) : Sort(filePath)
{
    
}

InsertionSort::~InsertionSort()
{
    
}

bool InsertionSort::executeSort()
{
    bool bRes = false;
    
	clock_t start, end;
	start = clock();

// 	LARGE_INTEGER freq, start, end;
// 	QueryPerformanceFrequency(&freq);
// 	QueryPerformanceCounter(&start); 
    
    try {
        for (int i = 1; i < _length; i++)
        {
            int key = _data[i];
            int j = i - 1;
            
            while (j >= 0 && _data[j] > key) {
                _data[j + 1] = _data[j];
                j--;
            }
            
            _data[j + 1] = key;
        }
        
        bRes = true;
    } catch (exception e) {
        bRes = false;
    }
    
	end = clock();
	_timeUsage = (double)(end - start);

// 	QueryPerformanceCounter(&end);
// 	_timeUsage = (double)(end.QuadPart - start.QuadPart) / (double)freq.QuadPart * 1000;

    return bRes;
}

MergeSort::MergeSort(string filePath) : Sort(filePath)
{
    
}

MergeSort::~MergeSort()
{
    
}

bool MergeSort::executeSort()
{
    bool bRes = false;
    
    clock_t start, end;
    start = clock();

// 	LARGE_INTEGER freq, start, end;
// 	QueryPerformanceFrequency(&freq);
// 	QueryPerformanceCounter(&start); 
    
    int *temp = new int[_length];
    
    try {
        if (NULL == temp) {
            return bRes;
        }
        
        mergeSort(_data, temp, 0, _length - 1);
        delete [] temp;
        
        bRes = true;
    } catch (exception e) {
        bRes = false;
    }
    
    end = clock();
    _timeUsage = (double)(end - start);
//  	QueryPerformanceCounter(&end);   
// 	_timeUsage = (double)(end.QuadPart - start.QuadPart) / (double)freq.QuadPart * 1000;

    return bRes;
}

void MergeSort::mergeSort(int *src, int *temp, int left, int right)
{
    if (left < right) {
        int center = (left + right) / 2;
        
        mergeSort(src, temp, left, center);
        mergeSort(src, temp, center + 1, right);
        
        int lPos = left;
        int rPos = center + 1;
        int lEnd = center;
        int rEnd = right;
        int tempPos = lPos;
        int num = rEnd - lPos + 1;
        
        while (lPos <= lEnd && rPos <= rEnd) {
            if (src[lPos] <= src[rPos]) {
                temp[tempPos++] = src[lPos++];
            } else {
                temp[tempPos++] = src[rPos++];
            }
        }
        
        while (lPos <= lEnd) {
            temp[tempPos++] = src[lPos++];
        }
        
        while (rPos <= rEnd) {
            temp[tempPos++] = src[rPos++];
        }
        
        for (int i = 0; i < num; i++, rEnd--) {
            src[rEnd] = temp[rEnd];
        }
    }
    
    return;
}

QuickSort::QuickSort(string filePath) : Sort(filePath)
{
    
}

QuickSort::~QuickSort()
{
    
}

int QuickSort::getPivot(int *data, int left, int right)
{
    int center = (left + right) / 2;
    
    if (data[left] > data[center]) {
        Sort::swap(&data[left], &data[center]);
    }
    
    if (data[left] > data[right]) {
        Sort::swap(&data[left], &data[right]);
    }
    
    if (data[center] > data[right]) {
        Sort::swap(&data[center], &data[right]);
    }

    Sort::swap(&data[center], &data[right - 1]);
    
    return data[right - 1];
}

void QuickSort::quickSort(int *data, int left, int right)
{
    if (right - left >= 3) {
        int i = left;
        int j = right - 1;
        int pivot = getPivot(data, left, right);
        
        for (;;)
        {
            while (data[++i] < pivot) {}
            while (data[--j] > pivot) {}
            
            if (i < j) {
                Sort::swap(&data[i], &data[j]);
            } else
                break;
        }
        
        Sort::swap(&data[i], &data[right - 1]);
        
        quickSort(data, left, i - 1);
        quickSort(data, i + 1, right);
        
    } else if (right - left == 2) {
        if (data[left] > data[right]) {
            Sort::swap(&data[left], &data[right]);
        }
        if (data[left] > data[left + 1]) {
            Sort::swap(&data[left], &data[left + 1]);
        }
        if (data[left + 1] > data[right]) {
            Sort::swap(&data[left + 1], &data[right]);
        }
        
    } else if (right - left == 1) {
        if (data[left] > data[right]) {
            Sort::swap(&data[left], &data[right]);
        }
        
    }

    return;
}


bool QuickSort::executeSort()
{
    bool bRes = false;
    
	clock_t start, end;
	start = clock();

// 	LARGE_INTEGER freq, start, end;
// 	QueryPerformanceFrequency(&freq);
// 	QueryPerformanceCounter(&start); 
    
    try {
        quickSort(_data, 0, _length - 1);
        bRes = true;
    } catch (exception e) {
        bRes = false;
    }
    
    end = clock();
    _timeUsage = (double)(end - start);
	
// 	QueryPerformanceCounter(&end);
// 	_timeUsage = (double)(end.QuadPart - start.QuadPart) / (double)freq.QuadPart * 1000;
    
    return bRes;
}

HeapSort::HeapSort(string filePath) : Sort(filePath)
{
    
}

HeapSort::~HeapSort()
{
    
}

void HeapSort::percDown(int *data, int i, int n)
{
    int child = 0;
    int temp = 0;
    
    for (temp = data[i]; 2 * i + 1 < n; i = child) {
        child = 2 * i + 1;
        
        if (child != n - 1 && data[child + 1] > data[child]) {
            child++;
        }
        
        if (temp < data[child]) {
            data[i] = data[child];
        } else {
            break;
        }
    }
    
    data[i] = temp;
}

bool HeapSort::executeSort()
{
    bool bRes = false;
    
    clock_t start, end;
    start = clock();

// 	LARGE_INTEGER freq, start, end;
// 	QueryPerformanceFrequency(&freq);
// 	QueryPerformanceCounter(&start); 
    
    try {
        int i = 0;
        
        for (i = _length / 2; i >= 0; i--) {
            percDown(_data, i, _length);
        }
        
        for (i = _length - 1; i > 0; i--) {
            Sort::swap(&_data[0], &_data[i]);
            percDown(_data, 0, i);
        }
        bRes = true;
    } catch (exception e) {
        bRes = false;
    }

	end = clock();
	_timeUsage = (double)(end - start);
	
// 	QueryPerformanceCounter(&end);
// 	_timeUsage = (double)(end.QuadPart - start.QuadPart) / (double)freq.QuadPart * 1000;
    
    return bRes;
}