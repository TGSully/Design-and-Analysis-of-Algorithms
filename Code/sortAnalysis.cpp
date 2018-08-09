/*
Taylor Sullivan
COT 4400 Design and Analysis of Algorithms
Term Project

Description:  This program implements Insertion Sort, Quick Sort and 
Heap Sort algorithms.  Each algorithm sorts an array of varying sizes 
(ns <= n <= nf).  After sorting, the measured runtime is calculated for each 
iteration. Then the average is taken of these execution times and
saved in an output file to be plotted.
*/
#include <string>
#include <sys/time.h> //time eval
#include <stdlib.h>  
#include <fstream>  
#include <iostream>
#include <cmath>

using namespace std;

double ns = 1000;
double nf = 20000;
double delta = 1000;

void insertionSort(int *A, int n);

void maxHeapify(int*A, int n, int i);
void heapSort(int *A, int n);

void quickSort(int *A, int p, int r);
double partition(int *A, int p, int r);


void quickSort(int *A, int p, int r) {
    int q = 0; 
            if (p < r) {
                q = partition (A,p,r);
                quickSort(A, p, q-1);
                quickSort(A, q+1, r);
            }
}

double partition(int *A, int p, int r) {
    int x = 0;
    int i = 0;
    int j = 0;
    int temp = 0;
    x = A[r]; //pivot
    i= p-1; //lower index
    for(int j = p; j <= r-1; j++) {
        if(A[j] <= x) {
            i++;
            temp = A[i]; //exchange A[i] with A[j]
            A[i] = A[j];
            A[j] = temp;
        }
    }
    temp = A[i+1]; //Exchange A[i+1] with A[r]
    A[i+1] = A[r];
    A[r] = temp;
    return i+1;
    
}

void maxHeapify(int *A, int n, int i) {
    // n = heapsize is implied since n is a parameter
    int l = (2 * i) + 1;     //left child
    int r = (2 * i) + 2;    //right child
    int largest = i;
    int temp = 0;           // for swap

    if (l < n && A[l] > A[i]) {
        largest = l;
    }else {  
        largest = i;
    }
    if (r < n && A[r] > A[largest]) {
        largest = r;
    }
    if (largest != i) {
        temp = A[i];  //exchange A[i] and A[largest]
        A[i] = A[largest];
        A[largest] = temp;
        maxHeapify(A, n, largest); 
    }
}

void heapSort(int *A, int n) {
    int temp= 0;
    int heapsize = n;

        for(int i = floor(n / 2) - 1  ; i >= 0 ; i--) {  //Build Max Heap
            
            maxHeapify(A, n, i);
        }

        for(int i = n - 1 ; i >= 1 ; i--) {  
            temp = A[0];  //exchange A[0] and A[i]
            A[0] = A[i];
            A[i] = temp;
            heapsize = heapsize - 1;    
            
            maxHeapify(A, i , 0);
           
        }    
}

void insertionSort(int *A, int n) {
    int x = n;
    int i = 0;
    int key = 0;
    for(int j=1; j <= x-1; j++) {  //insertion sort
        key = A[j];
        i = j-1;
        while( i > 0 && A[i] > key) {
            A[i+1] = A[i];
            i = i - 1;
        }    
        A[i+1] = key;
    }    
}


int main() {
    double m = 10; 
    int *A = NULL; 
    int *B = NULL; //array to copy to

    struct timeval start, finish;  // for timer
    double insert_sum = 0;
    double heap_sum = 0;
    double quick_sum = 0;
    double insert_avg = 0;
    double heap_avg = 0;
    double quick_avg = 0;

    ofstream insertPlot, heapPlot, quickPlot;

    insertPlot.open("insertPlot.csv");
    heapPlot.open("heapPlot.csv");
    quickPlot.open("quickPlot.csv");


    for (int x= ns; x <= nf; x += delta) {  // array setup rand num

        for(int z = 0; z < m; z++) { /// m=10 iterations
            A = new int[x];
            B =new int[x]; 

            for (int y=0; y <= x-1; y++) {  //creates unsorted array
                A[y] = rand();    // A[y] is populated with random numbers
                B[y] = A[y];                //copy A[] to B[] (B[] preserves array A)
                //cout << A[y] << endl;       //prints rand array
            }

            //////////////////// insertion sort /////////////////////
            
            gettimeofday(&start, NULL);  // start timer in microseconds
            double st = (start.tv_sec * 1000000 + start.tv_usec);

            insertionSort(A, x);

            gettimeofday(&finish, NULL);   // end timer
            double fn = (finish.tv_sec * 1000000 + finish.tv_usec);
            insert_sum += (fn - st);
            st = 0;  //reset timer
            fn = 0;

            // cout << " Insertion-sorted array" << endl;
            // for(int k = 0 ; k <= x-1 ; k++) { // prints array
            //     cout << A[k] << endl;
            // } 


            for (int y=0; y <= x-1; y++) { //reloads same unsorted array for iteration of m
                A[y] = B[y];    // restores A[] to original
            //  cout << A[y] << endl;       //prints rand array
            }

            /////////////////// heap-sort ///////////////////////

            gettimeofday(&start, NULL);  // start timer
            st = start.tv_sec * 1000000 + start.tv_usec;

            heapSort(A, x);

            gettimeofday(&finish, NULL);   // end timer
            fn = finish.tv_sec * 1000000 + finish.tv_usec;
            heap_sum += (fn - st);    // RT accumulator
            st = 0;  //reset timer
            fn = 0;
        
            // cout << " Heap-sorted array" << endl;
            // for(int k = 0 ; k <= x-1 ; k++) { // prints sorted array
            //     cout << A[k] << endl;
            // } 

            for (int y=0; y <= x-1; y++) { //reloads same unsorted array for iteration of m
                A[y] = B[y];    // restores A[] to original
            //   cout << A[y] << endl;       //prints rand array
            }

            //////////////////// quick sort /////////////////////

            gettimeofday(&start, NULL);  // start timer
            st = start.tv_sec * 1000000 + start.tv_usec;;        

            quickSort(A,0,x-1);

            gettimeofday(&finish, NULL);   // end timer
            fn = finish.tv_sec * 1000000 + finish.tv_usec;
            quick_sum += (fn - st);    // RT accumulator
            st = 0;  //reset timer
            fn = 0;    

            // cout << " Quick-sorted array" << endl;
            // for(int k = 0 ; k <= x-1 ; k++) { // prints array
            //     cout << A[k] << endl;
            // }  
 
        } 

        insert_avg = (insert_sum / m);  //computes RT averages 
        heap_avg = (heap_sum / m);
        quick_avg = (quick_sum / m);

        insertPlot << insert_avg << ',' << x << endl; // data input for .csv
        heapPlot << heap_avg << ',' << x << endl;
        quickPlot << quick_avg << ',' << x << endl;
    }
     
insertPlot.close();
heapPlot.close();
quickPlot.close();

return 0;
}
