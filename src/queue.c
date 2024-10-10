#include "../include/queue.h"


void initQueue(Queue *q, int initSize){
    if (!q) {
        fprintf(stderr, "Error on creating the queue\n");
        exit(1);
    } 
    
    q->arr = malloc(initSize*sizeof(sequence));
    q->cur_size = 0 ;
    q->tot_size = initSize ;
    q->head = -1 ;
    q->tail = -1 ; 
}

void enqueue(Queue *q, sequence* s){
    if(!q) {
        fprintf(stderr, "Error: the queue doesn't exists\n");
        exit(-1);
    }
    if (q->cur_size < q->tot_size - 1) {
        q->tail = (q->tail + 1)%q->tot_size ;  
        q->arr[q->tail].sstart = s->sstart;
        q->arr[q->tail].send = s->send ;
        q->arr[q->tail].pivot_value = s->pivot_value ; 
    }
    q->cur_size++;
}

sequence dequeue(Queue *q){
    if(!q) {
        fprintf(stderr, "Error: the queue doesn't exists\n");
        exit(1);
    }
    if (q->cur_size > 0) {
        q->cur_size-- ; 
        return q->arr[(++q->head)%q->tot_size];
    } else {
        fprintf(stderr, "Error: the queue has no more elements\n");
        exit(2);
    }
}

int is_Empty(Queue* q){
    if( q->cur_size == 0){
        return 1 ; 
    }
    return 0 ; 
}

void release_queue(Queue* q){
    free(q->arr) ; 
}
