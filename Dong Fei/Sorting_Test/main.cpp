//
//  main.cpp
//  Sorting_Test
//Created by D&F on 21/9/13.
//  Copyright (c) 2013 D&F. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>
#define MaxLength 100

using namespace std;

/*Sorting Algorithms*/

/*Insertion Sort*/
void Insertion_Sort(int Array[],int ArrayLength){
    int i=0,j;
    int key=0;
    for(j=1;j<ArrayLength;j++){
        key=Array[j];
        i=j-1;
        while(i>=0 && Array[i]>key){
            Array[i+1]=Array[i];
            i=i-1;
            Array[i+1]=key;
        }
    }
}
/*End of Insertion Sort*/

/*****************/

/*Merge Sort*/
void Merge(int *A, int * temp, int low, int mid, int high){
    int i,j,k=0,s=0;
    i=low;
    j=mid+1;
    
    while(i<=mid && j<=high){   //compare numbers in two short lists and merge
        if(A[i]<A[j]){
            temp[k++]=A[i++];
        }
        else
        {
            temp[k++]=A[j++];
        }
    }
    while(i<=mid)          //front lists data no comparison, need to be added in merged lists
        temp[k++]=A[i++];
    while(j<=high)          //behind lists data no comparison, need to added in merged lists
        temp[k++]=A[j++];
    for(s=0,i=low;i<=high;s++,i++)
        A[i]=temp[s];
}

void Merge_Sort(int *Array, int *temp, int low,int high){
    int mid;
    if(low<high)
    {
        mid=(low+high)/2;
        Merge_Sort(Array, temp, low,mid);
        Merge_Sort(Array, temp, mid+1,high);
        Merge(Array,temp,low,mid,high);
    }
}
/*End of Merge Sort*/


/*****************/

/*Quick Sort*/
/*void Sorting(int *A, int low,int mid,int high){
 int i=low,j=high;
 int temp;
 while(i!=j){
 for(j=high;j>mid;){               //compare A[j]and A[mid]
 if(A[j]<A[mid]){
 
 break;
 }
 else j--;
 }
 for(i=low;i<mid;){
 if(A[i]>A[mid]){
 
 break;
 }
 else i++;
 
 }
 temp=A[i];
 A[i]=A[j];
 A[j]=temp;
 
 }
 }
 void Quick_Sort(int *Array, int low,int high){
 int mid;
 if(low<high){
 mid=(low+high)/2;
 
 Sorting(Array,low,mid,high);
 Quick_Sort(Array,low,mid-1);
 Quick_Sort(Array,mid+1,high);
 
 }
 }*/

int Sorting(int *A, int low,int high){
    int i=low-1,j;
    int temp;
    int key=A[high];        //comparison key words equals to end number of Array
    for(j=low;j<high;j++){
        if(A[j]<=key){      //switch two numbers
            temp=A[i+1];
            A[i+1]=A[j];
            A[j]=temp;
            i++;
        }
        
    }
    temp=A[high];
    A[high]=A[i+1];
    A[i+1]=temp;
    return i+1;
}
void Quick_Sort(int *Array, int low,int high){
    int mid;
    if(low<high){
        mid=Sorting(Array,low,high);
        Quick_Sort(Array,low,mid-1);
        Quick_Sort(Array,mid+1,high);
        
    }
}


/*End of Quick Sort*/

/*****************/

/*Heap Sort*/

/*Build Heap*/
/*void Build_Heap(int A[], int L){
 int schild,i;
 int temp;
 for(i=L/2-1;i>=0;i--){
 for(temp=A[i];2*i+1<L;){
 schild=2*i+1;
 if(schild<=L-1 && A[schild+1]>A[schild]) ++schild;      //choose the maximum of two child node
 if(A[i]<A[schild]){
 
 A[i]=A[schild];
 A[schild]=temp;
 }
 else break;
 i=schild;           //Adjust child node
 temp=A[i];
 }
 }
 }*/

/* Adjust Heap Function */
void Adjust_Heap(int A[], int i, int L){
    
    int schild;
    int temp;
    
    for(temp=A[i];2*i+1<L;){
        schild=2*i+1;
        
        if(schild<L-1 && A[schild+1]>A[schild]) ++schild;      //choose the maximum of two child node
        if(A[i]<A[schild]){
            
            A[i]=A[schild];
            A[schild]=temp;
        }
        else break;
        i=schild;           //Adjust child node
        //  temp=A[i];
    }
}
/*End of Function Adjust_Heap*/



void Heap_Sort(int A[], int L){
    int tmp;
    int j;
    // Build_Heap(A, L);            //Build a heap with the top of heap is the maximum
    for(j=L/2-1;j>=0;j--)
        Adjust_Heap(A, j, L);
    for(int i=L-1; i>0;i--){            //switch R1 and Rn, that is the first number and last number
        tmp=A[i];
        A[i]=A[0];
        A[0]=tmp;
        Adjust_Heap(A, 0, i);           //Adjust the heap after switch the first number and the last number
    }
    
}

/*End of Heap Sort*/

int main(void)
{
    cout << "Place input.txt in the following directory: ";
    system("pwd");
  
    ifstream fp1;
    ofstream fp2("output.txt");
    ofstream fp3("time.txt",ios::app);
    double runtime=0.0;
    int k=0,low=0,high;
    int * Array;
    int *temp;
    int j=0;

    fp1.open("input.txt");
    if(!fp1){
        cout<<"cannot open the file!";
    }
   
    
    std::string line;
    std::vector<int> all_integers;
    while ( getline( fp1, line ) ) {
        all_integers.push_back(stoi(line));
    }
    
    int inputLength = int(all_integers.size());
    Array = new int [inputLength];
    for (int i = 0; i < inputLength;i++){
        Array[i] = all_integers[i];
    }
    
  /*  while(!fp1.eof()){
        fp1>>Array[i];
        ++i;
    }*/
    fp1.close();
    
    high=inputLength-1;
    
    temp=(int *)malloc(sizeof(int)*inputLength);
    clock_t start=clock();
   // Insertion_Sort(Array, inputLength);
    //Merge_Sort(Array, temp, low, high);
     //Quick_Sort(Array,low,high);
    //Heap_Sort(Array, inputLength);
    clock_t finish=clock();
    runtime=finish-start;
    while(k<inputLength){
        fp2<<Array[k]<<'\n';
        k++;
    }
    while(j<inputLength){
        cout<<Array[j]<<"\n";
        j++;
    }
    fp2.close();
    fp3<< "sorting time: "<< runtime <<" us\n";
    fp3.close();
    printf("%lf us\n",runtime);
    
}

