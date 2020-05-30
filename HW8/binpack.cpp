#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>

using namespace std;

class items {
    private:
        int count;
        int* unsorted;
        int* sorted;
        int idx = 0;
    public:
        items(int cnt){
            this->count = cnt;
            this->unsorted = new int[cnt];
            this->sorted = new int[cnt];      
        }
        void add(int wt){
            if(this->idx < this->count){
                this->unsorted[idx] = wt;
                this->sorted[idx] = wt;
                idx++;
            }
        }
        void fill(string str){
            stringstream sstr(str);
            string num;
            while(getline(sstr, num, ' ')){
                this->add(stoi(num));
            }
            sort(this->sorted, this->sorted + this->count, greater<int>());
        }
        int itemCount(){
            return this->count;
        }
        int sindex(int idx){
            return this->sorted[idx];
        }
        int uindex(int idx){
            return this->unsorted[idx];
        }
        void print(){
            cout << "<<Items>>" << endl << "Count: " << this->count << endl;
            cout << "Sorted Weights: ";
            for(int i = 0; i < this->count; i++){
                cout << this->sorted[i] << " ";
            }
            cout << endl << "Unsorted Weights: ";
            for(int i = 0; i < this->count; i++){
                cout << this->unsorted[i] << " ";
            }
            cout << endl;
        }
};

class bin {
    private:
        int capacity = 10;
        int size = 0;
        int* content;
        bin* next = NULL;
        int idx = 0;
    public:
        bin(int cap){
            this->capacity = cap;
            this->content = new int[this->capacity];
        }
        void add(int itm){
            this->content[idx] = itm;
            this->idx++;
            this->size += itm;
        }
        void newbin(){
            if(this->next == NULL){
                this->next = new bin(capacity);
            }
        }
        bin* nextBin(){
            return this->next;
        }
        bool fits(int wt){
            if(wt <= this->capacity - this->size){
                return true;
            }
            else { return false; }
        }
        int space(){
            return this->capacity - this->size;
        }
        void print(){
            cout << "<<bin>>" << endl << "Capacity: " << this->capacity << endl;
        }
};

class binStack {   
    private:
        bin* first;
        bin* last;
        int binCapacity;
        int binCount = 1;
    public:
        binStack(int bincap){
            this->binCapacity = bincap;
            this->first = new bin(bincap);
            this->last = first;
        }
        void add(){
            this->last->newbin();
            this->last = this->last->nextBin();
            this->binCount++;
        }
        void packff(items* itm){
            this->binCount = 1;
            for(int i = 0; i < itm->itemCount(); i++){
                bin* tmp = this->first;
                while(!tmp->fits(itm->uindex(i)) && tmp->nextBin() != NULL){
                    tmp = tmp->nextBin();
                }
                if(tmp->fits(itm->uindex(i))){
                    tmp->add(itm->uindex(i));
                }
                else{
                    this->add();
                    this->last->add(itm->uindex(i));
                }
            }
        }
        void packffd(items* itm){
            this->binCount = 1;
            for(int i = 0; i < itm->itemCount(); i++){
                bin* tmp = this->first;
                while(!tmp->fits(itm->sindex(i)) && tmp->nextBin() != NULL){
                    tmp = tmp->nextBin();
                }
                if(tmp->fits(itm->sindex(i))){
                    tmp->add(itm->sindex(i));
                }
                else{
                    this->add();
                    this->last->add(itm->sindex(i));
                }
            }
        }
        void packbf(items* itm){
            this->binCount = 1;
            for(int i = 0; i < itm->itemCount(); i++){
                bin* bf = NULL;
                int bestns = this->binCapacity;
                bin* tmp = this->first;
                while(tmp->nextBin() != NULL){
                    if(tmp->space() - itm->uindex(i) < bestns && tmp->fits(itm->uindex(i))){
                        bf = tmp;
                        bestns = tmp->space() - itm->uindex(i);
                    }
                    tmp = tmp->nextBin();
                }
                if(bf == NULL){
                    this->add();
                    this->last->add(itm->uindex(i));
                }
                else{
                    bf->add(itm->uindex(i));
                }
            }

        }
        void print(){
            cout << this->binCount;
        }
};

int main(int argc, char* argv[]){
    fstream infile; //initialize file stream object
    infile.open("bin.txt", ios::in); //open input file bin.txt
    if(infile.is_open()){
        string line;
        getline(infile, line); //get number of cases from file
        int cscnt = stoi(line); //set number of cases
        int csnum = 0;
        while(csnum < cscnt){ //process all cases
            getline(infile, line);
            binStack bs(stoi(line));

            getline(infile, line);
            items tems(stoi(line));

            getline(infile, line);
            tems.fill(line);

            cout << "Test Case " << csnum + 1 << " First Fit: ";
            bs.packff(&tems);
            bs.print();
            cout << ", First Fit Decreasing: ";
            bs.packffd(&tems);
            bs.print();
            cout << ", Best Fit: ";
            bs.packbf(&tems);
            bs.print();
            cout << endl;
            
            csnum++;
        }
        infile.close();
    }
    return 0;
}
