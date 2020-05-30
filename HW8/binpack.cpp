#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>

using namespace std;

/**
 *  Stores sorted and unsorted arrays of the same list of numbers
 * */
class items
{
private:
    int count;      //Indicates size of both arrays
    int *unsorted;  //Pointer to unsorted array of integers
    int *sorted;    //Pointer to sorted array of integers
    int idx = 0;    //Used when filling array to indicate next index to be filled

public:
    /**
     * Takes integer indicating size of arrays and creates items object
     * with sorted and unsorted arrays of specified size
     * */
    items(int cnt)
    {
        this->count = cnt;              //array size
        this->unsorted = new int[cnt];  //initialize unsorted array
        this->sorted = new int[cnt];    //initialize sorted array
    }
    /**
     * Add provided item weight to next available index of both arrays
     * */
    void add(int wt)
    {
        if (this->idx < this->count)
        {
            this->unsorted[idx] = wt; //Add item to unsorted
            this->sorted[idx] = wt;   //Add item to sorted
            idx++;                    //increment to next available index
        }
    }
    /**
     * Take string of item weights, convert them to integers, and store them 
     * in the next available index of both arrays
     * */
    void fill(string str)
    {
        stringstream sstr(str); //Create string stream from string
        string num; //Stores each weight value as it is read from the stream
        while (getline(sstr, num, ' ')) //Retrieve each item weight from stream
        {
            this->add(stoi(num)); //Add item weight to arrays
        }
        sort(this->sorted, this->sorted + this->count, greater<int>()); //Sort sorted array(The name was a lie until now)
    }
    /**
     * Return size of arrays
     * */
    int itemCount()
    {
        return this->count;
    }
    /**
     * Return item weight stored at requested index of sorted array
     * */
    int sindex(int idx)
    {
        return this->sorted[idx];
    }
    /**
     * Return item weight stored at requested index of unsorted array
     * */
    int uindex(int idx)
    {
        return this->unsorted[idx];
    }
    /**
     * Print contents of sorted and unsorted arrays (For diagnostic purposes)
     * */
    void print()
    {
        cout << "<<Items>>" << endl
             << "Count: " << this->count << endl;
        cout << "Sorted Weights: ";
        for (int i = 0; i < this->count; i++)
        {
            cout << this->sorted[i] << " ";
        }
        cout << endl
             << "Unsorted Weights: ";
        for (int i = 0; i < this->count; i++)
        {
            cout << this->unsorted[i] << " ";
        }
        cout << endl;
    }
};

/**
 * Stores array of item weights, combined weight of items in the array, 
 * and weight capacity of bin
 * */
