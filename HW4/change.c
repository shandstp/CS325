#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//takes a base coin value, maximum power, and change value to make
int* makeChange(int c, int k, int n){
    int* C = malloc(sizeof(int) * (k+1)); //array index corresponds to denomination of c^k, 
                                          //value at index indicates quantity of corresponding denomination
    int sum = 0; int i = k;

    while(sum !=n){ //add coins until total sum of coins is equal to n
        int count = 0;
        int ci = pow(c,i); //get denomination value
        while(((count * ci) + sum) <= n){ //add current denomination of coin until n is met or exceeded
            ++count;
        }
        C[i] = count - 1; //revmove extra coin and store number of c^i needed at C[i]
        sum += C[i] * ci; //accumulate total sum
        --i; //continue to next smallest denomination
    }
    return C;
}

//takes a string of three values separated by tabs or spaces and three integers by reference
//extracts c, k, and n values from the string and stores them in the corresponding variables
void getVals(int* c, int* k, int* n, char str[]){
    char* num = strtok(str, " \n\t"); //get c from line
    *c = atoi(num); //convert c to int and store it
    num = strtok(NULL, " \n\t"); //get k from line
    *k = atoi(num); //convert k to int and store it
    num = strtok(NULL, " \n\t"); //get n from line
    *n = atoi(num); //convert n to int and store it
}

int main(int argc, char* argv[]){
    size_t strln = 0; //stores length of each line read from file
    char* buff = NULL; //stores each line read from file
    int status = 1; //indicates the status of last attempt to read from file. -1 when EOF is read
    FILE* data = fopen("data.txt", "r"); //open file stream
    FILE* out = fopen("change.txt", "w"); //open output file for writing
    while((status = getline(&buff, &strln, data) != -1)){ //read each line from file until EOF is reached
        int c, k, n;
        getVals(&c, &k, &n, buff); //get c, k, n values from current line 
        int* change = makeChange(c,k,n); //determine quantity of each denomination of coin needed to make change

        //print results to file with desired formatting
        fprintf(out, "\nData input: c = %d, k = %d, n = %d\n", c, k, n);
        for(int i = k; i >= 0; i--){
            int denom = pow(c, i);
            if(change[i] < 1){
                fprintf(out, "Denomination: %d Quantity: none\n", denom);
            }
            else{
                fprintf(out, "Denomination: %d Quantity: %d\n", denom, change[i]);
            }
        }
        fprintf(out, "\n");
        free(change); //free space allocated for change array
    }
    //close open file streams
    fclose(data);
    fclose(out);
    return 0;
}
