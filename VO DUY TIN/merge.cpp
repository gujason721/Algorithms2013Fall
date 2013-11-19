/**************************************************************************************************
                                        MERGE SORT
Function: 
          - Read an input.txt file from C:\\input.txt and load all digits from the file in an array
          - Sort the array using Merge sort
          - Create an ouput.txt file into C:\\output.txt and write these sorted digits in this file
          - Print onto the terminal: 
                  + Sorting kind
                  + Total numbers containing in the input file
                  + Running time of programm
*******************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
#include<time.h>
//*********************************************************
void Merge(int a[], int left, int mid, int right);
//*************************************************
int arr[2000]; 
int n = 2000; 
int length;
FILE *f1, *f2;
char inp[]="C:\\input.txt";
char outp[]="C:\\output.txt";
clock_t start, finish;
//****************************************************
void ReadFile()
{  
    int count=0; 
	f1 = fopen(inp,"r");
	for(int i=0;i<n;i++)
    {
        if (f1!=NULL && !feof(f1))
	    {
          fscanf(f1,"%d",&arr[i]);
          count++;
    	}    	
     }
    fclose(f1);
    length=(count<=n)?count:n;
    printf("Total of digits : \t ");
    printf("%d",count); 
}
//*********************************************
void WriteFile()
{   
	f2 = fopen(outp,"w");
	for(int i=0;i<length;i++)
    {
        if (f2!=NULL)
	    {
          fprintf(f2,"%d\n",arr[i]);
    	}    	
     }
    fclose(f2);
}
//*********************************************************  
void MergeSort(int a[], int left, int right)
{
    int mid;
    if (left < right) 
    {
        mid = (right + left)/2;
        MergeSort(a, left, mid);
        MergeSort(a, mid + 1, right);
        Merge(a, left, mid, right);
    }
}
//************************************************** 
void Merge(int a[], int left, int mid, int right)
{
    int b[2000];
    int i = left;
    int j = mid + 1;
    int k = 0;
    while (i <= mid && j <= right) 
    {
        if (a[i] <= a[j])
        {
            b[k] = a[i];
            k++;
            i++;
        }
        else
        {
            b[k] = a[j];
            k++;
            j++;
        }
    }
    while (i <= mid)
    {
        b[k] = a[i];
        k++;
        i++;
    }
    while (j <= right)
    {
        b[k] = a[j];
        k++;
        j++;
    }
    k--;
    while (k >= 0) 
    {
        a[left + k] = b[k];
        k--;
    }
}
//****************************************************
int main()
{
    printf("\t\t\t\t MERGE SORT\n");
    printf("\t\t\t --------------------------\n\n\n\n");
    start = clock();    
    ReadFile(); 
    MergeSort(arr,0,length-1); 
    WriteFile();
    finish = clock();
    printf("\nRunning time    :\t ");
    printf("%f",((double)(finish - start))/CLOCKS_PER_SEC);
    printf("s");
    getch();
    return 0;
}
