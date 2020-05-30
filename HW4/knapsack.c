#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct items
{
    int* value;
    int* weight;
};

int max(int a, int b) {return (a > b)? a : b;}

int* knapsack(struct items* t, int n, int W){
    int K[n+1][W+1];
    int* sack = malloc(sizeof(int) * n);

    for(int i = 0; i <= n; i++){
        for(int j = 0; j <= W; j++){
            if(i == 0 || j == 0){
                K[i][j] = 0;
            }
            else if(t->weight[i-1] <= j){
                K[i][j] = max(t->value[i-1] + K[i-1][j-t->weight[i-1]], K[i-1][j]);
            }
            else{
                K[i][j] = K[i-1][j];
            }
        }
    }
    for(int i = 0; i <= n; i++){
      for(int j = 0; j <= W; j++){
        printf("%d\t", K[i][j]);
      }
      printf("\n");
    }
    int maxVal = K[n][W];
    printf("%d\n", maxVal);

    int j = W;
    int index = 0;
    for(int i = n; i > 0 && maxVal > 0; i--){
        if(maxVal != K[i-1][j]){
           sack[index] = i-1;
           index++;
           maxVal = maxVal - t->value[i-1];
           j = j - t->weight[i-1];
           if(index <= n){
             sack[index] = -1;
           }
        }
    }
    return sack;
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    if(argc < 3){
        printf("Error: requires two arguments, number of items and max weight\n");
        exit(0);
    }
    int n = atoi(argv[1]);
    int W = atoi(argv[2]);
    struct items tems;
    tems.value = malloc(sizeof(int) * n);
    tems.weight = malloc(sizeof(int) * n);
    int v[] = {49, 4, 61, 95, 48, 16, 4, 40, 40, 41};
    int w[] = {5, 5, 6, 7, 9, 5, 8, 4, 6, 8};
    printf("Values: ");
    for(int i = 0; i < n; i++){
        tems.value[i] = v[i];
        printf("%d ", tems.value[i]);
    }
    printf("\nWeights: ");
    for(int i = 0; i < n; i++){
        tems.weight[i] = w[i];
        printf("%d ", tems.weight[i]);
    }
    printf("\n");
    int* sack = knapsack(&tems, n, W);
//    printf("Optimal weight is %d\n", sack);
    int i = 0;
    while(i < n && sack[i] != -1){
        printf("%d ", sack[i]);
        ++i;
    }
    printf("\n");
    free(sack);
    free(tems.value);
    free(tems.weight);
    return 0;
}
