#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
#include<time.h>
//*********************************************
int arr[2000]; 
int n = 2000; 
FILE *f1;
char inp[]="C:\\input.txt";
//*********************************************
void WriteFile()
{   
	f1 = fopen(inp,"w");
	for(int i=0;i<n;i++)
    {
        if (f1!=NULL)
	    {
          if (i!=n-1)
             fprintf(f1,"%d\n",rand()%10000);
          else
              fprintf(f1,"%d",rand()%10000);
    	}    	
     }
    fclose(f1);
}
//****************************************************
int main()
{
    WriteFile();
    return 0;
}