class bin
{
private:
    int capacity = 10;      //Max weight that bin can hold
    int size = 0;           //Current weight of bin
    int *content;           //Array of items in bin
    bin *next = NULL;       //Next bin in bin stack
    int idx = 0;            //Next available index in item array

public:
    /**
     * Takes integer indicating capacity of bin and creates bin object
     * */
    bin(int cap)
    {
        this->capacity = cap;                       //Set capacity of bin
        this->content = new int[this->capacity];    //Initialize item array
    }
    /**
     * Add item to next available index and add its weight to that of the bin
     * */
    void add(int itm)
    {
        this->content[idx] = itm;   //Add item weight to array
        this->idx++;                //Point to next available index
        this->size += itm;          //Increase weight of bin by that of the item
    }
    /**
     * Creates a new bin and points to it
     * (Not sure if this was the best idea but it works)
     * */
    void newbin()
    {
        if (this->next == NULL) //Only add new bin if current one is last in the stack
        {
            this->next = new bin(capacity); //Add new bin and point to it
        }
    }
    /**
     * Return address of next bin in stack
     * */
    bin *nextBin()
    {
        return this->next;
    }
    /**
     * Return true of false indicating whether 
     * a given item can fit in this bin
     * */
    bool fits(int wt)
    {
        if (wt <= this->capacity - this->size)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    /**
     * Return integer representing weight capacity remaining in bin
     * */
    int space()
    {
        return this->capacity - this->size;
    }
    /**
     * Print weight values representing contets of bin (For diagnostic purposes)
     * */
    void print()
    {
        for (int i = 0; i < this->capacity; i++)
        {
            cout << this->content[i] << " ";
        }
    }
};

/**
 * Stores a linked list of bins, pointers to the first and last bins,
 * capacity of each bin, and a count of how many bins are in the list.
 * */
class binStack
{
private:
    bin *first;         //Pointer to first bin
    bin *last;          //Pointer to last bin
    int binCapacity;    //Capacity of each bin in list
    int binCount = 1;   //Number of bins in list

public:
    /**
     * Constructs a new bin stack with single bin set as the
     * first and last in the list. The bin's capacity is set 
     * with the provided integer
     * */
    binStack(int bincap)
    {
        this->binCapacity = bincap;         //Set capacity of bins
        this->first = new bin(bincap);      //Set new bin as first
        this->last = first;                 //Set new bin as last
    }
    /**
     * Add new bin to bin stack
     * */
    void add()
    {
        this->last->newbin();                   //Create new bin at end of list
        this->last = this->last->nextBin();     //Set new bin as last
        this->binCount++;                       //Increment bin count
    }
    /**
     * Take item object and pack items contained therein in first fit ordering
     * */
    void packff(items *itm)
    {
        this->binCount = 1;                                                 //Each bin stack starts with one bin
        for (int i = 0; i < itm->itemCount(); i++)                          //Iterate through each item
        {
            bin *tmp = this->first;                                         //Start at first bin for each item
            while (!tmp->fits(itm->uindex(i)) && tmp->nextBin() != NULL)    //Iterate through bins until a suitable one is found or none remain
            {
                tmp = tmp->nextBin();                                       //Advance to next bin
            }
            if (tmp->fits(itm->uindex(i)))                                  //If current item fits in current bin add to bin
            {
                tmp->add(itm->uindex(i));                                   //Add item to bin
            }
            else                                                            //Item does not fit in existing bins
            {
                this->add();                                                //Create new bin
                this->last->add(itm->uindex(i));                            //Add item to new bin
            }
        }
    }
    /**
     * Take item object and pack items contained therein in first fit desecending ordering
     * */
    void packffd(items *itm)
    {
        this->binCount = 1;                                                 //Each bin stack starts with one bin
        for (int i = 0; i < itm->itemCount(); i++)                          //Iterate through each item
        {
            bin *tmp = this->first;                                         //Start at first bin for each item
            while (!tmp->fits(itm->sindex(i)) && tmp->nextBin() != NULL)    //Iterate through bins until a suitable one is found or none remain
            {
                tmp = tmp->nextBin();                                       //Advance to next bin
            }
            if (tmp->fits(itm->sindex(i)))                                  //If current item fits in current bin add to bin
            {
                tmp->add(itm->sindex(i));                                   //Add item to bin
            }
            else                                                            //Item does not fit in existing bins
            {
                this->add();                                                //Create new bin
                this->last->add(itm->sindex(i));                            //Add item to new bin
            }
        }
    }
    /**
     * Take item object and pack items contained therein in best fit ordering
     * */
    void packbf(items *itm)
    {
        this->binCount = 1;                                                 //Each bin stack starts with one bin
        for (int i = 0; i < itm->itemCount(); i++)                          //Iterate through each item
        {
            bin *bf = this->first;                                          //Start with first bin as best fit
            int bestns = this->binCapacity;                                 //Set bin capacity as starting best negative space
            bin *tmp = this->first;                                         //Start at first bin for each item
            while (tmp != NULL)                                             //Iterate through all bins
            {
                if (tmp->space() - itm->uindex(i) < bestns && tmp->fits(itm->uindex(i)))    //If item fits better in current bin
                {
                    bf = tmp;                                                               //Set current bin as best fit
                    bestns = tmp->space() - itm->uindex(i);                                 //Set negative space after add of best fit bin
                }
                tmp = tmp->nextBin();                                       //Advance to next bin
            }
            if (!bf->fits(itm->uindex(i)))                                  //If item doesn't fit in current best fit (probably first bin)
            {
                this->add();                                                //Add new bin
                this->last->add(itm->uindex(i));                            //Add item to new bin
            }
            else                                                            //If item does fit in current best fit
            {
                bf->add(itm->uindex(i));                                    //Add item to best fit bin
            }
        }
    }
    /**
     * Print number of bins used by packing algorithm 
     * (Not accurate if multiple packing algorithms are run
     * on the same bin stack object)
     * */
    void report()
    {
        cout << this->binCount;     //Print number of bins used
    }
    /**
     * Print contents of all bins in bin stack (For diagnostic purposes)
     * */
    void print()
    {
        bin *tmp = this->first;
        cout << endl
             << "<<Bins>>" << endl;
        while (tmp != NULL)
        {
            tmp->print();
            tmp = tmp->nextBin();
            cout << endl;
        }
        cout << endl;
    }
};

int main(int argc, char *argv[])
{
    fstream infile;                  //initialize file stream object
    infile.open("bin.txt", ios::in); //open input file bin.txt
    if (infile.is_open())            //only continue if file successfully opened
    {
        string line;
        getline(infile, line);  //get number of cases from file
        int cscnt = stoi(line); //set number of cases
        int csnum = 0;          //indicates number of cases processed
        while (csnum < cscnt)   //loop through all cases
        {
            getline(infile, line);          //get capacity of bins
            binStack bsff(stoi(line));      //initialize bin stack for first fit
            binStack bsffd(stoi(line));     //initialize bin stack for first fit decreasing
            binStack bsbf(stoi(line));      //initialize bin stack for best bit

            getline(infile, line);          //get number of items
            items tems(stoi(line));         //initialize items object with number of items

            getline(infile, line);          //get item weights
            tems.fill(line);                //fill items object with item weights

            cout << "Test Case " << csnum + 1 << " First Fit: ";
            bsff.packff(&tems);             //first fit pack items
            bsff.report();                  //print result
            // bsff.print();
            cout << ", First Fit Decreasing: ";
            bsffd.packffd(&tems);           //first fit decreasing pack items
            bsffd.report();                 //print result
            // bsffd.print();
            cout << ", Best Fit: ";
            bsbf.packbf(&tems);             //best fit pack items
            bsbf.report();                  //print result
            // bsbf.print();
            cout << endl;

            csnum++;                        //increment case number
        }
        infile.close();                     //close file
    }
    return 0;
}
