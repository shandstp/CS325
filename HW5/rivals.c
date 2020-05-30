#define _POSIX_C_SOURCE 200809L //facilitates appropriate use of getline
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//faciliates switching between teams with !team and 
//comparisons between teams using the HEEL and BABY keywords
#define HEEL false
#define BABY true

//easily check and set state of nodes in graph to determine
//which ones have been investigate and which have not
#define WHITE 1
#define GREY 2
#define BLACK 3


// stores information pertaining to each wrestler
struct wrestler {
    char name[255]; //wrestler name
    struct wrestler** rivals; //array of pointers to rival wrestlers
    int rivCnt; //indicates how many rivals a given rester has
    //indicates whether a wrestler has been examined and how throughougly
    int color; //set and check using WHITE, GREY, or BLACK keywords
    //indicates whether the wrestler is a Baby Face or Heel
    bool thisTeam; //set and check using BABY and HEEL keywords
};

// used to construct a linked list consisting of wrestlers in a queue
struct node {
    struct wrestler* this;  //current wrestler in queue
    struct wrestler* prev;  //previous wreslter in queue
    struct wrestler* next;  //next wrestler in queue
};

// stores pointers to first and last wrester in queue
struct queue {
    struct node* frst;
    struct node* last;
};

// stores names of wrestlers in each team and size of each time
struct teams {
    char** bf; //names of baby face wrestlers
    char** h;  //names of heel wrestlers
    int numbf; //number of wrestlers in baby face team
    int numh;  //number of wrestlers in heel team
};

// initialize wrestler to default values
void wrestInit(struct wrestler* w, int rc){
    memset(w->name, '\0', 255); //ensure names shorter than 255 char is terminated with \0
    w->color = WHITE; //set default node color to white
    w->thisTeam = BABY; //default team is BABY
    w->rivCnt = 0; //initialize default rival count to zero
    w->rivals = malloc(sizeof(struct wrestler*) * rc); //initialize rivals array
    for(int i = 0; i < rc; i++){
        w->rivals[i] = NULL;
    }
}

// take an array of wrestlers, an array of space separated rival pairs,
// number of wrestlers, and number of rivalries, and produce a graph containing
// restlers as verticies with edges connecting to rival wrestlers
void buildGraph(struct wrestler* w, char** r, int wc, int rc){
    for(int i = 0; i < rc; i++){ //iterate through each rivalry pair
        char* name = strtok(r[i], " \n"); //get first name in pair
        int j = 0;
        while(j < wc && (strcmp(w[j].name, name) != 0)){ //locate wrestler node with matching name
            j++;
        }
        int k = 0;
        name = strtok(NULL, " \n"); //get second name in pair
        name[strlen(name) - 1] = '\0'; //remove trailing \r
        while(k < wc && (strcmp(w[k].name, name) != 0)){
            k++;
        }
        w[j].rivals[w[j].rivCnt] = &w[k]; //connect w[j] rival to w[k] rival
        w[k].rivals[w[k].rivCnt] = &w[j]; //connect w[k] rival to w[j] rival
        //increment rival count for both wrestlers
        w[j].rivCnt++; 
        w[k].rivCnt++;
    }
}

//initialize nodes for pointing to wrestlers in queue
void nodeInit(struct node* n){
    n->prev = NULL;
    n->this = NULL;
    n->next = NULL;
}

//initialize queue
void queueInit(struct queue* q){
    q->frst = NULL;
    q->last = NULL;
}

//initialize teams
void teamsInit(struct teams* t, int num){
    t->bf = malloc(sizeof(char*) * num); //initialize baby face array
    t->h = malloc(sizeof(char*) * num); //initialize heel array
    for(int i = 0; i < num; i++){
        t->bf[i] = malloc(sizeof(char) * 255);
        t->h[i] = malloc(sizeof(char) * 255);
        t->numbf = 0;
        t->numh = 0;
    }
}

//takes a queue and a wrester and adds the latter to the former
void enq(struct queue* q, struct wrestler* w){
    struct node* wrst = malloc(sizeof(struct node)); //create new node
    wrst->this = w; //point node to wrestler 
    if(q->frst == NULL){ //if queue is empty, point both ends of queue to new node
        q->frst = wrst;
        q->last = wrst;
    }
    else{ //if queue is not empty, add new node to end of queue
        q->last->next = wrst->this;
        q->last = wrst;
    }
}

//remove and return first wrestler from front of queue
struct wrestler*  deq(struct queue* q){
    struct wrestler* w = q->frst->this; //point to wrestler at front of queue
    struct node* n = q->frst; //temporarily point to first node in queue
    if(q->frst != q->last){ //if nodes remain in queue, point start of queue to next node
        q->frst->this = q->frst->next; 
        q->frst->prev = NULL;
    }
    else{ //if no nodes remain, sent first and last pointers in queue to null
        q->frst = NULL;
        q->last = NULL;
    }
    free(n);
    return w; //return wrestler from front of queue
}

