#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(int argc, char* argv[]){
    srand(time(NULL));
    int casec = 1;
    if(argc < 2){
        cout << "Please enter the number of cases to generate: ";
        cin >> casec;
    }
    else {
        casec = stoi(argv[1]);
    }
    ofstream bintxt;
    bintxt.open("bin.txt");
    bintxt << casec << endl;
    for(int j = 0; j < casec; j++){
        int bincap = rand() % 100 + 1;
        int itemc = rand() % bincap + 1;
        int* items = new int[itemc];
        for(int i = 0; i < itemc; i++){
            items[i] = rand() % bincap + 1;
        }
        bintxt << bincap << endl << itemc << endl;
        for(int i = 0; i < itemc; i++){
            bintxt << items[i] << " ";
        }
        bintxt << endl;
        free(items);
    }
    bintxt.close();
    return 0;
}