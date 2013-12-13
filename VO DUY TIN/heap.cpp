/*******************************************************************************************************
                                        HEAP SORT
Function: 
          - Read an input.txt file from C:\\input.txt and load all digits from the file in an array
          - Sort the array using Heap sort
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
//*************************************************
void BuildMaxHeap(int a[]); 
void Heapify(int a[], int i) ;
//*************************************************
int arr[2000]; 
int n = 2000; 
int length, heapSize;
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
//***********************************************
void HeapSort(int a[]) 
{
    BuildMaxHeap(a);
    for (int i=length-1;i>0;i--) 
    {
        int temp = a[0];
        a[0] = a[i];
        a[i] = temp;
        heapSize = heapSize - 1;
        Heapify(a, 0);
    }
}
//******************************************
void BuildMaxHeap(int a[]) 
{
     heapSize=length;
     for (int i=(length-1)/2;i>=0;i--) 
     {
         Heapify(a,i);
     }
}
//****************************************
void Heapify(int a[], int i) 
{
     int largest;
     int left = (2*i)+1;
     int right = (2*i)+2;
     if ((a[left]>a[i]) && (left<heapSize))
     {
        largest = left;
     }
     else 
     {
        largest = i;
     }
     if ((a[right]>a[largest]) && (right<heapSize))
     {
        largest = right;
     }
     if (largest!= i)
     {
        int temp = a[i];
        a[i] = a[largest];
        a[largest] = temp;
        Heapify(a, largest);
     }
}
//****************************************************
int main()
{
    printf("\t\t\t\t HEAP SORT\n");
    printf("\t\t\t --------------------------\n\n\n\n");
    start = clock();    
    ReadFile(); 
    HeapSort(arr);
    WriteFile();
    finish = clock();
    printf("\nRunning time    :\t ");
    printf("%f",((double)(finish - start))/CLOCKS_PER_SEC);
    printf("s");
    getch();
    return 0;
}
