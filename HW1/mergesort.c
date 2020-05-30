#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

//takes an integer array by reference and a string buffer. 
//Converts each number in the string to an integer and stores the result at the next available index of the array
void fillArray(int *arr, char* line){
    char* substr = strtok(line, " \0"); //separate each number into its own string
    int index = 0;
    while(substr != NULL){ //loop through string until now numbers remain
        arr[index] = atoi(substr); //convert number string to integer and store in array
        index++;
        substr = strtok(NULL, " \0"); //get next number from string or NULL if all numbers have be retrieved
    }
}

//counts how many numbers a given string contains
int countNums(char* line, int* offset){
    int index = 0;
    int count = 0;

    //return zero if string is empty
    if(line[0] == ' '){
        return count;
    }
    //find space after first number
    while(line[index] != ' '){
        index++;
    }
    line[index] = '\0'; //terminat string containing first number
    count = atoi(line); //convert first number to integer
    *offset = index + 1; //calculate offset to new start of string
    return count; //return number count
}


int main(int argc, char* argv[]){
    size_t strln = 0; //stores length of each line read from file
    char* buff = NULL; //stores each line read from file
    int status = 1; //indicates the status of last attempt to read from file. -1 when EOF is read
    FILE* data = fopen("data.txt", "r"); //open file stream
    FILE* out = fopen("merge.out", "w"); //open output file for writing
    while((status = getline(&buff, &strln, data) != -1)){ //read each line from file until EOF is reached
        int offset = 0;
        int numCount = countNums(buff, &offset); //count numbers in string to determine array size
        buff = buff + offset;
        //printf("This line contains %d numbers\n", numCount);
        int* nums = malloc(sizeof(int) * numCount); //allocate array
        fillArray(nums, buff); //fill new array with numbers from string
        sort(nums, 0, numCount - 1); //perform sorting algorithm on newly filled array
        for(int i = 0; i < numCount; i++){
            fprintf(out, "%d ", nums[i]);
            //printf("%d ", nums[i]);
        }
        fprintf(out, "\n");
        free(nums); //free nums to be used or any remaining lines in the input file
        //printf("%s", buff);
    }
    //close open file streams
    fclose(data);
    fclose(out);
    return 0;
}
