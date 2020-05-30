#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//takes 2d array of items, array of items held by individual, value achieved thus far, number of items remaining to choose from
//and carrying capacity remaining and attempts to return the optimal combination of items and their combined value
int spree(int** itm, int* arms, int value, int n, int m){
    if(itm[1][n-1] > m){ //if true item is too large for family member to carry
        return value;
    }
    else if(itm[1][n-1] <= m){ //item is light enough to be carried
        int doAdd = 0;
        int dontAdd = 0;
        doAdd += spree(itm, arms, value + itm[0][n-1], n-1, m-itm[1][n-1]); //try adding the item
        dontAdd += spree(itm, arms, value, n-1, m); //try a different item
        if(doAdd > dontAdd){
            arms[m] = n-1; //add the item to the family members arms
            return doAdd; //return value with the price of the item added
        }
        else{
            return dontAdd; //return the previous value
        }
    }
    return value;
}

//takes 2d array of items, array of stength values, number of items, size of family, and output file stream
//attempts to calculate the optimal combination of items for each family member to take and outputs the results to file stream
void shoppers(int** itm, int* stren, int n, int fs, FILE* out){
    int maxVals[fs]; //contains max value that each family member can carry
    int** arms = malloc(sizeof(int*) * fs); //should contain index numbers of items for each family member
    for(int i = 0; i < fs; i++){
        arms[i] = malloc(sizeof(int) * n);
        for(int j = 0; j < n; j++){
            arms[i][j] = -1; //initialize all values to -1 
        }
    }
    int totalPrice = 0; //accumulate value of items for entire family
    for(int i = 0; i < fs; i++){
        maxVals[i] = spree(itm, arms[i], 0, n, stren[i]); //attempt to find optimal combination for each family member
        totalPrice += maxVals[i]; //accumulate value achieved by entire family
    }
    
    //output results to file
    fprintf(out, "Total Price %d\n", totalPrice);
    fprintf(out, "Member Items\n");
    for(int i = 0; i < fs; i++){
        fprintf(out, "%d:\t", i+1);
        for(int j = 0; j < stren[i]; j++){
            if(arms[i][j] > 0 && arms[i][j] <= 100){ //attempts to determine which items are held by each family member. Doesn't work properly
                fprintf(out, "%d ", arms[i][j]);
            }
        }
        fprintf(out, "\n");
    }
    for(int i = 0; i < fs; i++){
        free(arms[i]);
    }
    free(arms);
}

int main(int argc, char* argv[]){
    size_t strln = 0; //stores length of each line read from file
    char* buff = NULL; //stores each line read from file
    FILE* data = fopen("shopping.txt", "r"); //open file stream
    FILE* out = fopen("results.txt", "w"); //open output file for writing
    getline(&buff, &strln, data); //get number of test cases

    int numtc = atoi(buff); //convert number of test cases to integer

    for(int i = 0; i < numtc; i++){ //loop through each test case
        getline(&buff, &strln, data); //get number of items
        int n = atoi(buff); //convert number of items to integer
        int **items = malloc(sizeof(int*) * 2); //allocate space to store price and weight of items
        items[0] = malloc(sizeof(int) * n); //allocate space for price of items
        items[1] = malloc(sizeof(int) * n); //allocate space for weight of items
        for(int j = 0; j < n; j++){
            getline(&buff, &strln, data); //get price and weight line
            items[0][j] = atoi(strtok(buff, " \n")); //convert price to int and store in array
            items[1][j] = atoi(strtok(NULL, " \n")); //convert weight to int and store in array
        }
        getline(&buff, &strln, data); //get family size
        int famSz = atoi(buff); //convert family size to integer
        int famSt[famSz]; //allocate space to store stength of each family member
        for(int j = 0; j < famSz; j++){
            getline(&buff, &strln, data); //get strength of each family member
            famSt[j] = atoi(buff); //convert strength of family member to int
        }
        shoppers(items, famSt, n, famSz, out); //start shopping
        //free up space on heap
        free(items[0]);
        free(items[1]);
        free(items);
    }
    fclose(data);
    
    return 0;
}
