#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define HEEL false
#define BABY true
#define WHITE 1
#define GREY 2
#define BLACK 3

struct wrestler {
    char name[255];
    struct wrestler** rivals;
    int rivCnt;
    int color;
    bool thisTeam;
};

struct node {
    struct wrestler* this;
    struct wrestler* prev;
    struct wrestler* next;
};


struct queue {
    struct node* frst;
    struct node* last;
};

void wrestInit(struct wrestler* w, int rc){
    memset(w->name, '\0', 255);
    w->color = WHITE;
    w->thisTeam = BABY;
    w->rivCnt = 0;
    w->rivals = malloc(sizeof(struct wrestler*) * rc);
    for(int i = 0; i < rc; i++){
        w->rivals[i] = NULL;
    }
}

void buildGraph(struct wrestler* w, char** r, int wc, int rc){
    for(int i = 0; i < rc; i++){
        char* name = strtok(r[i], " \n");
        int j = 0;
        while(j < wc && (strcmp(w[j].name, name) != 0)){
            j++;
        }
        int k = 0;
        name = strtok(NULL, " \n");
        name[strlen(name) - 1] = '\0';
        while(k < wc && (strcmp(w[k].name, name) != 0)){
            k++;
        }
        w[j].rivals[w[j].rivCnt] = &w[k];
        w[k].rivals[w[k].rivCnt] = &w[j];
        w[j].rivCnt++;
        w[k].rivCnt++;
    }
}

void nodeInit(struct node* n){
    n->prev = NULL;
    n->this = NULL;
    n->next = NULL;
}

void queueInit(struct queue* q){
    q->frst = NULL;
    q->last = NULL;
}

void enq(struct queue* q, struct wrestler* w){
    struct node* wrst = malloc(sizeof(struct node));
    wrst->this = w;
    if(q->frst == NULL){
        q->frst = wrst;
        q->last = wrst;
    }
    else{
        q->last->next = wrst->this;
        q->last = wrst;
    }
}

struct wrestler*  deq(struct queue* q){
    struct wrestler* w = q->frst->this;
    struct node* n = q->frst;
    if(q->frst != q->last){
        q->frst->this = q->frst->next;
        q->frst->prev = NULL;
    }
    else{
        q->frst = NULL;
        q->last = NULL;
    }
    free(n);
    return w;
}

bool bfvheels(struct wrestler* w, int wc){
    w[0].color = GREY;
    bool team = BABY;
    w[0].thisTeam = team;
    struct queue* q = malloc(sizeof(struct queue));
    queueInit(q);
    enq(q, &w[0]);
    while(q->frst != NULL){
        struct wrestler* u = deq(q);
        team = !team;
        for(int i = 0; i < u->rivCnt; i++){
            if(u->rivals[i]->color == WHITE){
                u->rivals[i]->color = GREY;
                u->rivals[i]->thisTeam = team;
                enq(q, u->rivals[i]);
            }
            else if(u->rivals[i]->thisTeam != team){
                return false;
            }
        }
        u->color = BLACK;
    }

    return true;
}

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("Error: missing input file\n");
        exit(0);
    }
    char* fn = argv[1];
    FILE* fs = fopen(fn, "r");
    int errnum = errno;
    if(fs != NULL){
        char* buff = NULL;
        size_t sz = 0;
        getline(&buff, &sz, fs);
        int wrstNum = atoi(buff);

        struct wrestler w[wrstNum];
        for(int i = 0; i < wrstNum; i++){
            wrestInit(&w[i], 5);
        }
        for(int i = 0; i < wrstNum; i++){
            getline(&buff, &sz, fs);
            strcpy(w[i].name, buff);
            w[i].name[strlen(w[i].name) - 2] = '\0';
        }
        getline(&buff, &sz, fs);
        int rivalNum = atoi(buff);
        char** rivals = malloc(sizeof(char*) * rivalNum);
        for(int i = 0; i < rivalNum; i++){
            rivals[i] = malloc(sizeof(char) * 255);
            getline(&buff, &sz, fs);
            strcpy(rivals[i], buff);
        }
        buildGraph(w, rivals, wrstNum, rivalNum);
        bool possible = bfvheels(w, wrstNum);
        printf("%s\n", possible ? "Yes" : "No");
        if(possible){
            int bf[wrstNum];
            for(int i = 0; i < wrstNum; i++){
               bf[i] = -1;
            } 
            int count = 0;
            printf("Babyfaces: ");
            for(int i = 0; i < wrstNum; i++){
                if(w[i].thisTeam == BABY){
                    printf("%s ", w[i].name);
                    bf[count] = i;
                    count++;
                }
            }
            printf("\n");
            int j = 0;
            printf("Heels: ");
            while(bf[j] != -1 && j < wrstNum){
                for(int k = 0; k < w[bf[j]].rivCnt; k++){
                    printf("%s ", w[bf[j]].rivals[k]->name);
                }
                j++;
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
