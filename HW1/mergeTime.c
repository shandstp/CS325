#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Takes an array by reference, a start, middle, and end index, copies the values inbetween 
//into two arrays and replaces the values in sorted order into the original array
void merge(int *arr, int l, int m, int r){
    int sz1 = m - l + 1; //calculate size of left array
    int sz2 = r - m; //calculate size of right array
    int larr[sz1]; 
    int rarr[sz2];
    //copy values into left array
    for(int i = 0; i < sz1; i++){
        larr[i] = arr[l + i];
    }
    //copy values into right array
    for(int i = 0; i < sz2; i++){
        rarr[i] = arr[m + 1 + i];
    }

    int i = 0;
    int j = 0;
    //copy values, in sorted order, back into source array
    for(int k = l; k <= r; k++){
       //if no values remain in left array, copy remaining value from right array
       if(i >= sz1){
           arr[k] = rarr[j];
           j++;
       }
       //if no values remain in right array, copy remaining value from left array
       else if(j >= sz2){
           arr[k] = larr[i];
           i++;
       }
       //if current value in left array is less than current value of right array, copy to current index of source array
       else if(larr[i] <= rarr[j]){
          arr[k] = larr[i];
          i++; 
       }
       //if current value in right array is less than current value of left array, copy to current index of source array
       else{
           arr[k] = rarr[j];
           j++;
       }
    }
}

//takes an array of integers by reference along with its size and sorts the contents into ascending order
void sort(int *arr, int l, int r){
    if(l < r){
        int m = (l+r) / 2; //calculate middle of current subarray
        sort(arr, l, m); //further divide left half
        sort(arr, m + 1, r); //further divide right half
        merge(arr, l, m, r); //put it all back together
    }
}

//takes an array and size of that array as arguments and fills the array with random integers
void fillArray(int *arr, int sz){
    for(int i = 0; i < sz; i++){
        arr[i] = rand() % 10001;
    }
}

int main(int argc, char* argv[]){
    srand(time(NULL)); //seed random number generator
    int nCount = 10; //number of arrays to be sorted
    int n[] = {50000, 60000, 70000, 80000, 90000, 100000, 200000, 300000, 400000, 500000}; //size of arrays to be sorted
    int i = 0;
    while(i < nCount){
        int* nums = malloc(sizeof(int) * n[i]); //allocate space for current array to be sorted
        fillArray(nums, n[i]); //fill array with random integers
        double start = clock(); //get start time of sorting algorithm
        sort(nums, 0, n[i]); //sort array
        double runtime = (clock() - start) / CLOCKS_PER_SEC; //calculate time in seconds of time elapsed while sorting
        printf("%d %f\n", n[i], runtime); //print data set size followed by runtime of algorithm
        free(nums); //free array for reuse
        i++; //increment to next array size
    }
    return 0;
}
