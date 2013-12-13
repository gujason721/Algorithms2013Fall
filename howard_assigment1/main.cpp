#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<time.h>
#include<limits.h>
#include<stdlib.h>

using namespace std;

//insertioin sort
void insertion_sort(int A[], int n);
// merge sort
void mrg(int *A, int p, int q, int r);
void mrg_sort(int *A, int p, int r);
// heapsort
void max_heapify(int *A, int i, int heapsize);
void build_maxheap(int *A, int heapsize);
void heapsort(int *A, int heaplength);
//quicksort
void quicksort(int *A, int p, int r);
int prts(int *A, int p, int r);

void swp(int *a, int *b);

int main()
{
    int const N = 10000;
    int A[N];
    int sze;
    int i=0;
    clock_t start, finish;

    ifstream f;
    ofstream fout;

// read in data
    f.open("C:\\Users\\howard_yeung\\Documents\\for_cpp\\howard_assigment1\\input.txt");
    if(f == NULL){
        cout<<"there is error";
        exit(-1);
    }
    while(!f.eof()){
        f>>A[i];
        ++i;
    }
    sze = i;// define the size of the array
    f.close();

    int heapsize = sze-1;

    start = clock();
// four kinds of sorting
    insertion_sort(A, sze);
    //mrg_sort(A,0, sze-1);
    //heapsort(A, heapsize);
    //quicksort(A, 0, sze-1);
    finish = clock();

//write data
   fout.open("C:\\Users\\howard_yeung\\Documents\\for_cpp\\howard_assigment1\\output.txt");
   if(!fout){
    cerr<<"Error!"<<'\n';
    exit(-1);
   }

   int kk = 0;
   for(kk=0; kk!=sze; kk++){
    fout<<A[kk]<<'\n';
   }
   fout.close();

    cout<<"\nSorting time: "<<(finish-start)<<" ms\n";
    return 0;
}

void insertion_sort(int A[], int n){
    int key;
    int i, j;

    for(j=1; j!=n; j++){
    key = A[j];
    i = j-1;

        while(i>=0 && A[i] > key){
            A[i+1] = A[i];
            i--;
        }
    A[i+1] = key;
    }

}

void mrg(int *A, int p, int q, int r){
    int n_1, n_2;
    int *p1;
    int *p2;
    int i, j, k;

    n_1 = q - p + 1;
    n_2 = r - q ;

    // make two new arrays
    p1 = (int *)malloc((1+n_1)*sizeof(int));
    p2 = (int *)malloc((1+n_2)*sizeof(int));

    for(i = 0; i != n_1; i++){
        *(p1+i) = A[p+i];
    }
    *(p1 + n_1) = INT_MAX;

    for(j = 0; j != n_2; j++){
        *(p2 + j) = A[q + j + 1];
    }
    *(p2 + n_2) = INT_MAX;

    i = j = 0;
    for(k = p; k != r+1; k++){
        if(*(p1+i) < *(p2+j)){
            A[k] = *(p1+i);
            ++i;
        }
        else{
            A[k] = *(p2+j);
            ++j;
        }

    }
    free(p1);
    free(p2);

}

void mrg_sort(int *A, int p, int r){
    int q;

    if(p < r){
        q = (int)((p+r)/2);

    mrg_sort(A, p, q);
    mrg_sort(A, q+1, r);
    mrg(A, p, q, r);
    }

}

void max_heapify(int *A, int i, int heapsize){
    int lf = 2*i + 1;
    int rt = 2*i + 2;
    int largest;

// max heap procedure, make the largest at the root
    if(lf <= heapsize && A[lf] > A[i])
            largest = lf;
    else
        largest = i;
    if(rt <= heapsize && A[rt] > A[largest])
        largest = rt;
// if the root is not the largest, swap it and go down stears
    if(largest != i){
            swp(&A[i], &A[largest]);
            max_heapify(A, largest, heapsize);
    }

}

void build_maxheap(int *A, int heapsize){
    int i;

    for(i = (int)((heapsize-1)/2); i != -1; i--){
        max_heapify(A, i, heapsize);
    }
}

void heapsort(int *A, int heapsize){
    int i;

    build_maxheap(A, heapsize);
    for(i = heapsize; i != 0; i--){
        swp(&A[0], &A[i]);
        heapsize--;
        max_heapify(A, 0, heapsize);
    }
}

void quicksort(int *A, int p, int r){
    int q;

    if(p<r){
        q = prts(A, p, r);
        quicksort(A, p, q-1);
        quicksort(A, q+1, r);
    }
}

int prts(int *A, int p, int r){
    int x, i, j;

    x = A[r];
    i = p-1;
    for(j=p; j!=r; j++){
        if(A[j] <= x){
            ++i;
            swp(&A[i], &A[j]);
        }
    }
    swp(&A[i+1], &A[r]);

    return i+1;
}


void swp(int *a, int *b){
    int dino;

    dino = *a;
    *a = *b;
    *b = dino;
}
