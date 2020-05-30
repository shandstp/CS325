#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    line[index] = '\0'; //terminate string containing first number
    count = atoi(line); //convert first number to integer
    *offset = index + 1; //calculate offset to new start of string
    return count; //return number count 
}


int main(int argc, char* argv[]){
    size_t strln = 0; //stores length of each line read from file
    char* buff = NULL; //stores each line read from file
    int status = 1; //indicates the status of last attempt to read from file. -1 when EOF is read
    FILE* data = fopen("data.txt", "r"); //open file stream
    FILE* out = fopen("insert.out", "w"); //open output file for writing
    while((status = getline(&buff, &strln, data) != -1)){ //read each line from file until EOF is reached
        int offset = 0;
        int numCount = countNums(buff, &offset); //count numbers in string to determine array size
        buff = buff + offset; //adjust string to start at second number
        //printf("This line contains %d numbers\n", numCount);
        int* nums = malloc(sizeof(int) * numCount); //allocate array
        fillArray(nums, buff); //fill new array with numbers from string
        sort(nums, numCount); //perform sorting algorithm on newly filled array
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
