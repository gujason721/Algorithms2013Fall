/*******************************************************************************************************
                                        QUICK SORT
Function: 
          - Read an input.txt file from C:\\input.txt and load all digits from the file in an array
          - Sort the array using Quick sort
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
int Partition(int a [], int left, int right);
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
//***********************************************
void QuickSort(int a[], int left, int right)
{
   int pivot;
   if(left >= right)
       return;
   if(left < right)
   {
       pivot = Partition(a, left, right);
       QuickSort(a, left, pivot - 1);
       QuickSort(a, pivot + 1, right);
   }
}
//****************************************************
int Partition(int a[], int left, int right)
{
   int x, j, i, temp;
   x = a[right];                                                        
   i = left-1;
   for(j=left;j<right;j++)
   {
      if(a[j] <= x)
      {
            i++;
            temp = a[j];
            a[j] = a[i];
            a[i] = temp;
      }
   }
   temp = a[i+1];    
   a[i+1] = a[right];
   a[right] = temp; 
   return i+1;
}
//****************************************************
int main()
{
    printf("\t\t\t\t QUICK SORT\n");
    printf("\t\t\t --------------------------\n\n\n\n");
    start = clock();    
    ReadFile(); 
    QuickSort(arr,0,length-1); 
    WriteFile();
    finish = clock();
    printf("\nRunning time    :\t ");
    printf("%f",((double)(finish - start))/CLOCKS_PER_SEC);
    printf("s");
    getch();
    return 0;
}