//takes a restler and teams and attempts to add the restler to the appropriate team
void addToTeam(struct wrestler* w, struct teams* t){
    int i = 0;
    if(w->thisTeam == BABY){ //if wrestler is marked as a baby face, add to baby face team
        while((strcmp(w->name, t->bf[i]) != 0) && i < t->numbf){ //find next open space on team
            i++;
        }
        strcpy(t->bf[i], w->name); //copy name to team array
        t->numbf++;  //increase team member count
    }
    else{
        while((strcmp(w->name, t->h[i]) != 0) && i < t->numh){ //find next open space on team
            i++;
        }
        strcpy(t->h[i], w->name); //copy name to team array
        t->numh++; //increase team member count
    }
}

//takes a wrestler, number of wrestlers (might be vestigial), and teams,
//returns true if all rivalries are valid and fills teams arrays with names of team members
bool bfvheels(struct wrestler* w, int wc, struct teams* t){
    w->color = GREY; //set root vertex to grey
    bool team = BABY; //set starting team as baby face
    w->thisTeam = team; //set first vertex to baby face
    addToTeam(w, t); //add first vertex to baby face team
    struct queue* q = malloc(sizeof(struct queue)); //initialize memory for queue
    queueInit(q); //initialize queue
    enq(q, w); //add current vertex to queue
    while(q->frst != NULL){ //while verticies remain in queue, continue
        struct wrestler* u = deq(q); //get first wrestler in queue
        team = !team; //switch current team
        for(int i = 0; i < u->rivCnt; i++){ //iterate through rivals of current wrestler
            if(u->rivals[i]->color == WHITE){ //check whether adjacent rival has be investigated
                u->rivals[i]->color = GREY; //set adjacent rival as investiaged
                u->rivals[i]->thisTeam = team; //set adjacent rival as current team
                addToTeam(u->rivals[i], t); //add adjacent restler to appropriate team
                enq(q, u->rivals[i]); //enque adjacent wrestler
            }
            //if current wrestler and adjacent wrestler are in the same team, return false
            else if(u->rivals[i]->thisTeam != team){ 
                return false; //rivalry is not valid
            }
        }
        u->color = BLACK; //set current node as exhausted
    }

    return true; //all rivalries are valid
}

int main(int argc, char* argv[]){
    if(argc < 2){ //program must be called with file name as argument
        printf("Error: missing input file\n");
        exit(0);
    }
    char* fn = argv[1]; //get filename
    FILE* fs = fopen(fn, "r"); 
    int errnum = errno; //get error number of file cannot be opened 
    if(fs != NULL){ //if file as been opened
        char* buff = NULL;
        size_t sz = 0;
        getline(&buff, &sz, fs); //get nubmer of wrestlers
        int wrstNum = atoi(buff); //convert wrestler count to int

        struct wrestler w[wrstNum];
        for(int i = 0; i < wrstNum; i++){
            wrestInit(&w[i], 5); //initialize array of wrestlers
        }
        for(int i = 0; i < wrstNum; i++){ 
            getline(&buff, &sz, fs); //get each wrestler name
            strcpy(w[i].name, buff); //copy name to current wrestler
            w[i].name[strlen(w[i].name) - 2] = '\0'; //remove trailing \r
        }
        getline(&buff, &sz, fs); //get number of rivals
        int rivalNum = atoi(buff); //convert to in
        char** rivals = malloc(sizeof(char*) * rivalNum);
        for(int i = 0; i < rivalNum; i++){
            rivals[i] = malloc(sizeof(char) * 255);
            getline(&buff, &sz, fs); //get each rivalry pair
            strcpy(rivals[i], buff); //copy space separated pair to array of rival names
        }
        buildGraph(w, rivals, wrstNum, rivalNum); //build undirected, unweighted graph
        bool possible = true;
        int i = 0;
        struct teams* t = malloc(sizeof(struct teams));
        teamsInit(t, wrstNum); //initialize teams
        //as long as rivalry has not been found to be invalid, continue to traverse graph
        while(possible && i < wrstNum){
            if(w[i].color == WHITE){ //if a vertex has not been investigated, traverse graph
                                     //starting from current vertex
                possible = bfvheels(&w[i], wrstNum, t);
            }
            i++;
        }
        printf("%s\n", possible ? "Yes" : "No"); //report whether it is possible or not
        if(possible){ //print teams if all rivalries are valid
            printf("Babyfaces: ");
            for(int i = 0; i < t->numbf; i++){
                printf("%s ", t->bf[i]);
            } 
            printf("\n");
            printf("Heels: ");
            for(int i = 0; i < t->numh; i++){
                printf("%s ", t->h[i]);
            }
            printf("\n");
        }
    }
    else {
        fprintf(stderr, "Error opening file: %s\n", strerror(errnum));
        exit(0);
    }
    return 0;
}
