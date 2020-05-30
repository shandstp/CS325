#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//takes an array of integers by reference along with its size and sorts the contents into ascending order
void sort(int *arr, int sz){
    for(int i = 1; i < sz; i++){
        int tmp = arr[i];
        int cmp = i - 1;
        //check each element of the array. 
        //if current elemnt is smaller than the previous begining shifting elements forward until the proper location is found
        while(tmp < arr[cmp] && cmp >= 0){
            arr[cmp + 1] = arr[cmp];
            cmp--;
        }
        arr[cmp + 1] = tmp; //place element in the appropriate index
    }
}

//takes an array by reference and the size of that array as arguments and fills the array with random integers between 0 and 10,000
void fillArray(int *arr, int sz){
    for(int i = 0; i < sz; i++){
        arr[i] = rand() % 10001;
    }
}

int main(int argc, char* argv[]){
    srand(time(NULL)); //seed random number generator
    int nCount = 10; //set number of arrays to be sorted
    int n[] = {50000, 60000, 70000, 80000, 90000, 100000, 200000, 300000, 400000, 500000}; //set size of arrays to be sorted
    int i = 0;
    while(i < nCount){ 
        int* nums = malloc(sizeof(int) * n[i]); //allocate array
        fillArray(nums, n[i]); //fill new array with random integers 
        double start = clock(); //start clock to measure running time of algorithm
        sort(nums, n[i]); //perform sorting algorithm on newly filled array
        double runtime = (clock() - start) / CLOCKS_PER_SEC; //calculate time in seconds elapsed since algorithm started
        printf("%d %f\n", n[i], runtime); //print algorithm runtime and data set size to console
        free(nums); //free nums to be used again 
        i++; //increment to next data set size to be used
    }
    return 0;
}
