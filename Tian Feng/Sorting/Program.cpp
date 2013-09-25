//
//  main.cpp
//  algorithm_sort
//
//  Created by Tian Feng on 16/9/13.
//  Copyright (c) 2013 Tian Feng. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
using namespace std;

#include "Sort.h"
#include "SortUtility.h"

int main(int argc, const char * argv[])
{
    string str = "NULL";
    int typeVal = 0;
    SortType type = Insertion;
    
    cout<<"Please enter the path to the input file (.txt):\n";
    cin>>str;
    cout<<"\nType 1 for Insertion Sort;\nType 2 for Merge Sort;\nType 3 for Quick Sort; and\nType 4 for Heap Sort.\n";
    cout<<"\nPlease choose the sort strategy you want:\n";
    cin>>typeVal;
    
    type = (SortType)typeVal;
    Sort* pSort = Sort::getSortInstance(str, type);
    
    if (pSort->_bIsDataValid) {
        pSort->executeSort();
        cout<<"\nSorting Result:\n";
        pSort->showData();
        cout<<"\nTime Usage:\n"<<pSort->_timeUsage<<" ms.\n";
        
        string outputStr = "NULL";
        cout<<"\nPlease enter the path to the export file (.txt):\n";
        cin>>str;
        if (pSort->exportData(str)) {
            cout<<"\nCongratulations! Please exit and check the export file.\n";
        } else {
            cout<<"\nUnable to execute export process due to data error, please exit and try then.\n";
        }
    } else {
        cout<<"\nUnable to execute sort process due to data error, please exit and try then.\n";
    }
    
    delete pSort;

	system("pause");
    
    return 0;
}



