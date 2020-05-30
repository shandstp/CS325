#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//takes an array of integers by reference along with its size, low index, high index, alpha value  and sorts the contents into ascending order
void sort(int *arr, int l, int r, float a){
    int n = r - l + 1; //compute size of sub array
    if(n == 2 && (arr[l] > arr[l+1])){ //determine if values should be swapped
        int temp = arr[l+1];
        arr[l+1] = arr[l];
        arr[l] = temp;
    }
    else if(n > 2){ //if array hasn't be sufficiently divided, divide
        int m = (((n * a) + 0.99999999999)); //multiply size of array by alpha value and add 0.9999 to ensure that ceiling is taken
        //printf("sz * a = %d\n", m);
        sort(arr, l, m-1, a);
        sort(arr, r-m+1, r, a);
        sort(arr, l, m-1, a);
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
    if(argc < 2){
        printf("Requires alpha value, 0 < alpha < 1 as argument\n");
        return 1;
    }
    //printf("alpha is %s\n", argv[1]);
    char* alpha = argv[1];
    float alph;
    if(alpha[1] == '/'){
        alpha[1] = '\0';
        char* num = &alpha[0];
        char* den = &alpha[2];
        //printf("num is %s and den is %s\n", num, den);
        int numnum = atoi(num);
        int dennum = atoi(den);
        alph = (float) numnum / (float) dennum;
        //printf("numnum is %d, dennum %d, and alph is %f\n", numnum, dennum, alph);
    }
    else{alph = atof(alpha);}
    if(alph <= 0 || alph >=1){
       printf("Alpha value must be between 0 and 1\n");
    }
    size_t strln = 0; //stores length of each line read from file
    char* buff = NULL; //stores each line read from file
    int status = 1; //indicates the status of last attempt to read from file. -1 when EOF is read
    FILE* data = fopen("data.txt", "r"); //open file stream
    FILE* out = fopen("bad.out", "w"); //open output file for writing
    while((status = getline(&buff, &strln, data) != -1)){ //read each line from file until EOF is reached
        int offset = 0;
        int numCount = countNums(buff, &offset); //count numbers in string to determine array size
        buff = buff + offset; //adjust string to start at second number
        //printf("This line contains %d numbers\n", numCount);
        int* nums = malloc(sizeof(int) * numCount); //allocate array
        fillArray(nums, buff); //fill new array with numbers from string
        sort(nums, 0, numCount-1, alph); //perform sorting algorithm on newly filled array
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
