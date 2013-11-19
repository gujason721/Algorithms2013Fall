/**************************************************************************************************
                                        INSERTION SORT
Function: 
          - Read an input.txt file from C:\\input.txt and load all digits from the file in an array
          - Sort the array using Insertion sort
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
//***********************************************
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
//**************************************************
void InsertionSort(int a[], int l)
{
    int i, j, key;
    for (j=1; j<l; j++)
    {
        key = a[j];
        i = j-1;
        while (key < a[i] && i >= 0)
        {
            a[i+1] = a[i];
            i--;
        }
        a[i+1] = key;
    }
}
//****************************************************
int main()
{
    printf("\t\t\t\t INSERTION SORT\n");
    printf("\t\t\t ----------------------------\n\n\n\n");
    start = clock();
    ReadFile(); 
    InsertionSort(arr,length);
    WriteFile();
    finish = clock();
    printf("\nRunning time    :\t ");
    printf("%f",((double)(finish - start))/CLOCKS_PER_SEC);
    printf("s");
    getch();
    return 0;
}
